#include "Control.h"

/*
  �����˶�����ǰλ���������ٶ�����
*/
void Uniform_Motion(Vector2D* posCurr, Vector2D speed) {

	// λ�ø���
	posCurr->x += speed.x * 0.95f;
	posCurr->y += speed.y * 0.95f;

}