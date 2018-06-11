#include "GameObject.h"
#include "GameMain.h"

// 定数の定義 ==============================================================

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

// <オブジェクト左位置セット>
void GameObject_SetLeft(GameObject* obj, float left, float padding)
{
	obj->pos.x = left + (padding + obj->size.x / 2.f);
}

// <オブジェクト右位置セット>
void GameObject_SetRight(GameObject* obj, float right, float padding)
{
	obj->pos.x = right - (padding + obj->size.x / 2.f);
}

// <オブジェクト上位置セット>
void GameObject_SetTop(GameObject* obj, float top, float padding)
{
	obj->pos.y = top + (padding + obj->size.y / 2.f);
}

// <オブジェクト下位置セット>
void GameObject_SetBottom(GameObject* obj, float bottom, float padding)
{
	obj->pos.y = bottom - (padding + obj->size.y / 2.f);
}

// <オブジェクト左位置ゲット>
float GameObject_GetLeft(GameObject* obj, float padding)
{
	return obj->pos.x - (padding + obj->size.x / 2.f);
}

// <オブジェクト右位置ゲット>
float GameObject_GetRight(GameObject* obj, float padding)
{
	return obj->pos.x + (padding + obj->size.x / 2.f);
}

// <オブジェクト上位置ゲット>
float GameObject_GetTop(GameObject* obj, float padding)
{
	return obj->pos.y - (padding + obj->size.y / 2.f);
}

// <オブジェクト下位置ゲット>
float GameObject_GetBottom(GameObject* obj, float padding)
{
	return obj->pos.y + (padding + obj->size.y / 2.f);
}

// <オブジェクト当たり判定>
BOOL GameObject_IsHit(GameObject* obj1, GameObject* obj2)
{
	return (
		GameObject_GetLeft(obj2) < GameObject_GetRight(obj1) &&
		GameObject_GetLeft(obj1) < GameObject_GetRight(obj2) &&
		GameObject_GetTop(obj2) < GameObject_GetBottom(obj1) &&
		GameObject_GetTop(obj1) < GameObject_GetBottom(obj2)
	);
}

// <<ボールオブジェクト>> ----------------------------------------------

// <ボールオブジェクト作成>
GameObject GameObject_Ball_Create(void)
{
	return GameObject_Create({ 0, 0 }, { 0, 0 }, { BALL_SIZE, BALL_SIZE });
}

// <ボールオブジェクト移動デフォルト>
void GameObject_Ball_SetVelDefault(GameObject* obj)
{
	obj->vel = { BALL_VEL_X_MIN, -BALL_VEL_Y };
}

// <ボールオブジェクト座標Xデフォルト>
void GameObject_Ball_SetPosXDefault(GameObject* obj)
{
	obj->pos.x = SCREEN_CENTER_X;
}

// <ボールオブジェクト座標Yデフォルト>
void GameObject_Ball_SetPosYDefault(GameObject* obj)
{
	obj->pos.y = SCREEN_CENTER_Y;
}

// <<パドルオブジェクト>> ----------------------------------------------

// <パドルオブジェクト作成>
GameObject GameObject_Paddle_Create(void)
{
	return GameObject_Create({ 0, 0 }, { 0, 0 }, { PADDLE_WIDTH, PADDLE_HEIGHT });
}

// <パドルオブジェクト座標Yデフォルト>
void GameObject_Paddle_SetPosYDefault(GameObject* obj)
{
	GameObject_SetBottom(obj, SCREEN_BOTTOM, 16);
}
