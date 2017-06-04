/* Project:		GProj_Asteroid
   File Name:	Level1.c
   Author:		刘芳
   Date:		
   Purpose:		关卡1  */

#include <stdio.h>
#include "GameStateList.h"
#include "System.h"
#include "Level1.h"
#include "AEEngine.h"
#include "AEInput.h"
#include "Vector2d.h"
#include "Control.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
#define GAME_OBJ_BASE_NUM_MAX	32			// 对象类型（对象基类）数目上限
#define GAME_OBJ_NUM_MAX		2048		// 对象数目上限

#define SHIP_INITIAL_NUM			3		// 飞船的lives数目
#define ASTEROID_NUM				3		// 小行星数目
#define SHIP_SIZE					30.0f	// 飞船尺寸
#define SHIP_SPEED                  10.0f
#define SHIP_ROT_SPEED				(2.0f * PI)	// 飞船旋转速度(degree/second)
#define HOMING_MISSILE_ROT_SPEED	(PI / 4.0f)	// 导弹旋转速度(degree/second)
#define ENEMY_MAX                   10//总共刷新怪波数
#define BULLET_SPEED				150.0f		// 子弹沿当前方向的速度 (m/s)

#define FLAG_ACTIVE					0x00000001  // 活动对象标志

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
// 游戏对象基类/结构
typedef struct 
{
	unsigned long		type;		// 游戏对象类型
	AEGfxVertexList*	pMesh;		// 形状
}GameObjBase;

// 游戏对象类/结构
typedef struct 
{
	GameObjBase*		pObject;	// 指向基类（原始形状和类型）
	unsigned long		flag;		// 活动标志
	float				scale;		// 尺寸
	Vector2D			posCurr;	// 当前位置
	Vector2D			velCurr;	// 当前速度
	float				dirCurr;	// 当前方向
	AEMtx33				transform;	// 变换矩阵：每一帧都需要为每一个对象计算
}GameObj;



AEGfxTexture *Ship;
AEGfxVertexList*	shipMesh;		// 形状
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables: 
//------------------------------------------------------------------------------
// 游戏对象基类（类型）列表
static GameObjBase		sGameObjBaseList[GAME_OBJ_BASE_NUM_MAX];	// 该数组中的元素是游戏对象基类的实例：形状和类型
static unsigned long	sGameObjBaseNum;							// 已定义的游戏对象基类

// 游戏对象列表
static GameObj			sGameObjList[GAME_OBJ_NUM_MAX];				// 该数组中的元素是游戏对象的实例
static unsigned long	sGameObjNum;								// 游戏对象的个数

// 飞船对象：因为是Player，所以单独声明，方便程序设计
static GameObj* spShip;

// 剩余的飞船lives (lives 0 = game over)
static long				sShipLives;	

// the score = 击毁的小行星asteroid个数
static unsigned long	sScore;	

// 只允许一个巡航导弹生成，因此设置一个生成标志
int flag_missile = 0;
static int missile_target = -1;  // 导弹目标在对象列表中的下标

//标志
unsigned int bul_gap = 0;//子弹间隔标志
unsigned int flag_enemy = 0;//敌机个数标识
unsigned int enemys[ENEMY_MAX] = { 3,5,4,5,4,3,2,5,3,1 };
unsigned int phase = 0;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
// 创建/删除游戏对象
static GameObj*		gameObjCreate (unsigned long type, float scale, Vector2D* pPos, Vector2D* pVel, float dir);
static void			gameObjDestroy(GameObj* pInst);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void Load1(void)
{
	GameObjBase* pObjBase;

	// 初始化游戏对象基类的实例列表
	memset(sGameObjBaseList, 0, sizeof(GameObjBase) * GAME_OBJ_BASE_NUM_MAX);
	sGameObjBaseNum = 0;

	// 初始化游戏对象类的实例列表
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	sGameObjNum = 0;
	//纹理加载
	Ship = AEGfxTextureLoad("w_3.png");
	AE_ASSERT_MESG(Ship, "Failed to create object!!");
	// 创建基类的实例	
	// =====================
	// 飞船
	// =====================
	pObjBase	= sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type	= TYPE_SHIP;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-20.5f,  -20.5f, 0xFFFFFFFF, 0.0f, 1.0f, 
		20.5f, -20.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		- 20.5f,  20.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		20.5f, -20.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		20.5f, 20.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-20.5f, 20.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	shipMesh = pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create object!!");
	//spShip->pObject->pMesh = pObjBase->pMesh;
	// =======================
	// 子弹：尺寸很小，简化成三角形定义
	// =======================
	pObjBase		= sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type	= TYPE_BULLET;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f,  0.5f, 0xFFFFFFFF, 0.0f, 0.0f, 
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		 0.5f,  0.0f, 0xFFFFFFFF, 0.0f, 0.0f);

	pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Bullet object!!");

	// =========================
	// 小行星：用六个三角形拼成一个“圆形”
	// =========================
	pObjBase		= sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type	= TYPE_ASTEROID;

	AEGfxMeshStart();
	AEGfxTriAdd(
		0.0f,  0.0f, 0x010000FF, 0.0f, 0.0f, 
		0.5f,  0.0f, 0xFF0000FF, 0.0f, 0.0f,
		0.25f, 0.4f, 0xFF0000FF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f,  0.0f, 0x010000FF, 0.0f, 0.0f, 
		0.25f, 0.4f, 0xFF0000FF, 0.0f, 0.0f,
		-0.25f, 0.4f, 0xFF0000FF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f,  0.0f, 0x010000FF, 0.0f, 0.0f, 
		-0.5f, 0.0f, 0xFF0000FF, 0.0f, 0.0f,
		-0.25f, 0.4f, 0xFF0000FF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f,  0.0f, 0x010000FF, 0.0f, 0.0f, 
		0.5f,  0.0f, 0xFF0000FF, 0.0f, 0.0f,
		0.25f, -0.4f, 0xFF0000FF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f,  0.0f, 0x010000FF, 0.0f, 0.0f, 
		0.25f, -0.4f, 0xFF0000FF, 0.0f, 0.0f,
		-0.25f, -0.4f, 0xFF0000FF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.0f,  0.0f, 0x010000FF, 0.0f, 0.0f, 
		-0.5f, 0.0f, 0xFF0000FF, 0.0f, 0.0f,
		-0.25f, -0.4f, 0xFF0000FF, 0.0f, 0.0f);

	pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Asteroid object!!");
		
	// ========================
	// 导弹：两个三角形拼接的菱形
	// ========================
	pObjBase		= sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type	= TYPE_MISSILE;

	AEGfxMeshStart();
	AEGfxTriAdd(
		0.5f,  0.0f, 0xFFFFFF00, 0.0f, 0.0f, 
		0.0f,  0.5f, 0xFFFFFF00, 0.0f, 0.0f,
		0.0f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.5f,  0.0f, 0xFFFFFF00, 0.0f, 0.0f, 
		 0.0f,   0.5f, 0xFFFFFF00, 0.0f, 0.0f,
		 0.0f,  -0.5f, 0xFFFFFF00, 0.0f, 0.0f);
	pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Asteroid object!!");
}

void Ini1(void)
{
	GameObj* pObj;
	int i;
	
	// 为开始画对象做准备
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// 对象实例化：游戏开始只有飞船和小行星需要实例化
	// 飞船对象实例化
	spShip = gameObjCreate(TYPE_SHIP, SHIP_SIZE, 0, 0, 1.5707f);
	AE_ASSERT(spShip);
	// 小行星对象实例化 并 初始化


	// 分数及飞船数目初始化
	sScore      = 0;
	sShipLives    = SHIP_INITIAL_NUM;

	// 未生成导弹
	flag_missile = 0;


}

void Update1(void)
{

	unsigned long i;
	float winMaxX, winMaxY, winMinX, winMinY;
	double frameTime;

	// ==========================================================================================
	// 获取窗口四条边的坐标，当窗口发生移动或缩放，以下值会发生变化
	// ==========================================================================================
	winMaxX = AEGfxGetWinMaxX();
	winMaxY = AEGfxGetWinMaxY();
	winMinX = AEGfxGetWinMinX();
	winMinY = AEGfxGetWinMinY();

	// ======================
	// 帧时间：Unity中的dt
	// ======================
	frameTime = AEFrameRateControllerGetFrameTime();

	// =========================
	// 游戏逻辑响应输入
	// =========================
	
	//生成敌机
	if(flag_enemy == 0)
	{
		for (i = 0; i < enemys[phase]; i++)
		{
			GameObj* pObj;
			// 实例化
			pObj = gameObjCreate(TYPE_ASTEROID, 1.0f, 0, 0, 0.0f);
			AE_ASSERT(pObj);

			// 初始化: 坐标位置 朝向和尺寸大小
			switch (i)
			{
			case 0:
				pObj->posCurr.x = AEGfxGetWinMaxX();
				pObj->posCurr.y = 100.0f;
				break;
			case 1:
				pObj->posCurr.x = 100.0f;
				pObj->posCurr.y = AEGfxGetWinMaxY();
				break;
			case 2:
				pObj->posCurr.x = AEGfxGetWinMinX();
				pObj->posCurr.y = 100.0f;
				break;
			case 3:
				pObj->posCurr.x = 100.0f;
				pObj->posCurr.y = 100.0f;
				break;
			case 4:
				pObj->posCurr.x = 300.0f;
				pObj->posCurr.y = 100.0f;
				break;
			case 5:
				pObj->posCurr.x = 300.0f;
				pObj->posCurr.y = 300.0f;
				break;
			}
			// 朝向
			pObj->dirCurr = acosf(pObj->posCurr.x / ((float)sqrt(pObj->posCurr.x*pObj->posCurr.x + pObj->posCurr.y * pObj->posCurr.y))) - PI;

			pObj->scale = (i + 1) * 10.0f;
			flag_enemy++;
		}
		phase++;
	}
	
	// 状态切换
	if(AEInputCheckTriggered('R'))
	{
		Next = GS_Restart;
		return;
	}
	if(AEInputCheckTriggered(VK_ESCAPE))
	{
		Next = GS_Quit;
		return;
	}
	if(AEInputCheckTriggered('2'))
	{
		Next = GS_L2;
		return;
	}

	// 飞船移动
	if (AEInputCheckCurr(VK_UP))
	{

		// 速度更新
		spShip->velCurr.y = SHIP_SPEED;
		spShip->velCurr.x = 0;

		// 位置更新
		Uniform_Motion(&spShip->posCurr, spShip->velCurr);

		if ((spShip->posCurr.x - 1.0f< winMinX) || (spShip->posCurr.x + 20.0f> winMaxX) || (spShip->posCurr.y - 20.0f< winMinY) || (spShip->posCurr.y + 20.0f > winMaxY))
		{
			spShip->posCurr.x -= spShip->velCurr.x * 0.95f;
			spShip->posCurr.y -= spShip->velCurr.y * 0.95f;
		}
	}



	if (AEInputCheckCurr(VK_DOWN))
	{
		spShip->velCurr.y = -SHIP_SPEED;
		spShip->velCurr.x = 0;
		Uniform_Motion(&spShip->posCurr, spShip->velCurr);
		if ((spShip->posCurr.x - 20.0f< winMinX) || (spShip->posCurr.x + 20.0f> winMaxX) || (spShip->posCurr.y - 20.0f< winMinY) || (spShip->posCurr.y + 20.0f > winMaxY))
		{
			spShip->posCurr.x -= spShip->velCurr.x * 0.95f;
			spShip->posCurr.y -= spShip->velCurr.y * 0.95f;
		}
		// 位置更新



	}

	if (AEInputCheckCurr(VK_LEFT))
	{
		// 速度更新
		spShip->velCurr.x = -SHIP_SPEED;
		spShip->velCurr.y = 0;

		// 位置更新
		Uniform_Motion(&spShip->posCurr, spShip->velCurr);

		if ((spShip->posCurr.x - 20.0f< winMinX) || (spShip->posCurr.x + 20.0f> winMaxX) || (spShip->posCurr.y - 20.0f< winMinY) || (spShip->posCurr.y + 20.0f > winMaxY))
		{
			spShip->posCurr.x -= spShip->velCurr.x * 0.95f;
			spShip->posCurr.y -= spShip->velCurr.y * 0.95f;
		}
	}

	if (AEInputCheckCurr(VK_RIGHT))
	{

		// 速度更新
		spShip->velCurr.x = SHIP_SPEED;
		spShip->velCurr.y = 0;

		// 位置更新
		Uniform_Motion(&spShip->posCurr, spShip->velCurr);

		if ((spShip->posCurr.x - 20.0f< winMinX) || (spShip->posCurr.x + 20.0f> winMaxX) || (spShip->posCurr.y - 20.0f< winMinY) || (spShip->posCurr.y + 20.0f > winMaxY))
		{
			spShip->posCurr.x -= spShip->velCurr.x * 0.95f;
			spShip->posCurr.y -= spShip->velCurr.y * 0.95f;
		}
	}

	// 空格键射击(创建一个子弹对象)
	if (AEInputCheckCurr(VK_SPACE) && (bul_gap%8 == 0))
	{
		GameObj* pBullet;
		GameObj* pBullet1;
		// create a bullet
		pBullet = gameObjCreate(TYPE_BULLET, 3.0f, 0, 0, 0.0f);
		AE_ASSERT(pBullet);
		pBullet->posCurr.x = spShip->posCurr.x - 5.0f;
		pBullet->posCurr.y = spShip->posCurr.y;
		pBullet->dirCurr = spShip->dirCurr;
		pBullet1 = gameObjCreate(TYPE_BULLET, 3.0f, 0, 0, 0.0f);
		AE_ASSERT(pBullet1);
		pBullet1->posCurr.x = spShip->posCurr.x+5.0f;
		pBullet1->posCurr.y = spShip->posCurr.y;
		pBullet1->dirCurr = spShip->dirCurr;
	}

	// M键发射导弹
	if (AEInputCheckTriggered('M'))
	{
		GameObj* spMissile;

		// 只允许生成一个导弹
		if (!flag_missile)
		{
			// 创建导弹对象
			spMissile = gameObjCreate(TYPE_MISSILE, 10.0f, 0, 0, 0.0f);
			AE_ASSERT(spMissile);
			spMissile->posCurr = spShip->posCurr;
			spMissile->dirCurr = spShip->dirCurr;

			flag_missile = 1;
		}
	}

	// ==================================================
	// 更新所有其它（非player控制）活动对象的（位置等）
	// ==================================================
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;
		Vector2D added;

		// 遇到非活动对象，不处理
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// 更新小行星位置
		if (pInst->pObject->type == TYPE_ASTEROID)
		{
			Vector2DSet(&added, 5.0f * (float)(frameTime) * cosf(pInst->dirCurr), 5.0f * (float)(frameTime) * sinf(pInst->dirCurr));
			Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);		
		}

		// 更新子弹位置
		if (pInst->pObject->type == TYPE_BULLET)
		{
			Vector2DSet(&added, BULLET_SPEED * (float)(frameTime) * cosf(pInst->dirCurr), BULLET_SPEED * (float)(frameTime) * sinf(pInst->dirCurr));
			Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);		
		}

		// 更新导弹位置
		if (pInst->pObject->type == TYPE_MISSILE)
		{
			Vector2DSet(&added, 100.0f * (float)(frameTime) * cosf(pInst->dirCurr), 100.0f * (float)(frameTime) * sinf(pInst->dirCurr));
			Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);
		}
	}

	// ===================================
	// 其它更新
	// Example:
	//		-- 超出屏幕在对向出现（飞船 小行星）
	//		-- 超出屏幕删除（子弹）
	//		-- 导弹：计算新方向
	//      -- 子弹间隔增长
	// ===================================
	bul_gap++;
	if (bul_gap >= 0xffffffff)
	{
		bul_gap = 0;
	}
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// 不理会非活动对象
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		
		// 飞船：Wrap
		if (pInst->pObject->type == TYPE_SHIP)
		{
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - SHIP_SIZE, winMaxX + SHIP_SIZE);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - SHIP_SIZE, winMaxY + SHIP_SIZE);
			continue;
		}

		// 小行星：Wrap
		if (pInst->pObject->type == TYPE_ASTEROID)
		{
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - pInst->scale, winMaxX + pInst->scale);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - pInst->scale, winMaxY + pInst->scale);
			continue;
		}

		// 删除超出屏幕边界的子弹
		if (pInst->pObject->type == TYPE_BULLET)
		{
			if ( (pInst->posCurr.x < winMinX) || (pInst->posCurr.x > winMaxX) || (pInst->posCurr.y < winMinY) || (pInst->posCurr.y > winMaxY) )
				pInst->flag = 0;
			continue;
		}

		// 如导弹目标已击毁，则更新导弹方向
		if ( pInst->pObject->type == TYPE_MISSILE )
		{
			GameObj* pTarget;
			int j;
			float angle;
			Vector2D newv;

			if ( missile_target == -1 )
			{
				// 搜索确定新目标
				for (j = 0; j < GAME_OBJ_NUM_MAX; j++)
				{
					pTarget = sGameObjList + j;

					// 跳过非活动对象或非小行星对象
					if ((pTarget->flag & FLAG_ACTIVE) == 0 || pTarget->pObject->type != TYPE_ASTEROID)
						continue;
					
					missile_target = j;
					break;
				}
				
				// 找到目标
				if (missile_target > -1)
				{
					// 确定方向
					Vector2DSub(&newv,&(pTarget->posCurr),&(pInst->posCurr));
					if ( newv.x != 0)
						angle = atanf(newv.y/newv.x);
					else
						angle = 0;
				
					// 差向量在第二、三象限
					if ((newv.x < 0 && newv.y < 0) || (newv.x < 0 && newv.y > 0) )
						angle += PI;
					// 差向量在第四象限
					if ( newv.x > 0 && newv.y < 0)
						angle = 2*PI + angle;

					pInst->dirCurr = angle;
				}
				else
					// 销毁导弹
					pInst->flag = 0;
			}
		}
	}

	// ====================
	// 碰撞检测
	// ====================
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;
		GameObj* pInstOther;
		int j;

		// 不处理非活动对象
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		
		// asteroid 与 ship / bullet / missile 的碰撞检测
		if ( pInst->pObject->type == TYPE_ASTEROID )
		{
			for ( j = 0; j < GAME_OBJ_NUM_MAX; j++)
			{
				pInstOther = sGameObjList + j;

				// 跳过非活动对象和小行星自身
				if ( (pInstOther->flag & FLAG_ACTIVE) == 0 || (pInstOther->pObject->type == TYPE_ASTEROID) )
					continue;

				// asteroid vs. ship
				if ( pInstOther->pObject->type == TYPE_SHIP )
				{
					// 碰撞检测
					if ( AETestCircleToCircle(&(pInst->posCurr),pInst->scale,&(pInstOther->posCurr),pInstOther->scale))
					{
						// 飞船击毁
						sShipLives -= 1;

						if ( sShipLives <= 0 )
							// 重新开始关卡
								Next = GS_Restart;
						else
						{			
							// 更新位置
							pInstOther->posCurr.x = 100.0f;
							pInstOther->posCurr.y = winMinY;
						}
					}
					continue;
				}
				// asteroid vs. bullet
				if ( pInstOther->pObject->type == TYPE_BULLET )
				{
					// 发生碰撞，则两者都销毁
					if (AETestCircleToCircle(&(pInst->posCurr),pInst->scale,&(pInstOther->posCurr),pInstOther->scale))
					{
						pInstOther->flag = 0;
						pInst->flag = 0;
						sScore += 100;
						flag_enemy--;
					}
					continue;
				}			
				// asteroid vs. missile
				if ( pInstOther->pObject->type == TYPE_MISSILE )
				{
					// collision detection
					if (AETestCircleToCircle(&(pInst->posCurr),pInst->scale,&(pInstOther->posCurr),pInstOther->scale))
					{
						pInst->flag = 0;
						missile_target = -1;
						sScore += 500;
					}
				}
			}
		}
	}

	// =====================================
	// 计算所有对象的2D变换矩阵
	// =====================================
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		AEMtx33		 trans, rot, scale;
		GameObj* pInst = sGameObjList + i;
		
		// 不处理非活动对象
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// 缩放矩阵
		AEMtx33Scale(&scale, pInst->scale, pInst->scale);
		// 旋转矩阵
		AEMtx33Rot(&rot, pInst->dirCurr);
		// 平移矩阵
		AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
		// 以正确的顺序连乘以上3个矩阵形成2维变换矩阵
		AEMtx33Concat(&(pInst->transform), &trans, &rot);
		AEMtx33Concat(&(pInst->transform), &(pInst->transform), &scale);
	}
}

void Draw1(void)
{
	char strBuffer[1024];
	unsigned long i;
	double frameTime;

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(Ship, 0, 0);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 逐个绘制对象列表中的所有对象
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// 跳过非活动对象
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;


		if (pInst->pObject->type == TYPE_SHIP)	
		{

			continue;
		}
		
		// 设置绘制模式(Color or texture)
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);	
		// 设置所有参数(Color blend, textures, etc..)
		AEGfxSetBlendMode(AE_GFX_RM_COLOR); 
		// 设置对象的2D变换矩阵，使用函数：AEGfxSetTransform
		AEGfxSetTransform(pInst->transform.m);
		// 绘制当前对象，使用函数：AEGfxMeshDraw
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);

	}

	//AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(spShip->posCurr.x, spShip->posCurr.y);
	AEGfxTextureSet(Ship, 0, 0);
	AEGfxSetTransparency(1.0f);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxMeshDraw(spShip->pObject->pMesh, AE_GFX_MDM_TRIANGLES);

}

void Free1(void)
{
	int i = 0;

	// 使用函数gameObjDestroy删除列表中的对象
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;
		gameObjDestroy(pInst);
	}
}

void Unload1(void)
{
	int i = 0;

	// 卸载对象形状定义资源，使用函数：AEGfxMeshFree
	for (i = 0; i < GAME_OBJ_BASE_NUM_MAX; i++)
	{
		GameObjBase* pGamObjBase = sGameObjBaseList + i;
		AEGfxMeshFree(pGamObjBase->pMesh);
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
GameObj* gameObjCreate(unsigned long type, float scale, Vector2D* pPos, Vector2D* pVel, float dir)
{
	unsigned long i;
	Vector2D zero = { 0.0f, 0.0f};

	AE_ASSERT_PARM(type < sGameObjBaseNum);
	
	// 找游戏对象列表中没用过的位置
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// 找非活动对象的位置
		if (pInst->flag == 0)
		{
			// 找到了
			pInst->pObject	= sGameObjBaseList + type;
			pInst->flag		= FLAG_ACTIVE;
			pInst->scale	= scale;
			pInst->posCurr	= pPos ? *pPos : zero;
			pInst->velCurr	= pVel ? *pVel : zero;
			pInst->dirCurr	= dir;
			
			// 返回新创建的对象实例
			return pInst;
		}
	}

	// 位置满了
	return 0;
}

// ---------------------------------------------------------------------------

void gameObjDestroy(GameObj* pInst)
{
	// 若已经销毁
	if (pInst->flag == 0)
		return;

	// 销毁
	pInst->flag = 0;
}

//
