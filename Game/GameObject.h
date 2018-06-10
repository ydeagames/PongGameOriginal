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

// 関数の宣言 ==============================================================

// <<オブジェクト>> ----------------------------------------------------

// <オブジェクト作成>
GameObject GameObject_Create(Vec2 pos, Vec2 vel, Vec2 size);

// <オブジェクト左位置セット>
void GameObject_SetLeft(GameObject* obj, float left, float padding = 0.f);

// <オブジェクト左位置セット>
void GameObject_SetRight(GameObject* obj, float right, float padding = 0.f);

// <オブジェクト左位置セット>
void GameObject_SetTop(GameObject* obj, float top, float padding = 0.f);

// <オブジェクト左位置セット>
void GameObject_SetBottom(GameObject* obj, float bottom, float padding = 0.f);

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