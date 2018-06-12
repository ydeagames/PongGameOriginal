#pragma once
#include "GameObject.h"
#include "GameResource.h"

// 構造体の宣言 ============================================================

// <メニュー>
typedef struct
{
	GameObject* field;

	int selected;
} GameMenu;

// 関数の宣言 ==============================================================

// <<メニュー>> --------------------------------------------------------

// <メニュー作成>
GameMenu GameMenu_Create(GameObject* field);

// <メニュー更新>
void GameMenu_Update(GameMenu* menu);

// <メニュー描画>
void GameMenu_Render(GameMenu* menu, GameResource* res);
