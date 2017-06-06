/* Project:		GSMPre
   File Name:	Level2.c
   Author:		
   Date:		
   Purpose:		关卡2 展示游戏对象多帧动画效果
                使用的图片是playerrun.png，该动图含8帧   */

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
static float obj2X, obj2Y;		// 对象2的位置
static AEGfxVertexList*	pMesh2;	// 对象2的网格(mesh)模型
static AEGfxTexture *pTex1;		// 对象2的纹理
static AEGfxTexture *pTex2;     // 对象2的纹理（pTex1的80%），用于动画展示
static int AnimationController = 8; // 控制播放帧
static int AnimationCurFrame = 1;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void Load2(void)
{
	// 创建对象2
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

	// 载入纹理
	//pTex1 = AEGfxTextureLoad("PlanetTexture.png");
	pTex1 = AEGfxTextureLoad("PlayerRun.png");

}

void Ini2(void)
{
	// 为开始画对象做准备
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

}

void Update2(void)
{
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
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);   // 必须最先设置绘制模式为纹理
	AEGfxSetPosition(0.0f, 0.0f);
	// Set texture for object 2
	if (AnimationCurFrame < AnimationController)
	{
		AEGfxTextureSet(pTex1, AnimationCurFrame*0.125f, 0.0f);
		AnimationCurFrame += 1;		
	}
	else
	{
		AEGfxTextureSet(pTex1, 0.0f, 0.0f); // 参数1：纹理，偏移量(x,y)
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
	// 卸载对象资源	
	AEGfxMeshFree(pMesh2);
	AEGfxTextureUnload(pTex1);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
