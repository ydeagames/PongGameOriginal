#pragma once
#include "Vec2.h"

// 構造体の宣言 ============================================================

// <ゲームオブジェクト>
typedef struct
{
	Vec2 pos;	// <位置>
	Vec2 vel;	// <速度>
	Vec2 size;	// <大きさ>
} GameObject;

// 定数の定義 ==============================================================

// <ボール>
#define BALL_VEL_X_MAX 5	// パドルが移動している状態でボールにあたった時の速度
#define BALL_VEL_X_MIN 3	// パドルが静止している状態でボールにあたった時の速度
#define BALL_VEL_Y 4.5f		// Yの最大速度

// <パドル>
#define PADDLE_VEL 8		// パドルの速度

// <ボール>
#define BALL_SIZE 8

// <パドル>
#define PADDLE_WIDTH  8
#define PADDLE_HEIGHT 28

// 関数の宣言 ==============================================================

// <<オブジェクト>> ----------------------------------------------------

// <オブジェクト作成>
GameObject GameObject_Create(Vec2 pos, Vec2 vel, Vec2 size);

// <オブジェクト座標更新>
void GameObject_UpdatePosition(GameObject* obj);

// <オブジェクト左位置セット>
void GameObject_SetLeft(GameObject* obj, float left, float padding = 0.f);

// <オブジェクト右位置セット>
void GameObject_SetRight(GameObject* obj, float right, float padding = 0.f);

// <オブジェクト上位置セット>
void GameObject_SetTop(GameObject* obj, float top, float padding = 0.f);

// <オブジェクト下位置セット>
void GameObject_SetBottom(GameObject* obj, float bottom, float padding = 0.f);

// <オブジェクト左位置ゲット>
float GameObject_GetLeft(GameObject* obj, float padding = 0.f);

// <オブジェクト右位置ゲット>
float GameObject_GetRight(GameObject* obj, float padding = 0.f);

// <オブジェクト上位置ゲット>
float GameObject_GetTop(GameObject* obj, float padding = 0.f);

// <オブジェクト下位置ゲット>
float GameObject_GetBottom(GameObject* obj, float padding = 0.f);

// <オブジェクト当たり判定>
BOOL GameObject_IsHit(GameObject* obj1, GameObject* obj2);

// <<ボールオブジェクト>> ----------------------------------------------

// <ボールオブジェクト作成>
GameObject GameObject_Ball_Create(void);

// <ボールオブジェクト座標Xデフォルト>
void GameObject_Ball_SetPosXDefault(GameObject* obj);

// <ボールオブジェクト座標Yデフォルト>
void GameObject_Ball_SetPosYDefault(GameObject* obj);

// <ボールオブジェクト移動デフォルト>
void GameObject_Ball_SetVelDefault(GameObject* obj);

// <<パドルオブジェクト>> ----------------------------------------------

// <パドルオブジェクト作成>
GameObject GameObject_Paddle_Create(void);

// <パドルオブジェクト座標Yデフォルト>
void GameObject_Paddle_SetPosYDefault(GameObject* obj);