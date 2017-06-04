#include "Vector2D.h"

#define EPSILON 0.0001

// ---------------------------------------------------------------------------
/*
��2ά����pResult����������ֵ����Ϊ0
*/
void Vector2DZero(Vector2D *pResult)
{
	pResult->x = 0;
	pResult->y = 0;
}

// ---------------------------------------------------------------------------
/*
�ֱ�2ά����pResult������ֵ����Ϊx��y
*/
void Vector2DSet(Vector2D *pResult, float x, float y)
{
	pResult->x = x;
	pResult->y = y;
}

// ---------------------------------------------------------------------------
/*
pResult����ΪpVec0�ķ�����
*/
void Vector2DNeg(Vector2D *pResult, Vector2D *pVec0)
{
	pResult->x = -pVec0->x;
	pResult->y = -pVec0->y;
}

// ---------------------------------------------------------------------------
/*
pResult = pVec0 + pVec1
*/
void Vector2DAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
	pResult->x = pVec0->x + pVec1->x;
	pResult->y = pVec0->y + pVec1->y;
}

// ---------------------------------------------------------------------------
/*
pResult = pVec0 - pVec1
*/
void Vector2DSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
	pResult->x = pVec0->x - pVec1->x;
	pResult->y = pVec0->y - pVec1->y;
}

// ---------------------------------------------------------------------------
/*
pResult����Ϊ����pVec0�ĵ�λ����
*/
void Vector2DNormalize(Vector2D *pResult, Vector2D *pVec0)
{
	if (0.0 == pResult->x && 0.0 == pResult->y) 
	{
		pResult->x = 0;
		pResult->y = 0;
	}
	double module_squre = (pVec0->x * pVec0->x) + (pVec0->y * pVec0->y);
	double module = sqrt(module_squre);
	pResult->x = pVec0->x / (float)module;
	pResult->y = pVec0->y / (float)module;

}

// ---------------------------------------------------------------------------
/*
pResult����Ϊ����pVec0����c���Ľ��
*/
void Vector2DScale(Vector2D *pResult, Vector2D *pVec0, float c)
{
	pResult->x = pVec0->x * c;
	pResult->y = pVec0->y * c;
}

// ---------------------------------------------------------------------------
/*
pResult = ����pVec0����c�� + pVec1
*/
void Vector2DScaleAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
	pResult->x = pVec0->x * c + pVec1->x;
	pResult->y = pVec0->y * c + pVec1->y;
}

// ---------------------------------------------------------------------------
/*
pResult = ����pVec0����c�� - pVec1
*/
void Vector2DScaleSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
	pResult->x = pVec0->x * c - pVec1->x;
	pResult->y = pVec0->y * c - pVec1->y;
}

// ---------------------------------------------------------------------------
/*
��������pVec0�ĳ���
*/
float Vector2DLength(Vector2D *pVec0)
{
	double module_squre = (pVec0->x * pVec0->x) + (pVec0->y * pVec0->y);
	double module = sqrt(module_squre);
	return (float)module;
}

// ---------------------------------------------------------------------------
/*
��������pVec0���ȵ�ƽ��������ƽ��������
*/
float Vector2DSquareLength(Vector2D *pVec0)
{
	double module_squre = (pVec0->x * pVec0->x) + (pVec0->y * pVec0->y);
	return (float)module_squre;
}

// ---------------------------------------------------------------------------
/*
pVec0��pVec1��2ά�ռ�������㣬������������ľ���
*/
float Vector2DDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	float x, y;
	x = pVec0->x - pVec1->x;
	y = pVec0->y - pVec1->y;
	double module_squre = (x * x) + (y * y);
	double module = sqrt(module_squre);
	return (float)module;
}

// ---------------------------------------------------------------------------
/*
pVec0��pVec1��2ά�ռ�������㣬������������ľ����ƽ��������ƽ��������
*/
float Vector2DSquareDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	float x, y;
	x = pVec0->x - pVec1->x;
	y = pVec0->y - pVec1->y;
	float module_squre = (x * x) + (y * y);
	return module_squre;
}

// ---------------------------------------------------------------------------
/*
����pVec0��pVec1�ĵ��
*/
float Vector2DDotProduct(Vector2D *pVec0, Vector2D *pVec1)
{
	float x, y;
	x = pVec0->x * pVec1->x;
	y = pVec0->y * pVec1->y;
	return (x + y);
}

// ---------------------------------------------------------------------------
/*
�����굽ֱ�������µ�λ������ת����angleΪ��������30��
*/
void Vector2DFromAngleDeg(Vector2D *pResult, float angle)
{
	double angle_radian = (double)angle * 3.14159265 / 180;
	pResult->x = (float)cos(angle_radian);
	pResult->y = (float)sin(angle_radian);
}

// ---------------------------------------------------------------------------
/*
�����굽ֱ�������µ�λ������ת����angleΪ����
*/
void Vector2DFromAngleRad(Vector2D *pResult, float angle)
{
	pResult->x = (float)cos((double)angle);
	pResult->y = (float)sin((double)angle);
}

// ---------------------------------------------------------------------------
