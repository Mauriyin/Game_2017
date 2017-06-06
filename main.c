/*  Project:	GProj_Asteroid
File:		main.c
Author:		
Date:		
   */

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
	GSM_Initialize(GS_L0);

	// Game Flow
	while (Current != GS_Quit)
	{
		if (Current == GS_Restart)
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
		while (Current == Next)
		{
			AESysFrameStart();		// 帧开始：封装了PeekMessage
			AEInputUpdate();		// Alpha Engine处理输入
			pUpdate();
			pDraw();
			AESysFrameEnd();		// 帧结束
		}

		pFree();

		if (Next != GS_Restart)
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
