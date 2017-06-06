/* Project:		GSMPre
File Name:	menu.c
Author:dhr
Date:
Purpose:		   */

#include "Menu.h"
#include "GameStateList.h"
#include "System.h"
#include <stdio.h>
#include "AEEngine.h"
#include "windef.h"
#include "windows.h"
#include "end.h"

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
static AEGfxVertexList*	pMesh1;
static AEGfxVertexList*	pMesh2;	// ����2������(mesh)ģ��
static AEGfxTexture *pTex1;     // �������������
static AEGfxTexture *pTex2;     // ��ť���������
								//------------------------------------------------------------------------------
								// Private Function Declarations:
								//------------------------------------------------------------------------------

								//------------------------------------------------------------------------------
								// Public Functions:
								//------------------------------------------------------------------------------

void Load0(void)
{
	// ��������2
	AEGfxMeshStart();
	AEGfxTriAdd(
		-500.0f, -300.0f, 0x00FF00FF, 0.0f, 1.0f,
		500.0f, -300.0f, 0x00FFFF00, 1.0f, 1.0f,
		-500.0f, 300.0f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		500.0f, -300.0f, 0x00FFFFFF, 1.0f, 1.0f,
		500.0f, 300.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-500.0f, 300.0f, 0x00FFFFFF, 0.0f, 0.0f);
	
	pMesh1 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh1, "Failed to create mesh 2!!");

	// ��������
	pTex1 = AEGfxTextureLoad("Background_Image.jpg");

	AEGfxMeshStart();
	AEGfxTriAdd(
		-50.0f, -20.0f, 0x00FF00FF, 0.0f, 1.0f,
		50.0f, -20.0f, 0x00FFFF00, 1.0f, 1.0f,
		-50.0f, 20.0f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		50.0f, -20.0f, 0x00FFFFFF, 1.0f, 1.0f,
		50.0f, 20.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-50.0f, 20.0f, 0x00FFFFFF, 0.0f, 0.0f);
	
	pMesh2 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh2, "Failed to create mesh 1!!");

}

void Ini0(void)
{
	// Ϊ��ʼ��������׼��
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

}

void Update0(void)
{
	unsigned long i = 1;
	//POINT point;
	// ״̬�л�
	if (AEInputCheckTriggered('S'))
	{
		Next = GS_L1;
		return;
	}
	if (AEInputCheckTriggered('Q'))
	{
		Next = GS_Quit;
		return;
	}
	if (AEInputCheckTriggered('W'))
	{
		Next = GS_Help;
		return;
	}
	if (AEInputCheckTriggered('R'))
	{
		MessageBoxA(NULL, "By ����˵�Ķ���", "������Ա", MB_OK);
	}
}

void Draw0(void)
{
	// Drawing object 2
	// Set position for object 2
	unsigned long i;
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);   // �����������û���ģʽΪ����
	AEGfxSetPosition(0.0f, 0.0f);  //ƫ�ƴ������ĵ�λ��
								   // Set texture for object 2
	AEGfxTextureSet(pTex1, 0.0f, 0.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);
	for (i = 0; i < 3; i++)
	{
		AEGfxSetPosition(20.0f, (i)*-80.0f);
		switch (i)
		{
		case 0:
			pTex2 = AEGfxTextureLoad("Start.png");
			break;
		case 1:
			pTex2 = AEGfxTextureLoad("End.png");
			break;
		case 2:
			pTex2 = AEGfxTextureLoad("About.png");
			break;
		default:
			break;
		}
		AEGfxTextureSet(pTex2, 0.0f, 0.0f);
		AEGfxSetTransparency(1.0f);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);
	}
}

void Free0(void)
{
}

void Unload0(void)
{
	// ж�ض�����Դ	
	AEGfxMeshFree(pMesh1);
	AEGfxMeshFree(pMesh2);
	AEGfxTextureUnload(pTex1);
	AEGfxTextureUnload(pTex2);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
