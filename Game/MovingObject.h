#pragma once

#include "Vec2.h"

struct MovingObject
{
	Vec2 pos;
	Vec2 vel;

	MovingObject(Vec2 pos = {}, Vec2 vel = {});

	void UpdatePosition();
};

