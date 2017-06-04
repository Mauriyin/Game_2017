#include "Control.h"

/*
  匀速运动：当前位置向量、速度向量
*/
void Uniform_Motion(Vector2D* posCurr, Vector2D speed) {

	// 位置更新
	posCurr->x += speed.x * 0.95f;
	posCurr->y += speed.y * 0.95f;

}