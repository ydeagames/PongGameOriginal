#include "GameObject.h"
#include "GameMain.h"

// 定数の定義 ==============================================================

// <ボール>
#define BALL_SIZE 8

// <パドル>
#define PADDLE_WIDTH  8
#define PADDLE_HEIGHT 28

// 関数の宣言 ==============================================================

float GameObject_Paddle_GetBallVelX(GameObject* paddle, GameObject* ball);
float GameObject_Paddle_GetBallVelY(GameObject* paddle, GameObject* ball);

// 関数の定義 ==============================================================

// <<オブジェクト>> ----------------------------------------------------

// <オブジェクト作成>
GameObject GameObject_Create(Vec2 pos, Vec2 vel, Vec2 size)
{
	return { pos, vel, size };
}

// <オブジェクト座標更新>
void GameObject_UpdatePosition(GameObject* obj)
{
	obj->pos.x += obj->vel.x;
	obj->pos.y += obj->vel.y;
}

// <オブジェクトXオフセット>
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

// <オブジェクトXオフセット>
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

// <オブジェクトX位置セット>
void GameObject_SetX(GameObject* obj, ObjectSide side, float pos, float padding)
{
	obj->pos.x = GameObject_OffsetX(obj, side, pos, padding);
}

// <オブジェクトY位置セット>
void GameObject_SetY(GameObject* obj, ObjectSide side, float pos, float padding)
{
	obj->pos.y = GameObject_OffsetY(obj, side, pos, padding);
}

// <オブジェクトX位置ゲット>
float GameObject_GetX(GameObject* obj, ObjectSide side, float padding)
{
	return GameObject_OffsetX(obj, side, obj->pos.x, padding);
}

// <オブジェクトY位置ゲット>
float GameObject_GetY(GameObject* obj, ObjectSide side, float padding)
{
	return GameObject_OffsetY(obj, side, obj->pos.y, padding);
}

// <オブジェクト当たり判定>
BOOL GameObject_IsHit(GameObject* obj1, GameObject* obj2)
{
	return (
		GameObject_GetX(obj2, LEFT) < GameObject_GetX(obj1, RIGHT) &&
		GameObject_GetX(obj1, LEFT) < GameObject_GetX(obj2, RIGHT) &&
		GameObject_GetY(obj2, TOP) < GameObject_GetY(obj1, BOTTOM) &&
		GameObject_GetY(obj1, TOP) < GameObject_GetY(obj2, BOTTOM)
		);
}

// <オブジェクト描画>
void GameObject_Render(GameObject* obj, unsigned int color)
{
	DrawBoxAA(GameObject_GetX(obj, LEFT), GameObject_GetY(obj, TOP), GameObject_GetX(obj, RIGHT), GameObject_GetY(obj, BOTTOM), color, TRUE);
}

// <<ボールオブジェクト>> ----------------------------------------------

// <ボールオブジェクト作成>
GameObject GameObject_Ball_Create(void)
{
	return GameObject_Create(Vec2_Create(), Vec2_Create(), Vec2_Create(BALL_SIZE, BALL_SIZE));
}

// <ボールオブジェクト座標Xデフォルト>
void GameObject_Ball_SetPosXDefault(GameObject* obj, GameObject* field)
{
	obj->pos.x = field->pos.x;
}

// <ボールオブジェクト座標Yデフォルト>
void GameObject_Ball_SetPosYDefault(GameObject* obj, GameObject* field)
{
	obj->pos.y = field->pos.y;
}

// <ボールオブジェクト移動Xデフォルト>
void GameObject_Ball_SetVelXDefault(GameObject* obj)
{
	obj->vel.x = BALL_VEL_X_MIN;
}

// <ボールオブジェクト移動Yデフォルト>
void GameObject_Ball_SetVelYDefault(GameObject* obj)
{
	obj->vel.y = -BALL_VEL_Y;
}

// <<パドルオブジェクト>> ----------------------------------------------

// <パドルオブジェクト作成>
GameObject GameObject_Paddle_Create(void)
{
	return GameObject_Create(Vec2_Create(), Vec2_Create(), Vec2_Create(PADDLE_WIDTH, PADDLE_HEIGHT));
}

// <パドルオブジェクト座標Yデフォルト>
void GameObject_Paddle_SetPosYDefault(GameObject* obj)
{
	GameObject_SetY(obj, TOP, SCREEN_BOTTOM, 16);
}

// <パドルオブジェクトボール衝突処理>
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

// <パドルの速度からボールX速度を求める>
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

// <パドルにあたった位置からボールY速度を求める>
float GameObject_Paddle_GetBallVelY(GameObject* paddle, GameObject* ball)
{
	float range_top = GameObject_OffsetY(ball, BOTTOM, GameObject_GetY(paddle, TOP));
	float range_bottom = GameObject_OffsetY(ball, TOP, GameObject_GetY(paddle, BOTTOM));
	float range_height = range_bottom - range_top;

	return ((((ball->pos.y - range_top) / range_height) * 2 - 1)*BALL_VEL_Y);
}

// <<フィールドオブジェクト>> ------------------------------------------

// <ボールオブジェクト作成>
GameObject GameObject_Field_Create(void)
{
	return GameObject_Create(Vec2_Create(SCREEN_CENTER_X, SCREEN_CENTER_Y), Vec2_Create(), Vec2_Create(SCREEN_WIDTH, SCREEN_HEIGHT));
}

// <フィールド上下衝突処理>
ObjectSide GameObject_Field_CollisionVertical(GameObject* field, GameObject* obj, BOOL flag_with_bounce)
{
	// ヒットサイド
	ObjectSide side_hit = NONE;

	// ボール・上下壁当たり判定
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

		// 壁にめり込まないように調整
		obj->pos.y = ClampF(obj->pos.y, padding_top, padding_bottom);
	}

	return side_hit;
}

// <フィールド左右衝突処理>
ObjectSide GameObject_Field_CollisionHorizontal(GameObject* field, GameObject* obj, BOOL flag_with_bounce)
{
	// ヒットサイド
	ObjectSide side_hit = NONE;

	// ボール・左右壁当たり判定
	{
		float padding_left = GameObject_OffsetX(obj, RIGHT, GameObject_GetX(field, LEFT));
		float padding_right = GameObject_OffsetX(obj, LEFT, GameObject_GetX(field, RIGHT));

		if (flag_with_bounce)
		{
			// 画面外に出たときの処理
			if (obj->pos.x < padding_left || padding_right <= obj->pos.x)
			{
				obj->vel.x *= -1.f;
			}
		}

		if (obj->pos.x < padding_left)
			side_hit = LEFT;
		else if (padding_right <= obj->pos.x)
			side_hit = RIGHT;

		// 壁にめり込まないように調整
		obj->pos.x = ClampF(obj->pos.x, padding_left, padding_right);
	}

	return side_hit;
}

// <フィールド描画>
void GameObject_Field_Render(GameObject* field)
{
	// コート表示
	DrawDashedLineAA(field->pos.x, GameObject_GetY(field, TOP), field->pos.x, GameObject_GetY(field, BOTTOM), COLOR_WHITE, 8, 2);
}

