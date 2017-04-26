/* Project:		StarWars
   File Name:	System.h
   Author:		阴浩
   Date:		4.26  v1.0 引擎确认

   Purpose:		系统管理器
*/

#ifndef _System
#define _System

#include <Windows.h>
#include <stdio.h>

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
FILE *fp;   // Output.txt

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// 系统初始化
extern int System_Initialize(HINSTANCE hInstance, int nCmdShow);

// 系统退出
extern void System_Exit(void);

#endif