#pragma once
#include "GameScene.h"
#include "GameResource.h"

// 構造体の宣言 ============================================================

// <メニュー>
typedef struct
{
	GameScene* scene;

	int selected;
} GameMenu;

// 関数の宣言 ==============================================================

// <<メニュー>> --------------------------------------------------------

// <メニュー作成>
GameMenu GameMenu_Create(GameScene* scene);

// <メニュー更新>
void GameMenu_Update(GameMenu* menu);

// <メニュー決定が押されたとき>
BOOL GameMenu_OnPressed(GameMenu* menu);

// <メニュー描画>
void GameMenu_Render(GameMenu* menu, GameResource* res);
