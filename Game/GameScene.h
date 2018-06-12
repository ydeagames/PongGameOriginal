#pragma once
#include "GameObject.h"
#include "GameScore.h"

// �񋓌^�̒�` ============================================================

// <�V�[�����> --------------------------------------------------------
enum GameState
{
	// �f��
	STATE_DEMO,

	// �T�[�u
	STATE_SERVE,

	// �v���C
	STATE_PLAY
};

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

