#pragma once
#include "BoxMovingObject.h"
struct BallObject : BoxMovingObject
{
	static constexpr float BALL_VEL_X_MIN = 3.f;
	static constexpr float BALL_VEL_X_MAX = 5.f;
	static constexpr float BALL_VEL_Y = 4.5f;

	static constexpr float BALL_SIZE = 8.f;

	BallObject(Vec2 pos = {}, Vec2 vel = {}, Vec2 size = { BALL_SIZE, BALL_SIZE });
};

