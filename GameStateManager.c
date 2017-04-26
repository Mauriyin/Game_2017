/* Project:		StarWars
   File Name:	GameStateManager.c
   Author:		����
   Date:		4.26 v1.0 ����ȷ��
   Purpose:		GSM  
*/

#include "GameStateManager.h"
#include "GameStateList.h"
#include "System.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

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

// GSM��ʼ��
void GSM_Initialize(int level)
{
	// ��ʼ��״ָ̬ʾ��
	Current = Previous = Next = level;
}

// GSM����
void GSM_Update(int level)
{
	// Function Pointer Point to functions
	switch(level)
	{
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
			case GS_L3:
			{
				pLoad = Load3;
				pIni = Ini3;
				pUpdate = Update3;
				pDraw = Draw3;
				pFree = Free3;
				pUnload = Unload3;
				break;
			}
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

