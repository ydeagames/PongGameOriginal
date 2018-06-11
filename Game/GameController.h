#pragma once
#include "GameObject.h"

// 構造体の宣言 ============================================================

// <ゲームコントローラー>
typedef struct tag_GameController GameController;
struct tag_GameController
{
	GameObject* object;
	void(*UpdateControl)(GameController*);

	// 継承ができないので仕方なくPlayer用の変数
	int player_key_up;
	int player_key_down;

	// 継承ができないので仕方なくBot用の変数
	Vec2 bot_target_pos;
	GameObject* bot_ball;
	GameObject* bot_enemy;
};

// 関数の宣言 ==============================================================

// <<コントローラー>> --------------------------------------------------

// <コントローラー作成>
GameController GameController_Create(GameObject* object, void(*ctrlFunc)(GameController*));

// <<プレイヤーコントローラー>> ----------------------------------------

// <プレイヤーコントローラー作成>
GameController GameController_Player_Create(GameObject* object, int key_up, int key_down);

// <<Botコントローラー>> -----------------------------------------------

// <Botコントローラー作成>
GameController GameController_Bot_Create(GameObject* object, GameObject* ball, GameObject* enemy);
