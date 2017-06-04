/*  Project:	GProj_Asteroid
    File:		main.c
	Author:		刘芳
	Date:		2015年创建，使用最简单的退出控制流
	            2016年融合了GSM，可以完成关卡切换
	            2017年4月新增Level2：纹理动画效果展示
	Purpose:	在项目GSMProj的基础上，实现Level1的内容
				展示游戏对象设计和一个非常简单的对象管理器
				目的是呈现一个2D俯瞰式射击游戏的实现

				太空飞船：	由玩家控制
							左右箭头键负责逆时针和顺时针旋转
							上下箭头键负责前进和后退，运动为具有摩擦的加速运动
							空格键发射子弹，子弹为匀速运动
							M键发射导弹
				导弹：需要自动寻找目标（小行星）
				匀速运动通过碰撞击毁小行星

				3个小行星：分别从窗口视野外的3个不同位置生成
				每个小行星的运动：由生成位置向屏幕中央做直线匀速运动

				飞船或小行星运动至屏幕外则应在对向的屏幕边出现，如从
				窗口上方消失，则应出现在窗口下方对应位置
				————————————————————————————
				按键'2'切换到关卡2（键‘1’切换回来）
				关卡2展示了纹理贴图后的动画效果
				————————————————————————————
	注意:		本项目的Input、math、vec、matrix采用AE提供的函数
	            以上函数应由团队自己完成编码   */

#include "System.h"
#include <Windows.h>
#include "GameStateList.h"
#include "GameStateManager.h"
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

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// System Initialize
	System_Initialize(hInstance, nCmdShow);

	// GSM Initialize
	GSM_Initialize(GS_L1);

	// Game Flow
	while(Current != GS_Quit)
	{
		if(Current == GS_Restart)
		{
			Current = Previous;
			Next = Previous;
		}
		else
		{
			// GSM Update
			GSM_Update(Current);
			pLoad();
		}

		pIni();

		// Game Loop
		while(Current == Next)
		{
			AESysFrameStart();		// 帧开始：封装了PeekMessage
			AEInputUpdate();		// Alpha Engine处理输入
			pUpdate();
			pDraw();
			AESysFrameEnd();		// 帧结束
		}

		pFree();

		if(Next != GS_Restart)
			pUnload();

		Previous = Current;
		Current = Next;
	}

	// System Exit
	System_Exit();

	return 0;
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
