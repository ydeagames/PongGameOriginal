#pragma once
#include "GameObject.h"
#include "GameScore.h"

// 列挙型の定義 ============================================================

// <シーン状態> --------------------------------------------------------
enum GameState
{
	// デモ
	STATE_DEMO,

	// サーブ
	STATE_SERVE,

	// プレイ
	STATE_PLAY
};

// <パケット> -----------------------------------------------------------
enum GamePacket
{
	PACKET_NONE = 0,
	PACKET_START,
	PACKET_SCORE,
	PACKET_END,
	PACKET_SERVE
};

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

	// <パケット>
	GamePacket packet;

	// <サーブ待機>
	int counter;
} GameScene;

