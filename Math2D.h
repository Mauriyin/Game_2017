#ifndef MATH2D_H
#define MATH2D_H

#include "Vector2D.h"

/*
检测点pP是否与圆（圆心pCenter，半径pRadius）发生碰撞
*/
int StaticPointToStaticCircle(Vector2D *pP, Vector2D *pCenter, float Radius);

/*
检测点pPos是否与矩形（中心pRect，宽度Width，高度Height）发生碰撞
*/
int StaticPointToStaticRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height);

/*
检测两个圆是否发生碰撞
Circle0: 圆心pCenter0，半径pRadius0
Circle1: 圆心pCenter1，半径pRadius1
*/
int StaticCircleToStaticCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1);

/*
检测两个矩形是否发生碰撞
Rectangle0: 中心pRect0，宽度Width0，高度Height0
Rectangle1: 中心pRect1，宽度Width1，高度Height1
*/
int StaticRectToStaticRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1);


#endif