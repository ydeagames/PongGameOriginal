#pragma once
#include "MovingObject.h"
struct BoxMovingObject : MovingObject
{
	Vec2 size;

	bool IsHit(BoxMovingObject &other);
};

