#pragma once
#include "GameObject.h"
#include "GameController.h"
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
	GameController paddle1_ctrl;

	// <�p�h��2>
	GameObject paddle2;
	GameController paddle2_ctrl;

	// <���_>
	GameScore score;
} GameScene;

