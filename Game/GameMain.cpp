//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.cpp
//!
//! @brief  Pong �m�[�}���ۑ�
//!
//! @date   2018/06/01
//!
//! @author GF1 26 �R�� ����
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "GameMain.h"
#include "GameObject.h"
#include "GameController.h"
#include "GameScore.h"
#include "GameResource.h"

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


// �萔�̒�` ==============================================================

// <�T�[�u�ҋ@> --------------------------------------------------------
#define SERVE_WAIT_TIME 2*60


// �O���[�o���ϐ��̐錾 ====================================================

// <�V�[�����> --------------------------------------------------------
int g_game_state;

// <�t�B�[���h> --------------------------------------------------------
GameObject g_field;

// <�{�[��> ------------------------------------------------------------
GameObject g_ball;

// <�p�h��1> -----------------------------------------------------------
GameObject g_paddle1;
GameController g_paddle1_ctrl;

// <�p�h��2> -----------------------------------------------------------
GameObject g_paddle2;
GameController g_paddle2_ctrl;

// <���\�[�X> ----------------------------------------------------------
GameResource g_resource;

// <���_> --------------------------------------------------------------
GameScore g_score;

// <�T�[�u�ҋ@> --------------------------------------------------------
int g_counter;


// �֐��̐錾 ==============================================================

// <�Q�[���̍X�V����:�V�[��> -------------------------------------------
void UpdateGameSceneDemo(void);
void UpdateGameSceneServe(void);
void UpdateGameScenePlay(void);

// <�Q�[���̕`�揈��> --------------------------------------------------
void RenderGameSceneDemo(void);
void RenderGameSceneServe(void);
void RenderGameScenePlay(void);

void UpdateGameScore(ObjectSide side);

// �֐��̒�` ==============================================================

//----------------------------------------------------------------------
//! @brief �Q�[���̏���������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void InitializeGame(void)
{
	// �V�[�����
	g_game_state = STATE_DEMO;

	// �t�B�[���h
	g_field = GameObject_Field_Create();

	// �{�[��
	g_ball = GameObject_Ball_Create();
	GameObject_Ball_SetPosXDefault(&g_ball, &g_field);
	GameObject_Ball_SetPosYDefault(&g_ball, &g_field);
	GameObject_Ball_SetVelXDefault(&g_ball);
	GameObject_Ball_SetVelYDefault(&g_ball);

	// �p�h��1
	g_paddle1 = GameObject_Paddle_Create();
	GameObject_SetX(&g_paddle1, RIGHT, SCREEN_LEFT, 64);
	GameObject_Paddle_SetPosYDefault(&g_paddle1);
	g_paddle1_ctrl = GameController_Bot_Create(&g_paddle1, &g_field, &g_ball, &g_paddle2);

	// �p�h��2
	g_paddle2 = GameObject_Paddle_Create();
	GameObject_SetX(&g_paddle2, LEFT, SCREEN_RIGHT, 64);
	GameObject_Paddle_SetPosYDefault(&g_paddle2);
	g_paddle2_ctrl = GameController_Player_Create(&g_paddle2, &g_field, &g_ball, &g_paddle1, PAD_INPUT_UP, PAD_INPUT_DOWN);

	// ���\�[�X
	g_resource = GameResource_Create();

	// ���_
	g_score = GameScore_Create(&g_field);

	// �T�[�u�ҋ@
	g_counter = 0;
}


//----------------------------------------------------------------------
//! @brief �Q�[���̍X�V����
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void UpdateGame(void)
{
	switch (g_game_state)
	{
	case STATE_DEMO:
		UpdateGameSceneDemo();
		break;
	case STATE_SERVE:
		UpdateGameSceneServe();
		break;
	case STATE_PLAY:
		UpdateGameScenePlay();
		break;
	}
}

// <�Q�[���̍X�V����:�V�[��:�f��> --------------------------------------
void UpdateGameSceneDemo(void)
{
	// �ҋ@&������
	{
		// ���͂��ꂽ��
		if (IsButtonDown(PAD_INPUT_10))
		{
			// �_�����Z�b�g
			GameScore_Clear(&g_score);

			// X���W����ʒ����֖߂�
			GameObject_Ball_SetPosXDefault(&g_ball, &g_field);

			// �p�h���������ʒu��
			GameObject_Paddle_SetPosYDefault(&g_paddle1);
			GameObject_Paddle_SetPosYDefault(&g_paddle2);

			// �V�[�����v���C�ɕύX
			g_game_state = STATE_PLAY;
		}
	}

	// ���W�X�V
	GameObject_UpdatePosition(&g_ball);

	// �����蔻��
	GameObject_Field_CollisionVertical(&g_field, &g_ball, TRUE);
	GameObject_Field_CollisionHorizontal(&g_field, &g_ball, TRUE);
}

// <�Q�[���̍X�V����:�V�[��:�T�[�u> ------------------------------------
void UpdateGameSceneServe(void)
{
	// �ҋ@&������
	g_counter++;

	// ���Ԍo�߂�
	if (g_counter >= SERVE_WAIT_TIME)
	{
		// X���W����ʒ����֖߂�
		GameObject_Ball_SetPosXDefault(&g_ball, &g_field);

		// �V�[�����v���C�ɕύX
		g_game_state = STATE_PLAY;

		g_counter = 0;
	}

	// ����
	GameController_Update(&g_paddle1_ctrl);
	GameController_Update(&g_paddle2_ctrl);

	// ���W�X�V
	GameObject_UpdatePosition(&g_ball);
	GameObject_UpdatePosition(&g_paddle1);
	GameObject_UpdatePosition(&g_paddle2);

	// �����蔻��
	GameObject_Field_CollisionVertical(&g_field, &g_ball, TRUE);
	GameObject_Paddle_CollisionBall(&g_paddle1, &g_ball);
	GameObject_Paddle_CollisionBall(&g_paddle2, &g_ball);
	GameObject_Field_CollisionVertical(&g_field, &g_paddle1, FALSE);
	GameObject_Field_CollisionVertical(&g_field, &g_paddle2, FALSE);
}

// <�Q�[���̍X�V����:�V�[��:�v���C> ------------------------------------
void UpdateGameScenePlay(void)
{
	// ����
	GameController_Update(&g_paddle1_ctrl);
	GameController_Update(&g_paddle2_ctrl);

	// ���W�X�V
	GameObject_UpdatePosition(&g_ball);
	GameObject_UpdatePosition(&g_paddle1);
	GameObject_UpdatePosition(&g_paddle2);

	// �����蔻��
	if (GameObject_Field_CollisionVertical(&g_field, &g_ball, TRUE))
		PlaySoundMem(g_resource.sound_se02, DX_PLAYTYPE_BACK);
	{
		ObjectSide side = GameObject_Field_CollisionHorizontal(&g_field, &g_ball, FALSE);
		if (side)
		{
			UpdateGameScore(side);
			PlaySoundMem(g_resource.sound_se03, DX_PLAYTYPE_BACK);
		}
	}
	if (GameObject_Paddle_CollisionBall(&g_paddle1, &g_ball) || GameObject_Paddle_CollisionBall(&g_paddle2, &g_ball))
		PlaySoundMem(g_resource.sound_se01, DX_PLAYTYPE_BACK);
	GameObject_Field_CollisionVertical(&g_field, &g_paddle1, FALSE);
	GameObject_Field_CollisionVertical(&g_field, &g_paddle2, FALSE);
}

// <�Q�[���̍X�V����:�X�R�A���Z>
void UpdateGameScore(ObjectSide side)
{
	// ���_����
	GameScore_Add(&g_score, side);

	if (GameScore_IsFinished(&g_score))
	{
		GameObject_Ball_SetPosXDefault(&g_ball, &g_field);
		GameObject_Ball_SetVelXDefault(&g_ball);
		GameObject_Ball_SetVelYDefault(&g_ball);

		// �V�[�����f���ɕύX
		g_game_state = STATE_DEMO;
	}
	else
		// �V�[�����T�[�u�ɕύX
		g_game_state = STATE_SERVE;
}

//----------------------------------------------------------------------
//! @brief �Q�[���̕`�揈��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void RenderGame(void)
{
	switch (g_game_state)
	{
	case STATE_DEMO:
		RenderGameSceneDemo();
		break;
	case STATE_SERVE:
		RenderGameSceneServe();
		break;
	case STATE_PLAY:
		RenderGameScenePlay();
		break;
	}
}

// <�Q�[���̕`�揈��:�V�[��:�f��> ---------------------------------------------
void RenderGameSceneDemo(void)
{
	// <�I�u�W�F�N�g�`��>
	// �t�B�[���h�`��
	GameObject_Field_Render(&g_field);
	// �X�R�A�`��
	GameScore_Render(&g_score, g_resource.font);
	// �{�[���`��
	GameObject_Render(&g_ball, COLOR_WHITE);
}

// <�Q�[���̕`�揈��:�V�[��:�T�[�u> -------------------------------------------
void RenderGameSceneServe(void)
{
	// <�I�u�W�F�N�g�`��>
	// �t�B�[���h�`��
	GameObject_Field_Render(&g_field);
	// �X�R�A�`��
	GameScore_Render(&g_score, g_resource.font);
	// �p�h���`��
	GameObject_Render(&g_paddle1, COLOR_WHITE);
	GameObject_Render(&g_paddle2, COLOR_WHITE);
	// �{�[���`��
	GameObject_Render(&g_ball, COLOR_WHITE);
}

// <�Q�[���̕`�揈��:�V�[��:�v���C> -------------------------------------------
void RenderGameScenePlay(void)
{
	// <�I�u�W�F�N�g�`��>
	// �t�B�[���h�`��
	GameObject_Field_Render(&g_field);
	// �X�R�A�`��
	GameScore_Render(&g_score, g_resource.font);
	// �K�C�h�`��
	GameController_RenderGuide(&g_paddle1_ctrl);
	GameController_RenderGuide(&g_paddle2_ctrl);
	// �p�h���`��
	GameObject_Render(&g_paddle1, COLOR_WHITE);
	GameObject_Render(&g_paddle2, COLOR_WHITE);
	// �{�[���`��
	GameObject_Render(&g_ball, COLOR_WHITE);
}

//----------------------------------------------------------------------
//! @brief �Q�[���̏I������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void FinalizeGame(void)
{
	GameResource_Delete(&g_resource);
}
