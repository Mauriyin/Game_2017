/* Project:		GSMWin
   File Name:	Level1.h
   Author:		刘芳
   Date:		
   Purpose:		关卡1的属性和函数声明*/

#ifndef _Level1
#define _Level1

//------------------------------------------------------------------------------
// Typedef:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------
enum TYPE
{
	// 游戏对象类型列表
	TYPE_SHIP = 0,
	TYPE_SHIP1,
	TYPE_BULLET,
	TYPE_ASTEROID,
	TYPE_MISSILE,
	TYPE_SKILL,
	TYPE_NUM
};
//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// 6个状态函数
void Load1(void);
void Ini1(void);
void Update1(void);
void Draw1(void);
void Free1(void);
void Unload1(void);

#endif