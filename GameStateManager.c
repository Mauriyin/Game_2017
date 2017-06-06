/* Project:		GSMWin
   File Name:	GameStateManager.c
   Author:		yh
   Date:		
   Purpose:		GSM  */

#include "GameStateManager.h"
#include "GameStateList.h"
#include "System.h"
#include "Level1.h"
#include "Level2.h"
#include "menu.h"
#include "Help.h"
#include "End.h"
#include "Win.h"
#include "Select.h"



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

// GSM初始化
void GSM_Initialize(int level)
{
	// 初始化状态指示器
	Current = Previous = Next = level;
}

// GSM更新
void GSM_Update(int level)
{
	// Function Pointer Point to functions
	switch(level)
	{
	case GS_L0:
	{
		pLoad = Load0;
		pIni = Ini0;
		pUpdate = Update0;
		pDraw = Draw0;
		pFree = Free0;
		pUnload = Unload0;
		break;
	}
		case GS_L1:
			{
				pLoad = Load1;
				pIni = Ini1;
				pUpdate = Update1;
				pDraw = Draw1;
				pFree = Free1;
				pUnload = Unload1;
				break;
			}
			
			case GS_L2:
			{
				pLoad = Load2;
				pIni = Ini2;
				pUpdate = Update2;
				pDraw = Draw2;
				pFree = Free2;
				pUnload = Unload2;
				break;
			}
			case GS_Help:
			{
				pLoad = Load6;
				pIni = Ini6;
				pUpdate = Update6;
				pDraw = Draw6;
				pFree = Free6;
				pUnload = Unload6;
				break;
			}
			case GS_End:
			{
				pLoad = Load7;
				pIni = Ini7;
				pUpdate = Update7;
				pDraw = Draw7;
				pFree = Free7;
				pUnload = Unload7;
				break;
			}
			case GS_Win:
			{
				pLoad = Load8;
				pIni = Ini8;
				pUpdate = Update8;
				pDraw = Draw8;
				pFree = Free8;
				pUnload = Unload8;
				break;
			}
			case GS_Select:
			{
				pLoad = Load9;
				pIni = Ini9;
				pUpdate = Update9;
				pDraw = Draw9;
				pFree = Free9;
				pUnload = Unload9;
				break;
			}
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

