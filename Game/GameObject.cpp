#include "GameObject.h"
#include "GameMain.h"

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

// <<�I�u�W�F�N�g>> ----------------------------------------------------

// <�I�u�W�F�N�g�쐬>
GameObject GameObject_Create(Vec2 pos, Vec2 vel, Vec2 size)
{
	return { pos, vel, size };
}

// <�I�u�W�F�N�g���W�X�V>
void GameObject_UpdatePosition(GameObject* obj)
{
	obj->pos.x += obj->vel.x;
	obj->pos.y += obj->vel.y;
}

// <�I�u�W�F�N�gX�I�t�Z�b�g>
float GameObject_OffsetX(GameObject* obj, ObjectSide side, float pos, float padding)
{
	float offset = 0;
	switch (side)
	{
	case LEFT:
		offset = -(padding + obj->size.x / 2.f);
		break;
	case RIGHT:
		offset = (padding + obj->size.x / 2.f);
		break;
	}
	return pos + offset;
}

// <�I�u�W�F�N�gX�I�t�Z�b�g>
float GameObject_OffsetY(GameObject* obj, ObjectSide side, float pos, float padding)
{
	float offset = 0;
	switch (side)
	{
	case TOP:
		offset = -(padding + obj->size.y / 2.f);
		break;
	case BOTTOM:
		offset = (padding + obj->size.y / 2.f);
		break;
	}
	return pos + offset;
}

// <�I�u�W�F�N�gX�ʒu�Z�b�g>
void GameObject_SetX(GameObject* obj, ObjectSide side, float pos, float padding)
{
	obj->pos.x = GameObject_OffsetX(obj, side, pos, padding);
}

// <�I�u�W�F�N�gY�ʒu�Z�b�g>
void GameObject_SetY(GameObject* obj, ObjectSide side, float pos, float padding)
{
	obj->pos.y = GameObject_OffsetY(obj, side, pos, padding);
}

// <�I�u�W�F�N�gX�ʒu�Q�b�g>
float GameObject_GetX(GameObject* obj, ObjectSide side, float padding)
{
	return GameObject_OffsetX(obj, side, obj->pos.x, padding);
}

// <�I�u�W�F�N�gY�ʒu�Q�b�g>
float GameObject_GetY(GameObject* obj, ObjectSide side, float padding)
{
	return GameObject_OffsetY(obj, side, obj->pos.y, padding);
}

// <�I�u�W�F�N�g�����蔻��>
BOOL GameObject_IsHit(GameObject* obj1, GameObject* obj2)
{
	return (
		GameObject_GetX(obj2, LEFT) < GameObject_GetX(obj1, RIGHT) &&
		GameObject_GetX(obj1, LEFT) < GameObject_GetX(obj2, RIGHT) &&
		GameObject_GetY(obj2, TOP) < GameObject_GetY(obj1, BOTTOM) &&
		GameObject_GetY(obj1, TOP) < GameObject_GetY(obj2, BOTTOM)
		);
}

// <�I�u�W�F�N�g�`��>
void GameObject_Render(GameObject* obj, unsigned int color)
{
	DrawBoxAA(GameObject_GetX(obj, LEFT), GameObject_GetY(obj, TOP), GameObject_GetX(obj, RIGHT), GameObject_GetY(obj, BOTTOM), color, TRUE);
}

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
void GameObject_Paddle_SetPosYDefault(GameObject* obj)
{
	GameObject_SetY(obj, TOP, SCREEN_BOTTOM, 16);
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

// <<�t�B�[���h�I�u�W�F�N�g>> ------------------------------------------

// <�{�[���I�u�W�F�N�g�쐬>
GameObject GameObject_Field_Create(void)
{
	return GameObject_Create(Vec2_Create(SCREEN_CENTER_X, SCREEN_CENTER_Y), Vec2_Create(), Vec2_Create(SCREEN_WIDTH, SCREEN_HEIGHT));
}

// <�t�B�[���h�㉺�Փˏ���>
ObjectSide GameObject_Field_CollisionVertical(GameObject* field, GameObject* obj, BOOL flag_with_bounce)
{
	// �q�b�g�T�C�h
	ObjectSide side_hit = NONE;

	// �{�[���E�㉺�Ǔ����蔻��
	{
		float padding_top = GameObject_OffsetY(obj, BOTTOM, GameObject_GetY(field, TOP));
		float padding_bottom = GameObject_OffsetY(obj, TOP, GameObject_GetY(field, BOTTOM));

		if (flag_with_bounce)
		{
			if (obj->pos.y < padding_top || padding_bottom <= obj->pos.y)
			{
				obj->vel.y *= -1.f;
			}
		}

		if (obj->pos.y < padding_top)
			side_hit = TOP;
		else if (padding_bottom <= obj->pos.y)
			side_hit = BOTTOM;

		// �ǂɂ߂荞�܂Ȃ��悤�ɒ���
		obj->pos.y = ClampF(obj->pos.y, padding_top, padding_bottom);
	}

	return side_hit;
}

// <�t�B�[���h���E�Փˏ���>
ObjectSide GameObject_Field_CollisionHorizontal(GameObject* field, GameObject* obj, BOOL flag_with_bounce)
{
	// �q�b�g�T�C�h
	ObjectSide side_hit = NONE;

	// �{�[���E���E�Ǔ����蔻��
	{
		float padding_left = GameObject_OffsetX(obj, RIGHT, GameObject_GetX(field, LEFT));
		float padding_right = GameObject_OffsetX(obj, LEFT, GameObject_GetX(field, RIGHT));

		if (flag_with_bounce)
		{
			// ��ʊO�ɏo���Ƃ��̏���
			if (obj->pos.x < padding_left || padding_right <= obj->pos.x)
			{
				obj->vel.x *= -1.f;
			}
		}

		if (obj->pos.x < padding_left)
			side_hit = LEFT;
		else if (padding_right <= obj->pos.x)
			side_hit = RIGHT;

		// �ǂɂ߂荞�܂Ȃ��悤�ɒ���
		obj->pos.x = ClampF(obj->pos.x, padding_left, padding_right);
	}

	return side_hit;
}

// <�t�B�[���h�`��>
void GameObject_Field_Render(GameObject* field)
{
	// �R�[�g�\��
	DrawDashedLineAA(field->pos.x, GameObject_GetY(field, TOP), field->pos.x, GameObject_GetY(field, BOTTOM), COLOR_WHITE, 8, 2);
}

