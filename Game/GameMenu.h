#pragma once
#include "GameScene.h"
#include "GameResource.h"
#include "GameControllers.h"

// 構造体の宣言 ============================================================

// <メニュー>
typedef struct
{
	GameScene* scene;
	GameControllers* controllers;

	int selected;
	BOOL listening;
	BOOL connected;
} GameMenu;

// 関数の宣言 ==============================================================

// <<メニュー>> --------------------------------------------------------

// <メニュー作成>
GameMenu GameMenu_Create(GameScene* scene, GameControllers* controllers);

// <メニュー更新>
void GameMenu_Update(GameMenu* menu);

// <メニュー決定が押されたとき>
BOOL GameMenu_OnPressed(GameMenu* menu);

// <メニュー描画>
void GameMenu_Render(GameMenu* menu, GameResource* res);
