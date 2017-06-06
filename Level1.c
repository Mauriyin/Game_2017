/* Project:		GProj_Asteroid
File Name:	Level1.c
Author:		���ơ���˧
Date:
Purpose:		�ؿ�1  */

#include <stdio.h>
#include "GameStateList.h"
#include "System.h"
#include "Level1.h"
#include "AEEngine.h"
#include "AEInput.h"
#include "Vector2d.h"
#include "Math2D.h"
#include "Control.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
#define GAME_OBJ_BASE_NUM_MAX	32			// �������ͣ�������ࣩ��Ŀ����
#define GAME_OBJ_NUM_MAX		0x0ffff		// ������Ŀ����

#define SHIP_INITIAL_NUM			3		// �ɴ���lives��Ŀ
#define ASTEROID_NUM				3		// С������Ŀ
#define SHIP_SIZE					30.0f	// �ɴ��ߴ�
#define SHIP_SPEED                  10.0f
#define SHIP_ROT_SPEED				(2.0f * PI)	// �ɴ���ת�ٶ�(degree/second)
#define HOMING_MISSILE_ROT_SPEED	(PI / 4.0f)	// ������ת�ٶ�(degree/second)
#define ENEMY_MAX                   10//�ܹ�ˢ�¹ֲ���
#define BULLET_SPEED				150.0f		// �ӵ��ص�ǰ������ٶ� (m/s)
#define ENEMY_SIZE                  30.0f
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
static GameObj* spShip1;
static GameObj* spShip2;

//����
AEGfxTexture *Ship;
AEGfxTexture *Ship1;
AEGfxTexture *Boss;
AEGfxTexture *Enemy1;
AEGfxTexture *Enemy2;
AEGfxTexture *Enemy3;
AEGfxTexture *Skill;
// ʣ��ķɴ�lives (lives 0 = game over)
static long		sShipLives;

// the score = ���ٵ�С����asteroid����
static unsigned long	sScore;

// ֻ����һ��Ѳ���������ɣ��������һ�����ɱ�־
int flag_missile = 0;
static int missile_target = -1;  // ����Ŀ���ڶ����б��е��±�

								 //��־
unsigned int bul_gap = 0;//�ӵ������־
unsigned int flag_enemy = 0;//�л�������ʶ
unsigned int enemys[ENEMY_MAX] = { 5,5,4,5,4,3,2,5,3,1 };
unsigned int phase = 0;
//ս�����ͱ�־  1��2
static int shipflag = 1;
//�������ޱ�־
static int skillflag = 1;
static int skillState = 0;
float bossSpeed = 50.0f;
int bossHealth = 50;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
// ����/ɾ����Ϸ����
static GameObj*		gameObjCreate(unsigned long type, float scale, Vector2D* pPos, Vector2D* pVel, float dir);
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

	//��������
	Ship = AEGfxTextureLoad("ship.png");
	Ship1 = AEGfxTextureLoad("ship1.png");
	Boss = AEGfxTextureLoad("boss.png");
	Enemy1 = AEGfxTextureLoad("enemy1.png");
	Enemy2 = AEGfxTextureLoad("enemy2.png");
	Enemy3 = AEGfxTextureLoad("enemy3.png");
	Skill = AEGfxTextureLoad("skill.png");
	// ���������ʵ��	
	// =====================
	// �ɴ�
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
	// �ɴ�1
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
	// �ӵ����ɴ��ӵ� 
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
	// �ӵ����л��ӵ� 
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
	// �л�1
	// =========================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_ENEMY1;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-50.5f, -50.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		50.5f, -50.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-50.5f, 50.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		50.5f, -50.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		50.5f, 50.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-50.5f, 50.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	pObjBase->pMesh = AEGfxMeshEnd();

	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Asteroid object!!");


	// ========================
	// ����������������ƴ�ӵ�����
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
	// ���ߣ�����������ƴ�ӵ�������
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

	// =========================
	// �л�2
	// =========================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_ENEMY2;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-50.5f, -50.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		50.5f, -50.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-50.5f, 50.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		50.5f, -50.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		50.5f, 50.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-50.5f, 50.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	pObjBase->pMesh = AEGfxMeshEnd();

	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Asteroid object!!");

	// =========================
	// �л�3
	// =========================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_ENEMY3;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-50.5f, -50.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		50.5f, -50.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-50.5f, 50.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		50.5f, -50.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		50.5f, 50.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-50.5f, 50.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	pObjBase->pMesh = AEGfxMeshEnd();

	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Asteroid object!!");

	// =========================
	// Boss
	// =========================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_BOSS;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-300.5f, -100.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		300.5f, -100.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-300.5f, 100.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		300.5f, -100.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		300.5f, 100.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-300.5f, 100.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	pObjBase->pMesh = AEGfxMeshEnd();


	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Asteroid object!!");
}

void Ini1(void)
{


	// Ϊ��ʼ��������׼��
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ����ʵ��������Ϸ��ʼֻ�зɴ���С������Ҫʵ����
	// �ɴ�����ʵ����
	spShip1 = gameObjCreate(TYPE_SHIP, SHIP_SIZE, 0, 0, 1.5707f);
	AE_ASSERT(spShip1);
	// С���Ƕ���ʵ���� �� ��ʼ��


	// �������ɴ���Ŀ��ʼ��
	sScore = 0;
	sShipLives = SHIP_INITIAL_NUM;

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
	if (flag_enemy == 0)
	{
		for (i = 0; i < enemys[phase]; i++)
		{
			
			GameObj* pObj;//�л�1
			GameObj* pObj1;//���ܵ���
			if (phase < 9) {
				if ((phase + 2) % 4 == 0) {
					pObj = gameObjCreate(TYPE_SKILL, 1.0f, 0, 0, 0.0f);
					AE_ASSERT(pObj);
					pObj->posCurr.x = 0.0f;
					pObj->posCurr.y = 250.0f;
					// ����
					pObj->dirCurr = -1.5707f;
					pObj->scale = ENEMY_SIZE;

				}
				if (i < 2) {
					// ʵ����
					pObj = gameObjCreate(TYPE_ENEMY1, 1.0f, 0, 0, 0.0f);
					AE_ASSERT(pObj);
					pObj->posCurr.x = -200.0f;
					pObj->posCurr.y = 250.0f;
					// ����
					pObj->dirCurr = -1.5707f;

					pObj->scale = ENEMY_SIZE;

					pObj = gameObjCreate(TYPE_ENEMY1, 1.0f, 0, 0, 0.0f);
					AE_ASSERT(pObj);
					pObj->posCurr.x = 200.0f;
					pObj->posCurr.y = 250.0f;
					// ����
					pObj->dirCurr = -1.5707f;

					pObj->scale = ENEMY_SIZE;
					
					i = 1;
					flag_enemy = 2;
				}
				else if (i == 2 && enemys[phase] == 3) {
					pObj = gameObjCreate(TYPE_ENEMY2, 1.0f, 0, 0, 0.0f);
					AE_ASSERT(pObj);
					pObj->posCurr.x = 0.0f;
					pObj->posCurr.y = 200.0f;
					// ����
					pObj->dirCurr = -1.5707f;

					pObj->scale = ENEMY_SIZE;
					flag_enemy++;
				}
				else if (i == 2 && enemys[phase] > 3) {
					pObj = gameObjCreate(TYPE_ENEMY2, 1.0f, 0, 0, 0.0f);
					AE_ASSERT(pObj);
					pObj->posCurr.x = -125.0f;
					pObj->posCurr.y = 230.0f;
					// ����
					pObj->dirCurr = -1.5707f;
					pObj->scale = ENEMY_SIZE;
					pObj = gameObjCreate(TYPE_ENEMY2, 1.0f, 0, 0, 0.0f);
					AE_ASSERT(pObj);
					pObj->posCurr.x = 125.0f;
					pObj->posCurr.y = 230.0f;
					// ����
					pObj->dirCurr = -1.5707f;
					pObj->scale = ENEMY_SIZE;
					i = 3;
					flag_enemy = 4;
				}
				else if (i == 4 && enemys[phase] == 5) {
					pObj = gameObjCreate(TYPE_ENEMY3, 1.0f, 0, 0, 0.0f);
					AE_ASSERT(pObj);
					pObj->posCurr.x = 0.0f;
					pObj->posCurr.y = 200.0f;
					// ����
					pObj->dirCurr = -1.5707f;
					pObj->scale = ENEMY_SIZE;

					flag_enemy = 5;
				}
				//printf("%d\n", phase);
			}
			if(phase == 9){
				pObj = gameObjCreate(TYPE_BOSS, 1.0f, 0, 0, 0.0f);
				AE_ASSERT(pObj);
				pObj->posCurr.x = 0.0f;
				pObj->posCurr.y = 200.0f;
				// ����
				pObj->dirCurr = -1.5707f;
				pObj->scale = 100.0f;
				flag_enemy = 1;
			}
			
		}
		phase++;
	}




	// =========================
	//�л�����ս��
	// =========================
	if (AEInputCheckTriggered('Z') && (shipflag == 2))
	{
		spShip2->flag = 0;
		spShip1 = gameObjCreate(TYPE_SHIP, SHIP_SIZE, 0, 0, 1.5707f);
		AE_ASSERT(spShip1);
		shipflag = 1;
	}
	if (AEInputCheckTriggered('X') && (shipflag == 1))
	{
		spShip1->flag = 0;
		spShip2 = gameObjCreate(TYPE_SHIP1, SHIP_SIZE, 0, 0, 1.5707f);
		AE_ASSERT(spShip2);
		shipflag = 2;
	}

	// ״̬�л�
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

	// �ɴ��ƶ�
	if (AEInputCheckCurr(VK_UP))
	{

		// �ٶȸ���
		spShip1->velCurr.y = SHIP_SPEED;
		spShip1->velCurr.x = 0;

		// λ�ø���
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
		// λ�ø���



	}

	if (AEInputCheckCurr(VK_LEFT))
	{
		// �ٶȸ���
		spShip1->velCurr.x = -SHIP_SPEED;
		spShip1->velCurr.y = 0;

		// λ�ø���
		Uniform_Motion(&spShip1->posCurr, spShip1->velCurr);

		if ((spShip1->posCurr.x - 20.0f< winMinX) || (spShip1->posCurr.x + 20.0f> winMaxX) || (spShip1->posCurr.y - 20.0f< winMinY) || (spShip1->posCurr.y + 20.0f > winMaxY))
		{
			spShip1->posCurr.x -= spShip1->velCurr.x * 0.95f;
			spShip1->posCurr.y -= spShip1->velCurr.y * 0.95f;
		}
	}

	if (AEInputCheckCurr(VK_RIGHT))
	{

		// �ٶȸ���
		spShip1->velCurr.x = SHIP_SPEED;
		spShip1->velCurr.y = 0;

		// λ�ø���
		Uniform_Motion(&spShip1->posCurr, spShip1->velCurr);

		if ((spShip1->posCurr.x - 20.0f< winMinX) || (spShip1->posCurr.x + 20.0f> winMaxX) || (spShip1->posCurr.y - 20.0f< winMinY) || (spShip1->posCurr.y + 20.0f > winMaxY))
		{
			spShip1->posCurr.x -= spShip1->velCurr.x * 0.95f;
			spShip1->posCurr.y -= spShip1->velCurr.y * 0.95f;
		}
	}

	// �ո�����(����һ���ӵ�����)
	if (AEInputCheckCurr(VK_SPACE) && (bul_gap % 8 == 0))
	{
		if (skillState == 0)
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

	// M��ʹ�ü���
	//����1��ʹ�õ���
	if (AEInputCheckTriggered('B') && (shipflag == 1) && skillflag)
	{
		for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
		{
			GameObj* pInst = sGameObjList + i;

			// �����ǻ����
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;
			if (pInst->pObject->type == TYPE_ENEMY1 || pInst->pObject->type == TYPE_BULLET1|| pInst->pObject->type == TYPE_ENEMY2|| pInst->pObject->type == TYPE_ENEMY3)
			{
				pInst->flag = 0;
				continue;
			}
		}
		flag_enemy = 0;
		skillflag--;
	}
	//����2:�ݶ�
	if (AEInputCheckTriggered('B') && (shipflag == 2) && skillflag)
	{
		skillState = 1;
		skillflag--;
		bul_gap = 0;
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
		if ((pInst->pObject->type == TYPE_ENEMY1|| pInst->pObject->type == TYPE_ENEMY2||pInst->pObject->type == TYPE_ENEMY3) )
		{
			Vector2DSet(&added, 30.0f * (float)(frameTime)* cosf(pInst->dirCurr), 30.0f * (float)(frameTime)* sinf(pInst->dirCurr));
			Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);
			pInst->scale = ENEMY_SIZE;
			if (bul_gap % 50 == 0)
			{
				GameObj* pBullet;
				pBullet = gameObjCreate(TYPE_BULLET1, 3.0f, 0, 0, 0.0f);

				AE_ASSERT(pBullet);
				pBullet->posCurr.x = pInst->posCurr.x;//- 5.0f;
				pBullet->posCurr.y = pInst->posCurr.y - 10.0f;
				pBullet->dirCurr = pInst->dirCurr;
			}

			
		}

		// �����ӵ�λ��
		if (pInst->pObject->type == TYPE_BULLET || pInst->pObject->type == TYPE_BULLET1)
		{
			Vector2DSet(&added, BULLET_SPEED * (float)(frameTime)* cosf(pInst->dirCurr), BULLET_SPEED * (float)(frameTime)* sinf(pInst->dirCurr));
			Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);
		}

		// ���µ���λ��
		if (pInst->pObject->type == TYPE_MISSILE)
		{
			Vector2DSet(&added, 100.0f * (float)(frameTime)* cosf(pInst->dirCurr), 100.0f * (float)(frameTime)* sinf(pInst->dirCurr));
			Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);
		}

		// ���¼��ܵ���λ��
		if (pInst->pObject->type == TYPE_SKILL)
		{
			Vector2DSet(&added, 100.0f * (float)(frameTime)* cosf(pInst->dirCurr), 100.0f * (float)(frameTime)* sinf(pInst->dirCurr));
			Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);

		}
		//����Bossλ��
		if (pInst->pObject->type == TYPE_BOSS )
		{
			Vector2DSet(&added, bossSpeed * (float)(frameTime)* sin(pInst->dirCurr), bossSpeed * (float)(frameTime)* cos(pInst->dirCurr));
			Vector2DAdd(&pInst->posCurr, &pInst->posCurr, &added);
			if ((pInst->posCurr.x - 200.0f< winMinX) || (pInst->posCurr.x + 200.0f> winMaxX) )
			{
				bossSpeed = -bossSpeed;
			}
			if (bul_gap % 20 == 0) 
			{
				GameObj* pBullet;
				pBullet = gameObjCreate(TYPE_BULLET1, 3.0f, 0, 0, 0.0f);
				AE_ASSERT(pBullet);
				pBullet->posCurr.x = pInst->posCurr.x;//- 5.0f;
				pBullet->posCurr.y = pInst->posCurr.y - 10.0f;
				pBullet->dirCurr = pInst->dirCurr;

				GameObj* pBullet1;
				pBullet1 = gameObjCreate(TYPE_BULLET1, 3.0f, 0, 0, 0.0f);
				AE_ASSERT(pBullet1);
				pBullet1->posCurr.x = pInst->posCurr.x;//- 5.0f;
				pBullet1->posCurr.y = pInst->posCurr.y - 10.0f;
				pBullet1->dirCurr = pInst->dirCurr - 0.523f;

				GameObj* pBullet2;
				pBullet2 = gameObjCreate(TYPE_BULLET1, 3.0f, 0, 0, 0.0f);
				AE_ASSERT(pBullet2);
				pBullet2->posCurr.x = pInst->posCurr.x;//- 5.0f;
				pBullet2->posCurr.y = pInst->posCurr.y - 10.0f;
				pBullet2->dirCurr = pInst->dirCurr + 0.523f;
				GameObj* pBullet3;
				pBullet3 = gameObjCreate(TYPE_BULLET1, 3.0f, 0, 0, 0.0f);
				AE_ASSERT(pBullet3);
				pBullet3->posCurr.x = pInst->posCurr.x;//- 5.0f;
				pBullet3->posCurr.y = pInst->posCurr.y - 10.0f;
				pBullet3->dirCurr = pInst->dirCurr - 1.0f;

				GameObj* pBullet4;
				pBullet4 = gameObjCreate(TYPE_BULLET1, 3.0f, 0, 0, 0.0f);
				AE_ASSERT(pBullet4);
				pBullet4->posCurr.x = pInst->posCurr.x;//- 5.0f;
				pBullet4->posCurr.y = pInst->posCurr.y - 10.0f;
				pBullet4->dirCurr = pInst->dirCurr + 1.0f;
			}
			

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
		if (pInst->pObject->type == TYPE_SHIP|| pInst->pObject->type == TYPE_SHIP1)
		{
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - SHIP_SIZE, winMaxX + SHIP_SIZE);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - SHIP_SIZE, winMaxY + SHIP_SIZE);
			continue;
		}

		// С���ǣ�Wrap
		if (pInst->pObject->type == TYPE_ENEMY1|| pInst->pObject->type == TYPE_ENEMY2||pInst->pObject->type == TYPE_ENEMY3)
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

		// ɾ��������Ļ�߽���ӵ�
		if (pInst->pObject->type == TYPE_BULLET || pInst->pObject->type == TYPE_BULLET1)
		{
			if ((pInst->posCurr.x < winMinX) || (pInst->posCurr.x > winMaxX) || (pInst->posCurr.y < winMinY) || (pInst->posCurr.y > winMaxY))
			{
				pInst->flag = 0;


			}

			continue;
		}

		// �絼��Ŀ���ѻ��٣�����µ�������
		if (pInst->pObject->type == TYPE_MISSILE)
		{
			GameObj* pTarget;
			int j;
			float angle;
			Vector2D newv;

			if (missile_target == -1)
			{
				// ����ȷ����Ŀ��
				for (j = 0; j < GAME_OBJ_NUM_MAX; j++)
				{
					pTarget = sGameObjList + j;

					// �����ǻ������С���Ƕ���
					if ((pTarget->flag & FLAG_ACTIVE) == 0 || (pTarget->pObject->type != TYPE_ENEMY1&&pTarget->pObject->type != TYPE_ENEMY2&&pTarget->pObject->type != TYPE_ENEMY3))
						continue;

					missile_target = j;
					break;
				}

				// �ҵ�Ŀ��
				if (missile_target > -1)
				{
					// ȷ������
					Vector2DSub(&newv, &(pTarget->posCurr), &(pInst->posCurr));
					if (newv.x != 0)
						angle = atanf(newv.y / newv.x);
					else
						angle = 0;

					// �������ڵڶ���������
					if ((newv.x < 0 && newv.y < 0) || (newv.x < 0 && newv.y > 0))
						angle += PI;
					// �������ڵ�������
					if (newv.x > 0 && newv.y < 0)
						angle = 2 * PI + angle;

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
		//ship���ӵ���

		if (pInst->pObject->type == TYPE_BOSS)
		{
			for (j = 0; j < GAME_OBJ_NUM_MAX; j++)
			{

				pInstOther = sGameObjList + j;
				if ((pInstOther->flag & FLAG_ACTIVE) == 0)
					continue;
				if (pInstOther->pObject->type == TYPE_SHIP || pInstOther->pObject->type == TYPE_SHIP1)
				{
					if (StaticPointToStaticCircle(&(pInst->posCurr), &(pInstOther->posCurr), pInstOther->scale))
					{
						// �ɴ�����
						sShipLives--;
						if (sShipLives <= 0)
							// ���¿�ʼ�ؿ�
							Next = GS_Quit;
						else
						{
							// ����λ��
							pInstOther->posCurr.x = -250.0f;
							pInstOther->posCurr.y = 80.0f;
						}
					}
					continue;
				}
				if (pInstOther->pObject->type == TYPE_BULLET)
				{
					printf("%d\n", bossHealth);
					if (StaticCircleToStaticCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
					{
						pInstOther->flag = 0;
						bossHealth--;
						printf("%d\n", bossHealth);
						if (bossHealth == 0)
						{
							Next = GS_L0;
						}
					}
					continue;
				}
			}
			}
			
		if (pInst->pObject->type == TYPE_BULLET1)
		{
			for (j = 0; j < GAME_OBJ_NUM_MAX; j++)
			{
				pInstOther = sGameObjList + j;


				// �����ǻ�����С��������
				if ((pInstOther->flag & FLAG_ACTIVE) == 0 || (pInstOther->pObject->type == TYPE_ENEMY1)|| (pInstOther->pObject->type == TYPE_ENEMY2)|| (pInstOther->pObject->type == TYPE_ENEMY3))
					continue;

				// asteroid vs. ship
				if (pInstOther->pObject->type == TYPE_SHIP || pInstOther->pObject->type == TYPE_SHIP1)
				{
					// ��ײ���
					if (StaticPointToStaticCircle(&(pInst->posCurr), &(pInstOther->posCurr), pInstOther->scale))
					{
						// �ɴ�����
						sShipLives --;
						if (sShipLives <= 0)
							// ���¿�ʼ�ؿ�
							Next = GS_Quit;
						else
						{
							// ����λ��
							pInstOther->posCurr.x = -250.0f;
							pInstOther->posCurr.y = 80.0f;
						}
					}
				}
			}
		}
		// asteroid �� ship / bullet / missile ����ײ���
		if ((pInst->pObject->type == TYPE_ENEMY1) || (pInst->pObject->type == TYPE_ENEMY2) || (pInst->pObject->type == TYPE_ENEMY3))
		{
			//printf("%d\n", pInst->pObject->type);
			for (j = 0; j < GAME_OBJ_NUM_MAX; j++)
			{
				pInstOther = sGameObjList + j;

				// �����ǻ�����С��������
				if ((pInstOther->flag & FLAG_ACTIVE) == 0 || (pInstOther->pObject->type == TYPE_ENEMY1) ||
					(pInstOther->pObject->type == TYPE_ENEMY2) || (pInstOther->pObject->type == TYPE_ENEMY3))
					continue;
				if (pInstOther->pObject->type == TYPE_BULLET)
				{
					// ������ײ�������߶�����
					if (StaticCircleToStaticCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
					{
						pInstOther->flag = 0;
						pInst->flag = 0;
						sScore += 100;
						flag_enemy--;
					}
					continue;
				}
				// asteroid vs. ship
				if ((pInstOther->pObject->type == TYPE_SHIP) || (pInstOther->pObject->type == TYPE_SHIP1))
				{
					// ��ײ���
					if (StaticCircleToStaticCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
					{
						// �ɴ�����
						sShipLives -= 1;

						if (sShipLives <= 0)
							// ���¿�ʼ�ؿ�
							Next = GS_Quit;
						else
						{
							// ����λ��
							pInstOther->posCurr.x = -250.0f;
							pInstOther->posCurr.y = 20.0f;
						}
					}
					continue;
				}
				// asteroid vs. bullet
		
				// asteroid vs. missile
				if (pInstOther->pObject->type == TYPE_MISSILE)
				{
					// collision detection
					if (StaticCircleToStaticCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
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

				// �����ǻ�����С��������
				if ((pInstOther->flag & FLAG_ACTIVE) == 0 )
					continue;

				// SKILL vs. SHIP
				if (pInstOther->pObject->type == TYPE_SKILL)
				{
					// ��ײ���
					if (StaticCircleToStaticCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
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
	AEGfxTextureSet(NULL, 0, 0);
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
		// ���û���ģʽ(Color or texture)
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
		// �������в���(Color blend, textures, etc..)
		AEGfxSetBlendMode(AE_GFX_RM_COLOR);
		// ���ö����2D�任����ʹ�ú�����AEGfxSetTransform
		AEGfxSetTransform(pInst->transform.m);
		// ���Ƶ�ǰ����ʹ�ú�����AEGfxMeshDraw
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}

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
	Vector2D zero = { 0.0f, 0.0f };

	AE_ASSERT_PARM(type < sGameObjBaseNum);

	// ����Ϸ�����б���û�ù���λ��
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// �ҷǻ�����λ��
		if (pInst->flag == 0)
		{
			// �ҵ���
			pInst->pObject = sGameObjBaseList + type;
			pInst->flag = FLAG_ACTIVE;
			pInst->scale = scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->velCurr = pVel ? *pVel : zero;
			pInst->dirCurr = dir;

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
