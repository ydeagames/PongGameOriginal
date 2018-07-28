#include "GameObjects.h"
#include "GameMain.h"
#include "GameUtils.h"
#include <math.h>

// �萔�̒�` ==============================================================

// <�{�[��>
#define BALL_SIZE 8

// <�p�h��>
#define PADDLE_WIDTH  8
#define PADDLE_HEIGHT 28

// �֐��̐錾 ==============================================================

float GameObject_Paddle_GetBallVelX(GameObject* paddle, GameObject* ball);
float GameObject_Paddle_GetBallVelY(GameObject* paddle, GameObject* ball);

// �֐��̒�` ==============================================================

// <<�{�[���I�u�W�F�N�g>> ----------------------------------------------

// <�{�[���I�u�W�F�N�g�쐬>
GameObject GameObject_Ball_Create(void)
{
	return GameObject_Create(Vec2_Create(), Vec2_Create(), Vec2_Create(BALL_SIZE, BALL_SIZE));
}

// <�{�[���I�u�W�F�N�g���WX�f�t�H���g>
void GameObject_Ball_SetPosXDefault(GameObject* obj, GameObject* field)
{
	obj->pos.x = field->pos.x;
}

// <�{�[���I�u�W�F�N�g���WY�f�t�H���g>
void GameObject_Ball_SetPosYDefault(GameObject* obj, GameObject* field)
{
	obj->pos.y = field->pos.y;
}

// <�{�[���I�u�W�F�N�g�ړ�X�f�t�H���g>
void GameObject_Ball_SetVelXDefault(GameObject* obj)
{
	obj->vel.x = BALL_VEL_X_MIN;
}

// <�{�[���I�u�W�F�N�g�ړ�Y�f�t�H���g>
void GameObject_Ball_SetVelYDefault(GameObject* obj)
{
	obj->vel.y = -BALL_VEL_Y;
}

// <<�p�h���I�u�W�F�N�g>> ----------------------------------------------

// <�p�h���I�u�W�F�N�g�쐬>
GameObject GameObject_Paddle_Create(void)
{
	return GameObject_Create(Vec2_Create(), Vec2_Create(), Vec2_Create(PADDLE_WIDTH, PADDLE_HEIGHT));
}

// <�p�h���I�u�W�F�N�g���WY�f�t�H���g>
void GameObject_Paddle_SetPosYDefault(GameObject* obj, GameObject* field)
{
	obj->pos.y = GameObject_OffsetY(obj, TOP, GameObject_GetY(field, BOTTOM, -16), 0);
}

// <�p�h���I�u�W�F�N�g�{�[���Փˏ���>
BOOL GameObject_Paddle_CollisionBall(GameObject* paddle, GameObject* ball)
{
	if (GameObject_IsHit(ball, paddle))
	{
		ball->vel.x *= -1;
		ball->vel.x = GameObject_Paddle_GetBallVelX(paddle, ball);

		ball->vel.y = GameObject_Paddle_GetBallVelY(paddle, ball);

		if (ball->vel.x < 0)
			ball->pos.x = GameObject_OffsetX(ball, LEFT, GameObject_GetX(paddle, LEFT));
		else
			ball->pos.x = GameObject_OffsetX(ball, RIGHT, GameObject_GetX(paddle, RIGHT));

		return TRUE;
	}

	return FALSE;
}

// <�p�h���̑��x����{�[��X���x�����߂�>
float GameObject_Paddle_GetBallVelX(GameObject* paddle, GameObject* ball)
{
	float ball_vel_diff_x, ball_vel_new_x;

	ball_vel_diff_x = paddle->vel.y / PADDLE_VEL * (BALL_VEL_X_MAX - BALL_VEL_X_MIN);
	if (ball_vel_diff_x < 0)
		ball_vel_diff_x *= -1;
	ball_vel_new_x = ball_vel_diff_x + BALL_VEL_X_MIN;
	if (ball->vel.x < 0)
		ball_vel_new_x *= -1;

	return ball_vel_new_x;
}

// <�p�h���ɂ��������ʒu����{�[��Y���x�����߂�>
float GameObject_Paddle_GetBallVelY(GameObject* paddle, GameObject* ball)
{
	float range_top = GameObject_OffsetY(ball, BOTTOM, GameObject_GetY(paddle, TOP));
	float range_bottom = GameObject_OffsetY(ball, TOP, GameObject_GetY(paddle, BOTTOM));
	float range_height = range_bottom - range_top;

	return ((((ball->pos.y - range_top) / range_height) * 2 - 1)*BALL_VEL_Y);
}
