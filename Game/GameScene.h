#pragma once
#include "GameObject.h"
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

	// <パドル2>
	GameObject paddle2;

	// <得点>
	GameScore score;

	// <サーブ待機>
	int counter;
} GameScene;

