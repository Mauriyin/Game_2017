#include "Matrix2D.h"


/*
将pResult设置为单位矩阵
*/
void Matrix2DIdentity(Matrix2D *pResult)
{
	int i = 0, j = 0;
	for (i = 0; i<3; i++)
	{
		for (j = 0; j<3; j++)
		{
			pResult->m[i][j] = 0;
		}
	}
	pResult->m[0][0] = 1;
	pResult->m[1][1] = 1;
	pResult->m[2][2] = 1;
}

// ---------------------------------------------------------------------------

/*
计算pMtx的转置，并将结果放到pResult
*/
void Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx)
{
	int i = 0, j = 0;
	for (i = 0; i<3; i++)
	{
		for (j = 0; j<3; j++)
		{
			pResult->m[i][j] = pMtx->m[j][i];
		}
	}
}

// ---------------------------------------------------------------------------

/*
Result = Mtx0*Mtx1
*/
void Matrix2DConcat(Matrix2D *pResult, Matrix2D *pMtx0, Matrix2D *pMtx1)
{
	int i = 0, j = 0, k = 0;
	Matrix2D m;
	for (i = 0; i<3; i++)
	{
		for (j = 0; j<3; j++)
		{
			m.m[i][j] = 0;
		}
	}
	for (i = 0; i<3; i++)
	{
		for (j = 0; j<3; j++)
		{
			for (k = 0; k<3; k++)
			{
				m.m[i][j] += pMtx0->m[i][k] * pMtx1->m[k][j];
			}

		}
	}
	for (i = 0; i<3; i++)
	{
		for (j = 0; j<3; j++)
		{
			pResult->m[i][j] = m.m[i][j];
		}
	}
}

// ---------------------------------------------------------------------------

/*
将pResult设置为平移矩阵，平移位移为x和y
*/
void Matrix2DTranslate(Matrix2D *pResult, float x, float y)
{
	Matrix2DIdentity(pResult);
	pResult->m[0][2] = x;
	pResult->m[1][2] = y;
}

// ---------------------------------------------------------------------------

/*
将pResult设置为缩放矩阵，x和y分别为水平和垂直方向的缩放比例
*/
void Matrix2DScale(Matrix2D *pResult, float x, float y)
{
	Matrix2DIdentity(pResult);
	pResult->m[0][0] = x;
	pResult->m[1][1] = y;

}

// ---------------------------------------------------------------------------

/*
将pResult设置为旋转矩阵，旋转量为Angle，为度数
*/
void Matrix2DRotDeg(Matrix2D *pResult, float Angle)
{
	Matrix2DIdentity(pResult);
	float angle_radian = Angle * 3.141592 / 180;
	pResult->m[0][0] = cos(angle_radian);
	pResult->m[1][1] = cos(angle_radian);
	pResult->m[0][1] = -sin(angle_radian);
	pResult->m[1][0] = sin(angle_radian);
}

// ---------------------------------------------------------------------------

/*
将pResult设置为旋转矩阵，旋转量为Angle，为弧度
*/
void Matrix2DRotRad(Matrix2D *pResult, float Angle)
{
	Matrix2DIdentity(pResult);
	pResult->m[0][0] = cos(Angle);
	pResult->m[1][1] = cos(Angle);
	pResult->m[0][1] = -sin(Angle);
	pResult->m[1][0] = sin(Angle);
}

// ---------------------------------------------------------------------------

/*
Result = Mtx * Vec
*/
void Matrix2DMultVec(Vector2D *pResult, Matrix2D *pMtx, Vector2D *pVec)
{
	Vector2D vec;
	vec.x = pMtx->m[0][0] * pVec->x + pMtx->m[0][1] * pVec->y + pMtx->m[0][2];
	vec.y = pMtx->m[1][0] * pVec->x + pMtx->m[1][1] * pVec->y + pMtx->m[1][2];
	pResult->x = vec.x;
	pResult->y = vec.y;
}

// ---------------------------------------------------------------------------
