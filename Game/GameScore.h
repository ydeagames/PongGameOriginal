#pragma once
#include "GameObject.h"

// �\���̂̐錾 ============================================================

// <�Q�[���I�u�W�F�N�g>
typedef struct
{
	int score1;	// 1P�̃X�R�A
	int score2;	// 2P�̃X�R�A

	GameObject* field;
	HFNT font;
} GameScore;

// �萔�̒�` ==============================================================

// <���_> --------------------------------------------------------------
#define SCORE_GOAL 11		// �ő�X�R�A

// <�t�H���g> ----------------------------------------------------------
#define FONT_SIZE_SCORE 100

// �֐��̐錾 ==============================================================

// <<�X�R�A>> ----------------------------------------------------

// <�X�R�A�쐬>
GameScore GameScore_Create(GameObject* field, HFNT font);

// <�X�R�A�ǉ�>
void GameScore_Add(GameScore* score, ObjectSide side);

// <�X�R�A���Z�b�g>
void GameScore_Clear(GameScore* score);

// <�X�R�A�I������>
BOOL GameScore_IsFinished(GameScore* score);

// <�X�R�A�`��>
void GameScore_Render(GameScore* score);
