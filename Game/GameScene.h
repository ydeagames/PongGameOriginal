#pragma once
#include "GameObject.h"
#include "GameScore.h"

// �\���̂̐錾 ============================================================

// <���j���[>
typedef struct
{
	// <�V�[�����>
	int game_state;

	// <�t�B�[���h>
	GameObject field;

	// <�{�[��>
	GameObject ball;

	// <�p�h��1>
	GameObject paddle1;

	// <�p�h��2>
	GameObject paddle2;

	// <���_>
	GameScore score;

	// <�T�[�u�ҋ@>
	int counter;
} GameScene;

