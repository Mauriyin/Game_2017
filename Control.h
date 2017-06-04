/* Project:
   File Name:	Control1.h
   Author:		阴浩
   Date:
   Purpose:		控制文件：封装一些必要的操作  */

#ifndef _Control
#define _Control

#include "Vector2d.h"

void Uniform_Motion(Vector2D* posCurr, Vector2D speed);
void Accu_Motion(Vector2D* posCurr, Vector2D speed, Vector2D ad);
#endif // !_Control

