/* Project:
   File Name:	Control1.h
   Author:		����
   Date:
   Purpose:		�����ļ�����װһЩ��Ҫ�Ĳ���  */

#ifndef _Control
#define _Control

#include "Vector2d.h"

void Uniform_Motion(Vector2D* posCurr, Vector2D speed);
void Accu_Motion(Vector2D* posCurr, Vector2D speed, Vector2D ad);
#endif // !_Control

