#include "BoxMovingObject.h"



bool BoxMovingObject::IsHit(BoxMovingObject &other)
{
	float b_x1 = other.pos.x - other.size.x / 2;
	float b_y1 = other.pos.y - other.size.y / 2;
	float b_x2 = other.pos.x + other.size.x / 2;
	float b_y2 = other.pos.y + other.size.y / 2;

	float p_x1 = pos.x - size.x / 2;
	float p_y1 = pos.y - size.y / 2;
	float p_x2 = pos.x + size.x / 2;
	float p_y2 = pos.y + size.y / 2;

	return (p_x1 < b_x2&&b_x1 < p_x2&&p_y1 < b_y2&&b_y1 < p_y2);
}
