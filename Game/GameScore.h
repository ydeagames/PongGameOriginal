#pragma once
#include "GameObject.h"

// �\���̂̐錾 ============================================================

// <�Q�[���X�R�A>
typedef struct
{
	int score1;	// 1P�̃X�R�A
	int score2;	// 2P�̃X�R�A

	GameObject* field;
} GameScore;

// �萔�̒�` ==============================================================

// <���_>
#define SCORE_GOAL 11		// �ő�X�R�A

// �֐��̐錾 ==============================================================

// <<�X�R�A>> ----------------------------------------------------

// <�X�R�A�쐬>
GameScore GameScore_Create(GameObject* field);

// <�X�R�A�ǉ�>
void GameScore_Add(GameScore* score, ObjectSide side);

// <�X�R�A���Z�b�g>
void GameScore_Clear(GameScore* score);

// <�X�R�A�I������>
BOOL GameScore_IsFinished(GameScore* score);

// <�X�R�A�`��>
void GameScore_Render(GameScore* score, HFNT font);
