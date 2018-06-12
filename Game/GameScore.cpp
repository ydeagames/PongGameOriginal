#include "GameScore.h"

// 関数の定義 ==============================================================

// <<スコア>> ----------------------------------------------------

// <スコア作成>
GameScore GameScore_Create(void)
{
	return { 0, 0 };
}

// <スコア追加>
void GameScore_Add(GameScore* score, ObjectSide side)
{
	switch (side)
	{
	case LEFT:
		score->score2++;
		break;
	case RIGHT:
		score->score1++;
		break;
	}
}

// <スコアリセット>
void GameScore_Clear(GameScore* score)
{
	score->score1 = 0;
	score->score2 = 0;
}

// <スコア終了判定>
BOOL GameScore_IsFinished(GameScore* score)
{
	return score->score1 >= SCORE_GOAL || score->score2 >= SCORE_GOAL;
}

// <スコア描画>
void GameScore_Render(GameScore* score, GameObject* field, HFNT font)
{
	// スコア描画

	// フォントを使用した文字の幅を取得
	int width_score1 = GetDrawFormatStringWidthToHandle(font, "%2d", score->score1);

	DrawFormatStringToHandle((int)(field->pos.x - 100 - width_score1), (int)(GameObject_GetY(field, TOP, 10)), COLOR_WHITE, font, "%2d", score->score1);
	DrawFormatStringToHandle((int)(field->pos.x + 100), (int)(GameObject_GetY(field, TOP, 10)), COLOR_WHITE, font, "%2d", score->score2);
}