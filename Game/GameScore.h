#pragma once
#include "GameObject.h"

// 構造体の宣言 ============================================================

// <ゲームスコア>
typedef struct
{
	int score1;	// 1Pのスコア
	int score2;	// 2Pのスコア

	GameObject* field;
} GameScore;

// 定数の定義 ==============================================================

// <得点>
#define SCORE_GOAL 11		// 最大スコア

// 関数の宣言 ==============================================================

// <<スコア>> ----------------------------------------------------

// <スコア作成>
GameScore GameScore_Create(GameObject* field);

// <スコア追加>
void GameScore_Add(GameScore* score, ObjectSide side);

// <スコアリセット>
void GameScore_Clear(GameScore* score);

// <スコア終了判定>
BOOL GameScore_IsFinished(GameScore* score);

// <スコア描画>
void GameScore_Render(GameScore* score, HFNT font);
