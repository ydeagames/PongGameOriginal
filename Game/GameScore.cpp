#include "GameScore.h"
#include "GameMain.h"

// �֐��̒�` ==============================================================

// <<�X�R�A>> ----------------------------------------------------

// <�X�R�A�쐬>
GameScore GameScore_Create(GameObject* field, HFNT font)
{
	return { 0, 0, field, font };
}

// <�X�R�A�ǉ�>
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

// <�X�R�A���Z�b�g>
void GameScore_Clear(GameScore* score)
{
	score->score1 = 0;
	score->score2 = 0;
}

// <�X�R�A�I������>
BOOL GameScore_IsFinished(GameScore* score)
{
	return score->score1 >= SCORE_GOAL || score->score2 >= SCORE_GOAL;
}

// <�X�R�A�`��>
void GameScore_Render(GameScore* score)
{
	// �X�R�A�`��

	// �t�H���g���g�p���������̕����擾
	int width_score1 = GetDrawFormatStringWidthToHandle(score->font, "%2d", score->score1);

	DrawFormatStringToHandle((float)(score->field->pos.x - 100 - width_score1), (float)(GameObject_GetY(score->field, TOP, 10)), COLOR_WHITE, score->font, "%2d", score->score1);
	DrawFormatStringToHandle((float)(score->field->pos.x + 100), (float)(GameObject_GetY(score->field, TOP, 10)), COLOR_WHITE, score->font, "%2d", score->score2);
}