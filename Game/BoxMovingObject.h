#pragma once
#include "MovingObject.h"
struct BoxMovingObject : MovingObject
{
	Vec2 size;

	BoxMovingObject(Vec2 pos = {}, Vec2 vel = {}, Vec2 size = {});

	bool IsHit(BoxMovingObject &other);
};

