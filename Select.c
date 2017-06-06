/* Project:		GSMPre
File Name:	help.c
Author:dhr
Date:
Purpose:		   */

#include "Select.h"
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

unsigned int choice;
//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static float obj3X, obj3Y;		// 对象4的位置
static AEGfxVertexList*	pMesh3;
static AEGfxVertexList*	pMesh4;	// 对象4的网格(mesh)模型
static AEGfxTexture *pTex3;     // 背景对象的纹理
static AEGfxTexture *pTex4;     // 按钮对象的纹理
								//------------------------------------------------------------------------------
								// Private Function Declarations:
								//------------------------------------------------------------------------------

								//------------------------------------------------------------------------------
								// Public Functions:
								//------------------------------------------------------------------------------

void Load9(void)
{
	// 创建对象4
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
	// 载入纹理
	pTex3 = AEGfxTextureLoad("Select.jpg");
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

void Ini9(void)
{
	// 为开始画对象做准备
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

}

void Update9(void)
{
	unsigned long i = 1;
	//POINT point;
	// 状态切换
	if (AEInputCheckTriggered('1'))
	{
		choice = 1;
		Next = GS_L1;
	}
	if (AEInputCheckTriggered('2'))
	{
		choice = 2;
		Next = GS_L1;
	}
	if (AEInputCheckTriggered('3'))
	{
		choice = 3;
		Next = GS_L1;
	}

}

void Draw9(void)
{
	unsigned long i;
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);   // 必须最先设置绘制模式为纹理
	AEGfxSetPosition(0.0f, 0.0f);  //偏移窗口中心的位置
								   // Set texture for object 2
	AEGfxTextureSet(pTex3, 0.0f, 0.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(pMesh3, AE_GFX_MDM_TRIANGLES);
	for (i = 0; i < 3; i++)
	{
		AEGfxSetPosition(20.0f, (i)*-80.0f);
		switch (i)
		{
		case 0:
			pTex4 = AEGfxTextureLoad("Easy.png");
			break;
		case 1:
			pTex4 = AEGfxTextureLoad("Normal.png");
			break;
		case 2:
			pTex4 = AEGfxTextureLoad("Difficult.png");
			break;
		default:
			break;
		}
		AEGfxTextureSet(pTex4, 0.0f, 0.0f);
		AEGfxSetTransparency(1.0f);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh4, AE_GFX_MDM_TRIANGLES);
	}
}
void Free9(void)
{
}

void Unload9(void)
{
	// 卸载对象资源	
	AEGfxMeshFree(pMesh3);
	AEGfxMeshFree(pMesh4);
	AEGfxTextureUnload(pTex3);
	AEGfxTextureUnload(pTex4);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
