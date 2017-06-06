/* Project:		GSMPre
   File Name:	Level2.c
   Author:		
   Date:		
   Purpose:		�ؿ�2 չʾ��Ϸ�����֡����Ч��
                ʹ�õ�ͼƬ��playerrun.png���ö�ͼ��8֡   */

#include "Level2.h"
#include "GameStateList.h"
#include "System.h"
#include <stdio.h>
#include "AEEngine.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static float obj2X, obj2Y;		// ����2��λ��
static AEGfxVertexList*	pMesh2;	// ����2������(mesh)ģ��
static AEGfxTexture *pTex1;		// ����2������
static AEGfxTexture *pTex2;     // ����2������pTex1��80%�������ڶ���չʾ
static int AnimationController = 8; // ���Ʋ���֡
static int AnimationCurFrame = 1;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void Load2(void)
{
	// ��������2
	AEGfxMeshStart();
	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f, 
		30.0f,  -30.0f, 0x00FFFF00, 0.125f,1.0f,
		-30.0f,  30.0f, 0x00F00FFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 0.125f, 1.0f, 
		30.0f,  30.0f, 0x00FFFFFF, 0.125f, 0.0f,
		-30.0f,  30.0f, 0x00FFFFFF, 0.0f, 0.0f);
	pMesh2 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh2, "Failed to create mesh 2!!");

	// ��������
	//pTex1 = AEGfxTextureLoad("PlanetTexture.png");
	pTex1 = AEGfxTextureLoad("PlayerRun.png");

}

void Ini2(void)
{
	// Ϊ��ʼ��������׼��
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

}

void Update2(void)
{
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
	if(AEInputCheckTriggered('1'))
	{
		Next = GS_L1;
		return;
	}
}

void Draw2(void)
{
	// Drawing object 2
	// Set position for object 2
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);   // �����������û���ģʽΪ����
	AEGfxSetPosition(0.0f, 0.0f);
	// Set texture for object 2
	if (AnimationCurFrame < AnimationController)
	{
		AEGfxTextureSet(pTex1, AnimationCurFrame*0.125f, 0.0f);
		AnimationCurFrame += 1;		
	}
	else
	{
		AEGfxTextureSet(pTex1, 0.0f, 0.0f); // ����1������ƫ����(x,y)
		AnimationCurFrame = 1;
	}
	AEGfxSetTransparency(1.0f);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);
}

void Free2(void)
{
}

void Unload2(void)
{
	// ж�ض�����Դ	
	AEGfxMeshFree(pMesh2);
	AEGfxTextureUnload(pTex1);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
