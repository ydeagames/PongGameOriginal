#include "PaddleObject.h"



PaddleObject::PaddleObject(Vec2 pos, Vec2 vel, Vec2 size) : BoxMovingObject::BoxMovingObject(pos, vel, size)
{
}

bool PaddleObject::Collision(BallObject &ball)
{
	// �{�[���E�p�h�������蔻��
	if (IsHit(ball))
	{
		ball.vel.x *= -1;

		ball.vel = { GetCollisionVelX(ball), GetCollisionVelY(ball) };

		// �߂荞�ݑ΍􏈗�
		if (ball.vel.x < 0)
			ball.pos.x = pos.x - PADDLE_WIDTH / 2 - ball.BALL_SIZE / 2;
		else
			ball.pos.x = pos.x + PADDLE_WIDTH / 2 + ball.BALL_SIZE / 2;

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
//! @brief �p�h���̑��x����{�[��X���x�����߂�
//!
//! @param[ball] �{�[��
//!
//! @return �V�����{�[����X���x
//----------------------------------------------------------------------
float PaddleObject::GetCollisionVelX(const BallObject &ball) const
{
	float ball_vel_diff_x, ball_vel_new_x;

	ball_vel_diff_x = vel.y / PADDLE_VEL * (ball.BALL_VEL_X_MAX - ball.BALL_VEL_X_MIN);
	if (ball_vel_diff_x < 0)
		ball_vel_diff_x *= -1;
	ball_vel_new_x = ball_vel_diff_x + ball.BALL_VEL_X_MIN;
	if (ball.vel.x < 0)
		ball_vel_new_x *= -1;

	return ball_vel_new_x;
}

//----------------------------------------------------------------------
//! @brief �p�h���ɂ��������ʒu����{�[��Y���x�����߂�
//!
//! @param[ball] �{�[��
//!
//! @return �V�����{�[����Y���x
//----------------------------------------------------------------------
float PaddleObject::GetCollisionVelY(const BallObject &ball) const
{
	float range_top = pos.y - (size.y / 2 - ball.size.y / 2);
	float range_bottom = pos.y + (size.y / 2 - ball.size.y / 2);
	float range_height = range_bottom - range_top;

	return ((((ball.pos.y - range_top) / range_height) * 2 - 1)*ball.BALL_VEL_Y);
}

