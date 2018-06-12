#pragma once
#include "GameObject.h"
#include "GameController.h"
#include "GameScore.h"

// 構造体の宣言 ============================================================

// <メニュー>
typedef struct
{
	// <シーン状態>
	int game_state;

	// <フィールド>
	GameObject field;

	// <ボール>
	GameObject ball;

	// <パドル1>
	GameObject paddle1;
	GameController paddle1_ctrl;

	// <パドル2>
	GameObject paddle2;
	GameController paddle2_ctrl;

	// <得点>
	GameScore score;
} GameScene;

