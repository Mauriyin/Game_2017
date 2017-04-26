/* Project:		StarWars
   File Name:	GameStateList.h
   Author:		阴浩
   Date:		4.26  v1.0 引擎确认

   Purpose:		游戏状态声明，主要为Game Flow的状态切换所用 
*/

#ifndef _GameStateList
#define _GameStateList

//------------------------------------------------------------------------------
// Typedef:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------
enum
{
	GS_L1,              // Level1
	GS_L2,              // Level2
	GS_L3,				// Level3				
	GS_NUM,             // Total Number
	GS_Restart,			// Restart
	GS_Quit				// Quit
};

int Current, Previous, Next;  // 状态指示器

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

#endif