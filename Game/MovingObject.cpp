#include "MovingObject.h"



MovingObject::MovingObject(Vec2 pos, Vec2 vel) : pos(pos), vel(vel)
{
}

void MovingObject::UpdatePosition()
{
	pos += vel;
}