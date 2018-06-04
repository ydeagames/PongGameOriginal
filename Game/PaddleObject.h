#pragma once
#include "BallObject.h"
struct PaddleObject : BoxMovingObject
{
	static constexpr float PADDLE_VEL = 8.f;

	static constexpr float PADDLE_WIDTH = 8.f;
	static constexpr float PADDLE_HEIGHT = 28.f;

	PaddleObject(Vec2 pos = {}, Vec2 vel = {}, Vec2 size = { PADDLE_WIDTH, PADDLE_HEIGHT });

	bool Collision(BallObject &ball);

private:
	float PaddleObject::GetCollisionVelX(const BallObject &ball) const;
	float PaddleObject::GetCollisionVelY(const BallObject &ball) const;
};

