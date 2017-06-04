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
#define GAME_OBJ_NUM_MAX		0x0ffff		// 对象数目上限

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
static GameObj* spShip1;
static GameObj* spShip2;

//纹理
AEGfxTexture *Ship;
AEGfxTexture *Ship1;
AEGfxTexture *Boss;
AEGfxTexture *Enemy1;
AEGfxTexture *Enemy2;
AEGfxTexture *Enemy3;
AEGfxTexture *Skill;
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
//战机类型标志  1或2
static int shipflag = 1;
//技能有无标志
static int skillflag = 1;
static int skillState = 0;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
// 创建/删除游戏对象
static GameObj*		gameObjCreate(unsigned long type, float scale, Vector2D* pPos, Vector2D* pVel, float dir);
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

	//纹理载入
	Ship = AEGfxTextureLoad("ship.png");
	Ship1 = AEGfxTextureLoad("ship1.png");
	Boss = AEGfxTextureLoad("boss.png");
	Enemy1 = AEGfxTextureLoad("enemy1.png");
	Enemy2 = AEGfxTextureLoad("enemy2.png");
	Enemy3 = AEGfxTextureLoad("enemy3.png");
	Skill = AEGfxTextureLoad("skill.png");
	// 创建基类的实例	
	// =====================
	// 飞船
	// =====================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_SHIP;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-20.5f, -20.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		20.5f, -20.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-20.5f, 20.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		20.5f, -20.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		20.5f, 20.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-20.5f, 20.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create object!!");

	// =====================
	// 飞船1
	// =====================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_SHIP1;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-20.5f, -20.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		20.5f, -20.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-20.5f, 20.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		20.5f, -20.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		20.5f, 20.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-20.5f, 20.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create object!!");

	// =======================
	// =======================
	// 子弹：飞船子弹 
	// =======================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_BULLET;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFF000, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFFF000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFF000, 0.0f, 0.0f);

	pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Bullet object!!");
	// =======================
	// 子弹：敌机子弹 
	// =======================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_BULLET1;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);

	pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Bullet object!!");

	// =========================
	// 小行星：用六个三角形拼成一个“圆形”
	// =========================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_ENEMY1;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-20.5f, -20.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		20.5f, -20.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-20.5f, 20.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		20.5f, -20.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		20.5f, 20.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-20.5f, 20.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	pObjBase->pMesh = AEGfxMeshEnd();

	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Asteroid object!!");

	// ========================
	// 导弹：两个三角形拼接的菱形
	// ========================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_MISSILE;

	AEGfxMeshStart();
	AEGfxTriAdd(
		0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		0.0f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f,
		0.0f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		0.0f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f,
		0.0f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f);
	pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Asteroid object!!");

	// ========================
	// 道具：两个三角形拼接的正方形
	// ========================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_SKILL;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-20.5f, -20.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		20.5f, -20.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-20.5f, 20.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		20.5f, -20.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		20.5f, 20.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-20.5f, 20.5f, 0xFFFFFFFF, 0.0f, 0.0f);
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
	spShip1 = gameObjCreate(TYPE_SHIP, SHIP_SIZE, 0, 0, 1.5707f);
	AE_ASSERT(spShip1);
	// 小行星对象实例化 并 初始化


	// 分数及飞船数目初始化
	sScore = 0;
	sShipLives = SHIP_INITIAL_NUM;

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
	if (flag_enemy == 0)
	{
		for (i = 0; i < enemys[phase]; i++)
		{
			GameObj* pObj;
			GameObj* pObj1;

			// 实例化
			pObj = gameObjCreate(TYPE_ENEMY1, 1.0f, 0, 0, 0.0f);

			AE_ASSERT(pObj);

			// 初始化: 坐标位置 朝向和尺寸大小
			switch (phase) {
			case 0:
				switch (i)
				{
				case 0:
					pObj->posCurr.x = -250.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 1:
					pObj->posCurr.x = 250.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 2:
					pObj->posCurr.x = 0.0f;
					pObj->posCurr.y = 200.0f;
					break;
				}
				break;
			case 1:
				switch (i)
				{
				case 0:
					pObj->posCurr.x = -200.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 1:
					pObj->posCurr.x = 200.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 2:
					pObj->posCurr.x = -100.0f;
					pObj->posCurr.y = 230.0f;
					break;
				case 3:
					pObj->posCurr.x = 100.0f;
					pObj->posCurr.y = 230.0f;
					break;
				case 4:
					pObj->posCurr.x = 0.0f;
					pObj->posCurr.y = 200.0f;

					pObj1 = gameObjCreate(TYPE_SKILL, 1.0f, 0, 0, 0.0f);
					pObj1->posCurr.x = 50.0f;
					pObj1->posCurr.y = 150.0f;

					pObj1->dirCurr = -1.5707f;
					pObj1->scale = 1.0f;
					break;
				}
				break;
			case 2:
				switch (i)
				{
				case 0:
					pObj->posCurr.x = -200.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 1:
					pObj->posCurr.x = 200.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 2:
					pObj->posCurr.x = -100.0f;
					pObj->posCurr.y = 230.0f;
					break;
				case 3:
					pObj->posCurr.x = 100.0f;
					pObj->posCurr.y = 230.0f;
					break;
				}
				break;
			case 3:
				switch (i)
				{
				case 0:
					pObj->posCurr.x = -200.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 1:
					pObj->posCurr.x = 200.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 2:
					pObj->posCurr.x = -100.0f;
					pObj->posCurr.y = 230.0f;
					break;
				case 3:
					pObj->posCurr.x = 100.0f;
					pObj->posCurr.y = 230.0f;
					break;
				case 4:
					pObj->posCurr.x = 0.0f;
					pObj->posCurr.y = 200.0f;
					break;
				}
				break;
			case 4:
				switch (i)
				{
				case 0:
					pObj->posCurr.x = -200.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 1:
					pObj->posCurr.x = 200.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 2:
					pObj->posCurr.x = -100.0f;
					pObj->posCurr.y = 230.0f;
					break;
				case 3:
					pObj->posCurr.x = 100.0f;
					pObj->posCurr.y = 230.0f;

					pObj1 = gameObjCreate(TYPE_SKILL, 1.0f, 0, 0, 0.0f);
					pObj1->posCurr.x = 50.0f;
					pObj1->posCurr.y = 150.0f;

					pObj1->dirCurr = -1.5707f;
					pObj1->scale = 1.0f;
					break;
				}
				break;
			case 5:
				switch (i)
				{
				case 0:
					pObj->posCurr.x = -250.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 1:
					pObj->posCurr.x = 250.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 2:
					pObj->posCurr.x = 0.0f;
					pObj->posCurr.y = 200.0f;
					break;
				}
				break;
			case 6:
				switch (i)
				{
				case 0:
					pObj->posCurr.x = -250.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 1:
					pObj->posCurr.x = 250.0f;
					pObj->posCurr.y = 250.0f;
					break;
				}
				break;
			case 7:
				switch (i)
				{
				case 0:
					pObj->posCurr.x = -200.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 1:
					pObj->posCurr.x = 200.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 2:
					pObj->posCurr.x = -100.0f;
					pObj->posCurr.y = 230.0f;
					break;
				case 3:
					pObj->posCurr.x = 100.0f;
					pObj->posCurr.y = 230.0f;
					break;
				case 4:
					pObj->posCurr.x = 0.0f;
					pObj->posCurr.y = 200.0f;

					pObj1 = gameObjCreate(TYPE_SKILL, 1.0f, 0, 0, 0.0f);
					pObj1->posCurr.x = 50.0f;
					pObj1->posCurr.y = 150.0f;

					pObj1->dirCurr = -1.5707f;
					pObj1->scale = 1.0f;
					break;
				}
				break;
			case 8:
				switch (i)
				{
				case 0:
					pObj->posCurr.x = -250.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 1:
					pObj->posCurr.x = 250.0f;
					pObj->posCurr.y = 250.0f;
					break;
				case 2:
					pObj->posCurr.x = 0.0f;
					pObj->posCurr.y = 200.0f;
					break;
				}
				break;
			case 9:
				switch (i)
				{
				case 0:
					pObj->posCurr.x = 0.0f;
					pObj->posCurr.y = 250.0f;
					break;
				}
				break;
			}
			// 朝向
			pObj->dirCurr = -1.5707f;

			pObj->scale = (i+1)*10.0f;
			flag_enemy++;
		}
		phase++;
	}

	// =========================
	//切换类型战机
	// =========================
	if (AEInputCheckTriggered('Q') && (shipflag == 2))
	{
		spShip2->flag = 0;
		spShip1 = gameObjCreate(TYPE_SHIP, SHIP_SIZE, 0, 0, 1.5707f);
		AE_ASSERT(spShip1);
		shipflag = 1;
	}
	if (AEInputCheckTriggered('W') && (shipflag == 1))
	{
		spShip1->flag = 0;
		spShip2 = gameObjCreate(TYPE_SHIP1, SHIP_SIZE, 0, 0, 1.5707f);
		AE_ASSERT(spShip2);
		shipflag = 2;
	}

	// 状态切换
	if (AEInputCheckTriggered('R'))
	{
		Next = GS_Restart;
		return;
	}
	if (AEInputCheckTriggered(VK_ESCAPE))
	{
		Next = GS_Quit;
		return;
	}
	if (AEInputCheckTriggered('2'))
	{
		Next = GS_L2;
		return;
	}

	// 飞船移动
	if (AEInputCheckCurr(VK_UP))
	{

		// 速度更新
		spShip1->velCurr.y = SHIP_SPEED;
		spShip1->velCurr.x = 0;

		// 位置更新
		Uniform_Motion(&spShip1->posCurr, spShip1->velCurr);

		if ((spShip1->posCurr.x - 1.0f< winMinX) || (spShip1->posCurr.x + 20.0f> winMaxX) || (spShip1->posCurr.y - 20.0f< winMinY) || (spShip1->posCurr.y + 20.0f > winMaxY))
		{
			spShip1->posCurr.x -= spShip1->velCurr.x * 0.95f;
			spShip1->posCurr.y -= spShip1->velCurr.y * 0.95f;
		}
	}



	if (AEInputCheckCurr(VK_DOWN))
	{
		spShip1->velCurr.y = -SHIP_SPEED;
		spShip1->velCurr.x = 0;
		Uniform_Motion(&spShip1->posCurr, spShip1->velCurr);
		if ((spShip1->posCurr.x - 20.0f< winMinX) || (spShip1->posCurr.x + 20.0f> winMaxX) || (spShip1->posCurr.y - 20.0f< winMinY) || (spShip1->posCurr.y + 20.0f > winMaxY))
		{
			spShip1->posCurr.x -= spShip1->velCurr.x * 0.95f;
			spShip1->posCurr.y -= spShip1->velCurr.y * 0.95f;
		}
		// 位置更新



	}

	if (AEInputCheckCurr(VK_LEFT))
	{
		// 速度更新
		spShip1->velCurr.x = -SHIP_SPEED;
		spShip1->velCurr.y = 0;

		// 位置更新
		Uniform_Motion(&spShip1->posCurr, spShip1->velCurr);

		if ((spShip1->posCurr.x - 20.0f< winMinX) || (spShip1->posCurr.x + 20.0f> winMaxX) || (spShip1->posCurr.y - 20.0f< winMinY) || (spShip1->posCurr.y + 20.0f > winMaxY))
		{
			spShip1->posCurr.x -= spShip1->velCurr.x * 0.95f;
			spShip1->posCurr.y -= spShip1->velCurr.y * 0.95f;
		}
	}

	if (AEInputCheckCurr(VK_RIGHT))
	{

		// 速度更新
		spShip1->velCurr.x = SHIP_SPEED;
		spShip1->velCurr.y = 0;

		// 位置更新
		Uniform_Motion(&spShip1->posCurr, spShip1->velCurr);

		if ((spShip1->posCurr.x - 20.0f< winMinX) || (spShip1->posCurr.x + 20.0f> winMaxX) || (spShip1->posCurr.y - 20.0f< winMinY) || (spShip1->posCurr.y + 20.0f > winMaxY))
		{
			spShip1->posCurr.x -= spShip1->velCurr.x * 0.95f;
			spShip1->posCurr.y -= spShip1->velCurr.y * 0.95f;
		}
	}

	// 空格键射击(创建一个子弹对象)
	if (AEInputCheckCurr(VK_SPACE) && (bul_gap % 8 == 0))
	{
		if(skillState == 0)
		{
			GameObj* pBullet;
			pBullet = gameObjCreate(TYPE_BULLET, 3.0f, 0, 0, 0.0f);
			AE_ASSERT(pBullet);
			pBullet->posCurr.x = spShip1->posCurr.x;//- 5.0f;
			pBullet->posCurr.y = spShip1->posCurr.y;
			pBullet->dirCurr = spShip1->dirCurr;
		}
		if (skillState == 1 && bul_gap <= 0xff)
		{
			printf("%x",bul_gap);
			if (bul_gap == 0xf8)
			{
				skillState = 0;
				printf("reset");
			}
			GameObj* pBullet;
			pBullet = gameObjCreate(TYPE_BULLET, 3.0f, 0, 0, 0.0f);
			AE_ASSERT(pBullet);
			pBullet->posCurr.x = spShip1->posCurr.x;//- 5.0f;
			pBullet->posCurr.y = spShip1->posCurr.y;
			pBullet->dirCurr = spShip1->dirCurr + 0.523f;
			GameObj* pBullet1;

			pBullet1 = gameObjCreate(TYPE_BULLET, 3.0f, 0, 0, 0.0f);
			AE_ASSERT(pBullet1);
			pBullet1->posCurr.x = spShip1->posCurr.x;//- 5.0f;
			pBullet1->posCurr.y = spShip1->posCurr.y;
			pBullet1->dirCurr = spShip1->dirCurr - 0.523f;
			
			GameObj* pBullet2;
			pBullet2 = gameObjCreate(TYPE_BULLET, 3.0f, 0, 0, 0.0f);
			AE_ASSERT(pBullet);
			pBullet2->posCurr.x = spShip1->posCurr.x;//- 5.0f;
			pBullet2->posCurr.y = spShip1->posCurr.y;
			pBullet2->dirCurr = spShip1->dirCurr;

		}
		

	}

	// M键使用技能
	//技能1：使用导弹
	if (AEInputCheckTriggered('M') && (shipflag == 1) && skillflag)
	{
		for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
		{
			GameObj* pInst = sGameObjList + i;

			// 不理会非活动对象
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;
			if (pInst->pObject->type == TYPE_ENEMY1 || pInst->pObject->type == TYPE_BULLET1)
			{
				pInst->flag = 0;
				continue;
			}
		}
		flag_enemy = 0;
		skillflag--;
	}
	//技能2:暂定
	if (AEInputCheckTriggered('M') && (shipflag == 2) && skillflag )
	{
		skillState = 1;
		skillflag--;
		bul_gap = 0;

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
		if (pInst->pObject->type == TYPE_ENEMY1 && (bul_gap % 50 == 0))
		{
			Vector2DSet(&added, 150.0f * (float)(frameTime)* cosf(pInst->dirCurr), 250.0f * (float)(frameTime)* sinf(pInst->dirCurr));
			Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);
			GameObj* pBullet;
			pBullet = gameObjCreate(TYPE_BULLET1, 3.0f, 0, 0, 0.0f);

			AE_ASSERT(pBullet);
			pBullet->posCurr.x = pInst->posCurr.x;//- 5.0f;
			pBullet->posCurr.y = pInst->posCurr.y - 10.0f;
			pBullet->dirCurr = pInst->dirCurr;
		}

		// 更新子弹位置
		if (pInst->pObject->type == TYPE_BULLET || pInst->pObject->type == TYPE_BULLET1)
		{
			Vector2DSet(&added, BULLET_SPEED * (float)(frameTime)* cosf(pInst->dirCurr), BULLET_SPEED * (float)(frameTime)* sinf(pInst->dirCurr));
			Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);
		}

		// 更新导弹位置
		if (pInst->pObject->type == TYPE_MISSILE)
		{
			Vector2DSet(&added, 100.0f * (float)(frameTime)* cosf(pInst->dirCurr), 100.0f * (float)(frameTime)* sinf(pInst->dirCurr));
			Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);
		}

		// 更新技能道具位置
		if (pInst->pObject->type == TYPE_SKILL)
		{
			Vector2DSet(&added, 100.0f * (float)(frameTime)* cosf(pInst->dirCurr), 100.0f * (float)(frameTime)* sinf(pInst->dirCurr));
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
	if (bul_gap >= 0xfffff)
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
		if (pInst->pObject->type == TYPE_ENEMY1)
		{
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - pInst->scale, winMaxX + pInst->scale);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - pInst->scale, winMaxY + pInst->scale);
			if ((pInst->posCurr.x < winMinX) || (pInst->posCurr.x > winMaxX) || (pInst->posCurr.y < winMinY) || (pInst->posCurr.y > winMaxY))
			{
				pInst->flag = 0;
				flag_enemy--;
			}

			continue;
		}

		// 删除超出屏幕边界的子弹
		if (pInst->pObject->type == TYPE_BULLET || pInst->pObject->type == TYPE_BULLET1)
		{
			if ((pInst->posCurr.x < winMinX) || (pInst->posCurr.x > winMaxX) || (pInst->posCurr.y < winMinY) || (pInst->posCurr.y > winMaxY))
			{
				pInst->flag = 0;


			}

			continue;
		}

		// 如导弹目标已击毁，则更新导弹方向
		if (pInst->pObject->type == TYPE_MISSILE)
		{
			GameObj* pTarget;
			int j;
			float angle;
			Vector2D newv;

			if (missile_target == -1)
			{
				// 搜索确定新目标
				for (j = 0; j < GAME_OBJ_NUM_MAX; j++)
				{
					pTarget = sGameObjList + j;

					// 跳过非活动对象或非小行星对象
					if ((pTarget->flag & FLAG_ACTIVE) == 0 || pTarget->pObject->type != TYPE_ENEMY1)
						continue;

					missile_target = j;
					break;
				}

				// 找到目标
				if (missile_target > -1)
				{
					// 确定方向
					Vector2DSub(&newv, &(pTarget->posCurr), &(pInst->posCurr));
					if (newv.x != 0)
						angle = atanf(newv.y / newv.x);
					else
						angle = 0;

					// 差向量在第二、三象限
					if ((newv.x < 0 && newv.y < 0) || (newv.x < 0 && newv.y > 0))
						angle += PI;
					// 差向量在第四象限
					if (newv.x > 0 && newv.y < 0)
						angle = 2 * PI + angle;

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
		//ship和子弹的
		if (pInst->pObject->type == TYPE_BULLET1)
		{
			for (j = 0; j < GAME_OBJ_NUM_MAX; j++)
			{
				pInstOther = sGameObjList + j;

				// 跳过非活动对象和小行星自身
				if ((pInstOther->flag & FLAG_ACTIVE) == 0 || (pInstOther->pObject->type == TYPE_ENEMY1))
					continue;

				// asteroid vs. ship
				if (pInstOther->pObject->type == TYPE_SHIP || pInstOther->pObject->type == TYPE_SHIP1)
				{
					// 碰撞检测
					if (AETestCircleToCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
					{
						// 飞船击毁
						sShipLives -= 1;

						if (sShipLives <= 0)
							// 重新开始关卡
							Next = GS_Quit;
						else
						{
							// 更新位置
							pInstOther->posCurr.x = 100.0f;
							pInstOther->posCurr.y = 80.0f;
						}
					}
				}
			}
		}
		// asteroid 与 ship / bullet / missile 的碰撞检测
		if (pInst->pObject->type == TYPE_ENEMY1)
		{
			for (j = 0; j < GAME_OBJ_NUM_MAX; j++)
			{
				pInstOther = sGameObjList + j;

				// 跳过非活动对象和小行星自身
				if ((pInstOther->flag & FLAG_ACTIVE) == 0 || (pInstOther->pObject->type == TYPE_ENEMY1))
					continue;

				// asteroid vs. ship
				if (pInstOther->pObject->type == TYPE_SHIP || pInstOther->pObject->type == TYPE_SHIP1)
				{
					// 碰撞检测
					if (AETestCircleToCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
					{
						// 飞船击毁
						sShipLives -= 1;

						if (sShipLives <= 0)
							// 重新开始关卡
							Next = GS_Restart;
						else
						{
							// 更新位置
							pInstOther->posCurr.x = 100.0f;
							pInstOther->posCurr.y = 80.0f;
						}
					}
					continue;
				}
				// asteroid vs. bullet
				if (pInstOther->pObject->type == TYPE_BULLET)
				{
					// 发生碰撞，则两者都销毁
					if (AETestCircleToCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
					{
						pInstOther->flag = 0;
						pInst->flag = 0;
						sScore += 100;
						flag_enemy--;
					}
					continue;
				}
				// asteroid vs. missile
				if (pInstOther->pObject->type == TYPE_MISSILE)
				{
					// collision detection
					if (AETestCircleToCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
					{
						pInst->flag = 0;
						missile_target = -1;
						sScore += 500;
						pInstOther->flag = 0;
						flag_enemy--;
					}
				}
			}
		}

		if (pInst->pObject->type == TYPE_SHIP || pInst->pObject->type == TYPE_SHIP1)
		{
			for (j = 0; j < GAME_OBJ_NUM_MAX; j++)
			{
				pInstOther = sGameObjList + j;

				// 跳过非活动对象和小行星自身
				if ((pInstOther->flag & FLAG_ACTIVE) == 0 || (pInstOther->pObject->type == TYPE_ENEMY1))
					continue;

				// SKILL vs. SHIP
				if (pInstOther->pObject->type == TYPE_SKILL)
				{
					// 碰撞检测
					if (AETestCircleToCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
					{
						// 
						skillflag += 1;
						pInstOther->flag = 0;

					}
					continue;
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
	AEGfxTextureSet(NULL, 0, 0);
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
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetPosition(pInst->posCurr.x, pInst->posCurr.y);
			AEGfxTextureSet(Ship, 0, 0);
			AEGfxSetTransparency(1.0f);
			AEGfxSetBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			continue;
		}
		if (pInst->pObject->type == TYPE_SHIP1)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetPosition(pInst->posCurr.x, pInst->posCurr.y);
			AEGfxTextureSet(Ship1, 0, 0);
			AEGfxSetTransparency(1.0f);
			AEGfxSetBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			continue;
		}
		if (pInst->pObject->type == TYPE_ENEMY1)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetPosition(pInst->posCurr.x, pInst->posCurr.y);
			AEGfxTextureSet(Enemy1, 0, 0);
			AEGfxSetTransparency(1.0f);
			AEGfxSetBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			continue;
		}
		if (pInst->pObject->type == TYPE_ENEMY2)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetPosition(pInst->posCurr.x, pInst->posCurr.y);
			AEGfxTextureSet(Enemy2, 0, 0);
			AEGfxSetTransparency(1.0f);
			AEGfxSetBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			continue;
		}
		if (pInst->pObject->type == TYPE_ENEMY3)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetPosition(pInst->posCurr.x, pInst->posCurr.y);
			AEGfxTextureSet(Enemy3, 0, 0);
			AEGfxSetTransparency(1.0f);
			AEGfxSetBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			continue;
		}
		if (pInst->pObject->type == TYPE_BOSS)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetPosition(pInst->posCurr.x, pInst->posCurr.y);
			AEGfxTextureSet(Boss, 0, 0);
			AEGfxSetTransparency(1.0f);
			AEGfxSetBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			continue;
		}
		if (pInst->pObject->type == TYPE_SKILL)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetPosition(pInst->posCurr.x, pInst->posCurr.y);
			AEGfxTextureSet(Skill, 0, 0);
			AEGfxSetTransparency(1.0f);
			AEGfxSetBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			continue;
		}
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// 设置绘制模式(Color or texture)
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
		// 设置所有参数(Color blend, textures, etc..)
		AEGfxSetBlendMode(AE_GFX_RM_COLOR);
		// 设置对象的2D变换矩阵，使用函数：AEGfxSetTransform
		AEGfxSetTransform(pInst->transform.m);
		// 绘制当前对象，使用函数：AEGfxMeshDraw
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}

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
	Vector2D zero = { 0.0f, 0.0f };

	AE_ASSERT_PARM(type < sGameObjBaseNum);

	// 找游戏对象列表中没用过的位置
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// 找非活动对象的位置
		if (pInst->flag == 0)
		{
			// 找到了
			pInst->pObject = sGameObjBaseList + type;
			pInst->flag = FLAG_ACTIVE;
			pInst->scale = scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->velCurr = pVel ? *pVel : zero;
			pInst->dirCurr = dir;

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
