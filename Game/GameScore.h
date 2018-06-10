#pragma once
#include "GameObject.h"

// 構造体の宣言 ============================================================

// <ゲームオブジェクト>
typedef struct
{
	int score1;	// 1Pのスコア
	int score2;	// 2Pのスコア

	GameObject* field;
	HFNT font;
} GameScore;

// 定数の定義 ==============================================================

// <得点> --------------------------------------------------------------
#define SCORE_GOAL 11		// 最大スコア

// <フォント> ----------------------------------------------------------
#define FONT_SIZE_SCORE 100

// 関数の宣言 ==============================================================

// <<スコア>> ----------------------------------------------------

// <スコア作成>
GameScore GameScore_Create(GameObject* field, HFNT font);

// <スコア追加>
void GameScore_Add(GameScore* score, ObjectSide side);

// <スコアリセット>
void GameScore_Clear(GameScore* score);

// <スコア終了判定>
BOOL GameScore_IsFinished(GameScore* score);

// <スコア描画>
void GameScore_Render(GameScore* score);
