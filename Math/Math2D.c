#include "Math2D.h"
#include "stdio.h"

/*
����pP�Ƿ���Բ��Բ��pCenter���뾶pRadius��������ײ
*/
int StaticPointToStaticCircle(Vector2D *pP, Vector2D *pCenter, float Radius)
{
	float dis = Vector2DDistance(pP, pCenter);
	if (dis <= Radius)
		return 1;
	return 0;
}


/*
����pPos�Ƿ�����Σ�����pRect�����Width���߶�Height��������ײ
*/
int StaticPointToStaticRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height)
{
	float x = fabs(pPos->x - (pRect->x));
	float y = fabs(pPos->y - (pRect->y));
	if (x <= Width / 2 && y <= Height / 2)
		return 1;
	return 0;
}

/*
Circle0: Բ��pCenter0���뾶pRadius0
Circle1: Բ��pCenter1���뾶pRadius1
*/
int StaticCircleToStaticCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1)
{
	float dis = Vector2DDistance(pCenter0, pCenter1);
	if (dis <= Radius0 + Radius1)
		return 1;
	return 0;
}

/*
������������Ƿ�����ײ
Rectangle0: ����pRect0�����Width0���߶�Height0
Rectangle1: ����pRect1�����Width1���߶�Height1
*/
int StaticRectToStaticRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1)
{
	float x = fabs(pRect0->x - (pRect1->x));
	float y = fabs(pRect0->y - (pRect1->y));
	if (x <= fabs(Width0+Width1) / 2 && y <= fabs(Height0+Height1) / 2)
		return 1;
	return 0;
}
