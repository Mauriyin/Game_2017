/* Project:		GSMPre
File Name:	help.c
Author:dhr
Date:
Purpose:		   */

#include "Help.h"
#include "GameStateList.h"
#include "System.h"
#include <stdio.h>
#include "AEEngine.h"
#include "windef.h"
#include "windows.h"

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
static float obj3X, obj3Y;		// ����4��λ��
static AEGfxVertexList*	pMesh3;
static AEGfxVertexList*	pMesh4;	// ����4������(mesh)ģ��
static AEGfxTexture *pTex3;     // �������������
static AEGfxTexture *pTex4;     // ��ť���������
								//------------------------------------------------------------------------------
								// Private Function Declarations:
								//------------------------------------------------------------------------------

								//------------------------------------------------------------------------------
								// Public Functions:
								//------------------------------------------------------------------------------

void Load7(void)
{
	// ��������4
	AEGfxMeshStart();
	AEGfxTriAdd(
		-600.0f, -300.0f, 0x00FF00FF, 0.0f, 1.0f,
		600.0f, -300.0f, 0x00FFFF00, 1.0f, 1.0f,
		-600.0f, 300.0f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		600.0f, -300.0f, 0x00FFFFFF, 1.0f, 1.0f,
		600.0f, 300.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-600.0f, 300.0f, 0x00FFFFFF, 0.0f, 0.0f);
	pMesh3 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh3, "Failed to create mesh 4!!");
	// ��������
	pTex3 = AEGfxTextureLoad("Gameover.jpg");
	AEGfxMeshStart();
	AEGfxTriAdd(
		-50.0f, -20.0f, 0x00FF00FF, 0.0f, 1.0f,
		50.0f, -20.0f, 0x00FFFF00, 1.0f, 1.0f,
		-50.0f, 20.0f, 0x00F00FFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		50.0f, -20.0f, 0x00FFFFFF, 1.0f, 1.0f,
		50.0f, 20.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-50.0f, 20.0f, 0x00FFFFFF, 0.0f, 0.0f);
	pMesh4 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh4, "Failed to create mesh 3!!");

}

void Ini7(void)
{
	// Ϊ��ʼ��������׼��
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

}

void Update7(void)
{
	unsigned long i = 1;
	//POINT point;
	// ״̬�л�


	if (AEInputCheckTriggered('T'))
	{
		Next = GS_L0;
		return;
	}

}

void Draw7(void)
{
	// Drawing object 4
	// Set position for object 4
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);   // �����������û���ģʽΪ����
	AEGfxSetPosition(0.0f, 0.0f);  //ƫ�ƴ������ĵ�λ��
								   // Set texture for object 4
	AEGfxTextureSet(pTex3, 0.0f, 0.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(pMesh3, AE_GFX_MDM_TRIANGLES);
	AEGfxSetPosition(20.0f, -250.0f);
	pTex4 = AEGfxTextureLoad("Return.png");
	AEGfxTextureSet(pTex4, 0.0f, 0.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	AEGfxMeshDraw(pMesh4, AE_GFX_MDM_TRIANGLES);
}

void Free7(void)
{
}

void Unload7(void)
{
	// ж�ض�����Դ	
	AEGfxMeshFree(pMesh3);
	AEGfxMeshFree(pMesh4);
	AEGfxTextureUnload(pTex3);
	AEGfxTextureUnload(pTex4);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------