/* Project:		GProj_Asteroid
   File Name:	Level1.c
   Author:		����
   Date:		
   Purpose:		�ؿ�1  */

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
#define GAME_OBJ_BASE_NUM_MAX	32			// �������ͣ�������ࣩ��Ŀ����
#define GAME_OBJ_NUM_MAX		2048		// ������Ŀ����

#define SHIP_INITIAL_NUM			3		// �ɴ���lives��Ŀ
#define ASTEROID_NUM				3		// С������Ŀ
#define SHIP_SIZE					30.0f	// �ɴ��ߴ�
#define SHIP_SPEED                  10.0f
#define SHIP_ROT_SPEED				(2.0f * PI)	// �ɴ���ת�ٶ�(degree/second)
#define HOMING_MISSILE_ROT_SPEED	(PI / 4.0f)	// ������ת�ٶ�(degree/second)
#define ENEMY_MAX                   10//�ܹ�ˢ�¹ֲ���
#define BULLET_SPEED				150.0f		// �ӵ��ص�ǰ������ٶ� (m/s)

#define FLAG_ACTIVE					0x00000001  // ������־

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
// ��Ϸ�������/�ṹ
typedef struct 
{
	unsigned long		type;		// ��Ϸ��������
	AEGfxVertexList*	pMesh;		// ��״
}GameObjBase;

// ��Ϸ������/�ṹ
typedef struct 
{
	GameObjBase*		pObject;	// ָ����ࣨԭʼ��״�����ͣ�
	unsigned long		flag;		// ���־
	float				scale;		// �ߴ�
	Vector2D			posCurr;	// ��ǰλ��
	Vector2D			velCurr;	// ��ǰ�ٶ�
	float				dirCurr;	// ��ǰ����
	AEMtx33				transform;	// �任����ÿһ֡����ҪΪÿһ���������
}GameObj;



AEGfxTexture *Ship;
AEGfxVertexList*	shipMesh;		// ��״
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables: 
//------------------------------------------------------------------------------
// ��Ϸ������ࣨ���ͣ��б�
static GameObjBase		sGameObjBaseList[GAME_OBJ_BASE_NUM_MAX];	// �������е�Ԫ������Ϸ��������ʵ������״������
static unsigned long	sGameObjBaseNum;							// �Ѷ������Ϸ�������

// ��Ϸ�����б�
static GameObj			sGameObjList[GAME_OBJ_NUM_MAX];				// �������е�Ԫ������Ϸ�����ʵ��
static unsigned long	sGameObjNum;								// ��Ϸ����ĸ���

// �ɴ�������Ϊ��Player�����Ե�������������������
static GameObj* spShip;

// ʣ��ķɴ�lives (lives 0 = game over)
static long				sShipLives;	

// the score = ���ٵ�С����asteroid����
static unsigned long	sScore;	

// ֻ����һ��Ѳ���������ɣ��������һ�����ɱ�־
int flag_missile = 0;
static int missile_target = -1;  // ����Ŀ���ڶ����б��е��±�

//��־
unsigned int bul_gap = 0;//�ӵ������־
unsigned int flag_enemy = 0;//�л�������ʶ
unsigned int enemys[ENEMY_MAX] = { 3,5,4,5,4,3,2,5,3,1 };
unsigned int phase = 0;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
// ����/ɾ����Ϸ����
static GameObj*		gameObjCreate (unsigned long type, float scale, Vector2D* pPos, Vector2D* pVel, float dir);
static void			gameObjDestroy(GameObj* pInst);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void Load1(void)
{
	GameObjBase* pObjBase;

	// ��ʼ����Ϸ��������ʵ���б�
	memset(sGameObjBaseList, 0, sizeof(GameObjBase) * GAME_OBJ_BASE_NUM_MAX);
	sGameObjBaseNum = 0;

	// ��ʼ����Ϸ�������ʵ���б�
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	sGameObjNum = 0;
	//�������
	Ship = AEGfxTextureLoad("w_3.png");
	AE_ASSERT_MESG(Ship, "Failed to create object!!");
	// ���������ʵ��	
	// =====================
	// �ɴ�
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
	// �ӵ����ߴ��С���򻯳������ζ���
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
	// С���ǣ�������������ƴ��һ����Բ�Ρ�
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
	// ����������������ƴ�ӵ�����
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
	
	// Ϊ��ʼ��������׼��
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ����ʵ��������Ϸ��ʼֻ�зɴ���С������Ҫʵ����
	// �ɴ�����ʵ����
	spShip = gameObjCreate(TYPE_SHIP, SHIP_SIZE, 0, 0, 1.5707f);
	AE_ASSERT(spShip);
	// С���Ƕ���ʵ���� �� ��ʼ��


	// �������ɴ���Ŀ��ʼ��
	sScore      = 0;
	sShipLives    = SHIP_INITIAL_NUM;

	// δ���ɵ���
	flag_missile = 0;


}

void Update1(void)
{

	unsigned long i;
	float winMaxX, winMaxY, winMinX, winMinY;
	double frameTime;

	// ==========================================================================================
	// ��ȡ���������ߵ����꣬�����ڷ����ƶ������ţ�����ֵ�ᷢ���仯
	// ==========================================================================================
	winMaxX = AEGfxGetWinMaxX();
	winMaxY = AEGfxGetWinMaxY();
	winMinX = AEGfxGetWinMinX();
	winMinY = AEGfxGetWinMinY();

	// ======================
	// ֡ʱ�䣺Unity�е�dt
	// ======================
	frameTime = AEFrameRateControllerGetFrameTime();

	// =========================
	// ��Ϸ�߼���Ӧ����
	// =========================
	
	//���ɵл�
	if(flag_enemy == 0)
	{
		for (i = 0; i < enemys[phase]; i++)
		{
			GameObj* pObj;
			// ʵ����
			pObj = gameObjCreate(TYPE_ASTEROID, 1.0f, 0, 0, 0.0f);
			AE_ASSERT(pObj);

			// ��ʼ��: ����λ�� ����ͳߴ��С
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
			// ����
			pObj->dirCurr = acosf(pObj->posCurr.x / ((float)sqrt(pObj->posCurr.x*pObj->posCurr.x + pObj->posCurr.y * pObj->posCurr.y))) - PI;

			pObj->scale = (i + 1) * 10.0f;
			flag_enemy++;
		}
		phase++;
	}
	
	// ״̬�л�
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

	// �ɴ��ƶ�
	if (AEInputCheckCurr(VK_UP))
	{

		// �ٶȸ���
		spShip->velCurr.y = SHIP_SPEED;
		spShip->velCurr.x = 0;

		// λ�ø���
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
		// λ�ø���



	}

	if (AEInputCheckCurr(VK_LEFT))
	{
		// �ٶȸ���
		spShip->velCurr.x = -SHIP_SPEED;
		spShip->velCurr.y = 0;

		// λ�ø���
		Uniform_Motion(&spShip->posCurr, spShip->velCurr);

		if ((spShip->posCurr.x - 20.0f< winMinX) || (spShip->posCurr.x + 20.0f> winMaxX) || (spShip->posCurr.y - 20.0f< winMinY) || (spShip->posCurr.y + 20.0f > winMaxY))
		{
			spShip->posCurr.x -= spShip->velCurr.x * 0.95f;
			spShip->posCurr.y -= spShip->velCurr.y * 0.95f;
		}
	}

	if (AEInputCheckCurr(VK_RIGHT))
	{

		// �ٶȸ���
		spShip->velCurr.x = SHIP_SPEED;
		spShip->velCurr.y = 0;

		// λ�ø���
		Uniform_Motion(&spShip->posCurr, spShip->velCurr);

		if ((spShip->posCurr.x - 20.0f< winMinX) || (spShip->posCurr.x + 20.0f> winMaxX) || (spShip->posCurr.y - 20.0f< winMinY) || (spShip->posCurr.y + 20.0f > winMaxY))
		{
			spShip->posCurr.x -= spShip->velCurr.x * 0.95f;
			spShip->posCurr.y -= spShip->velCurr.y * 0.95f;
		}
	}

	// �ո�����(����һ���ӵ�����)
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

	// M�����䵼��
	if (AEInputCheckTriggered('M'))
	{
		GameObj* spMissile;

		// ֻ��������һ������
		if (!flag_missile)
		{
			// ������������
			spMissile = gameObjCreate(TYPE_MISSILE, 10.0f, 0, 0, 0.0f);
			AE_ASSERT(spMissile);
			spMissile->posCurr = spShip->posCurr;
			spMissile->dirCurr = spShip->dirCurr;

			flag_missile = 1;
		}
	}

	// ==================================================
	// ����������������player���ƣ������ģ�λ�õȣ�
	// ==================================================
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;
		Vector2D added;

		// �����ǻ���󣬲�����
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// ����С����λ��
		if (pInst->pObject->type == TYPE_ASTEROID)
		{
			Vector2DSet(&added, 5.0f * (float)(frameTime) * cosf(pInst->dirCurr), 5.0f * (float)(frameTime) * sinf(pInst->dirCurr));
			Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);		
		}

		// �����ӵ�λ��
		if (pInst->pObject->type == TYPE_BULLET)
		{
			Vector2DSet(&added, BULLET_SPEED * (float)(frameTime) * cosf(pInst->dirCurr), BULLET_SPEED * (float)(frameTime) * sinf(pInst->dirCurr));
			Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);		
		}

		// ���µ���λ��
		if (pInst->pObject->type == TYPE_MISSILE)
		{
			Vector2DSet(&added, 100.0f * (float)(frameTime) * cosf(pInst->dirCurr), 100.0f * (float)(frameTime) * sinf(pInst->dirCurr));
			Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);
		}
	}

	// ===================================
	// ��������
	// Example:
	//		-- ������Ļ�ڶ�����֣��ɴ� С���ǣ�
	//		-- ������Ļɾ�����ӵ���
	//		-- �����������·���
	//      -- �ӵ��������
	// ===================================
	bul_gap++;
	if (bul_gap >= 0xffffffff)
	{
		bul_gap = 0;
	}
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// �����ǻ����
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		
		// �ɴ���Wrap
		if (pInst->pObject->type == TYPE_SHIP)
		{
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - SHIP_SIZE, winMaxX + SHIP_SIZE);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - SHIP_SIZE, winMaxY + SHIP_SIZE);
			continue;
		}

		// С���ǣ�Wrap
		if (pInst->pObject->type == TYPE_ASTEROID)
		{
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - pInst->scale, winMaxX + pInst->scale);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - pInst->scale, winMaxY + pInst->scale);
			continue;
		}

		// ɾ��������Ļ�߽���ӵ�
		if (pInst->pObject->type == TYPE_BULLET)
		{
			if ( (pInst->posCurr.x < winMinX) || (pInst->posCurr.x > winMaxX) || (pInst->posCurr.y < winMinY) || (pInst->posCurr.y > winMaxY) )
				pInst->flag = 0;
			continue;
		}

		// �絼��Ŀ���ѻ��٣�����µ�������
		if ( pInst->pObject->type == TYPE_MISSILE )
		{
			GameObj* pTarget;
			int j;
			float angle;
			Vector2D newv;

			if ( missile_target == -1 )
			{
				// ����ȷ����Ŀ��
				for (j = 0; j < GAME_OBJ_NUM_MAX; j++)
				{
					pTarget = sGameObjList + j;

					// �����ǻ������С���Ƕ���
					if ((pTarget->flag & FLAG_ACTIVE) == 0 || pTarget->pObject->type != TYPE_ASTEROID)
						continue;
					
					missile_target = j;
					break;
				}
				
				// �ҵ�Ŀ��
				if (missile_target > -1)
				{
					// ȷ������
					Vector2DSub(&newv,&(pTarget->posCurr),&(pInst->posCurr));
					if ( newv.x != 0)
						angle = atanf(newv.y/newv.x);
					else
						angle = 0;
				
					// �������ڵڶ���������
					if ((newv.x < 0 && newv.y < 0) || (newv.x < 0 && newv.y > 0) )
						angle += PI;
					// �������ڵ�������
					if ( newv.x > 0 && newv.y < 0)
						angle = 2*PI + angle;

					pInst->dirCurr = angle;
				}
				else
					// ���ٵ���
					pInst->flag = 0;
			}
		}
	}

	// ====================
	// ��ײ���
	// ====================
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;
		GameObj* pInstOther;
		int j;

		// ������ǻ����
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		
		// asteroid �� ship / bullet / missile ����ײ���
		if ( pInst->pObject->type == TYPE_ASTEROID )
		{
			for ( j = 0; j < GAME_OBJ_NUM_MAX; j++)
			{
				pInstOther = sGameObjList + j;

				// �����ǻ�����С��������
				if ( (pInstOther->flag & FLAG_ACTIVE) == 0 || (pInstOther->pObject->type == TYPE_ASTEROID) )
					continue;

				// asteroid vs. ship
				if ( pInstOther->pObject->type == TYPE_SHIP )
				{
					// ��ײ���
					if ( AETestCircleToCircle(&(pInst->posCurr),pInst->scale,&(pInstOther->posCurr),pInstOther->scale))
					{
						// �ɴ�����
						sShipLives -= 1;

						if ( sShipLives <= 0 )
							// ���¿�ʼ�ؿ�
								Next = GS_Restart;
						else
						{			
							// ����λ��
							pInstOther->posCurr.x = 100.0f;
							pInstOther->posCurr.y = winMinY;
						}
					}
					continue;
				}
				// asteroid vs. bullet
				if ( pInstOther->pObject->type == TYPE_BULLET )
				{
					// ������ײ�������߶�����
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
	// �������ж����2D�任����
	// =====================================
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		AEMtx33		 trans, rot, scale;
		GameObj* pInst = sGameObjList + i;
		
		// ������ǻ����
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// ���ž���
		AEMtx33Scale(&scale, pInst->scale, pInst->scale);
		// ��ת����
		AEMtx33Rot(&rot, pInst->dirCurr);
		// ƽ�ƾ���
		AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
		// ����ȷ��˳����������3�������γ�2ά�任����
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

	// ������ƶ����б��е����ж���
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// �����ǻ����
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;


		if (pInst->pObject->type == TYPE_SHIP)	
		{

			continue;
		}
		
		// ���û���ģʽ(Color or texture)
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);	
		// �������в���(Color blend, textures, etc..)
		AEGfxSetBlendMode(AE_GFX_RM_COLOR); 
		// ���ö����2D�任����ʹ�ú�����AEGfxSetTransform
		AEGfxSetTransform(pInst->transform.m);
		// ���Ƶ�ǰ����ʹ�ú�����AEGfxMeshDraw
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

	// ʹ�ú���gameObjDestroyɾ���б��еĶ���
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;
		gameObjDestroy(pInst);
	}
}

void Unload1(void)
{
	int i = 0;

	// ж�ض�����״������Դ��ʹ�ú�����AEGfxMeshFree
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
	
	// ����Ϸ�����б���û�ù���λ��
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// �ҷǻ�����λ��
		if (pInst->flag == 0)
		{
			// �ҵ���
			pInst->pObject	= sGameObjBaseList + type;
			pInst->flag		= FLAG_ACTIVE;
			pInst->scale	= scale;
			pInst->posCurr	= pPos ? *pPos : zero;
			pInst->velCurr	= pVel ? *pVel : zero;
			pInst->dirCurr	= dir;
			
			// �����´����Ķ���ʵ��
			return pInst;
		}
	}

	// λ������
	return 0;
}

// ---------------------------------------------------------------------------

void gameObjDestroy(GameObj* pInst)
{
	// ���Ѿ�����
	if (pInst->flag == 0)
		return;

	// ����
	pInst->flag = 0;
}

//
