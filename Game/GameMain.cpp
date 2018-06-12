//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.cpp
//!
//! @brief  Pong �I���W�i���ۑ�
//!
//! @date   2018/06/13
//!
//! @author GF1 26 �R�� ����
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "GameMain.h"
#include "GameObject.h"
#include "GameController.h"
#include "GameScore.h"
#include "GameResource.h"
#include "GameMenu.h"
#include "GameScene.h"


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

// <�V�[��> ------------------------------------------------------------
GameScene g_scene;

// <���\�[�X> ----------------------------------------------------------
GameResource g_resource;

// <�T�[�u�ҋ@> --------------------------------------------------------
int g_counter;

// <���j���[> ----------------------------------------------------------
GameMenu g_menu;


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
	g_scene.field = GameObject_Field_Create();

	// �{�[��
	g_scene.ball = GameObject_Ball_Create();
	GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);
	GameObject_Ball_SetPosYDefault(&g_scene.ball, &g_scene.field);
	GameObject_Ball_SetVelXDefault(&g_scene.ball);
	GameObject_Ball_SetVelYDefault(&g_scene.ball);

	// �p�h��1
	g_scene.paddle1 = GameObject_Paddle_Create();
	GameObject_SetX(&g_scene.paddle1, RIGHT, SCREEN_LEFT, 64);
	GameObject_Paddle_SetPosYDefault(&g_scene.paddle1);
	g_scene.paddle1_ctrl = GameController_Bot_Create(&g_scene.paddle1, &g_scene.field, &g_scene.ball, &g_scene.paddle2);

	// �p�h��2
	g_scene.paddle2 = GameObject_Paddle_Create();
	GameObject_SetX(&g_scene.paddle2, LEFT, SCREEN_RIGHT, 64);
	GameObject_Paddle_SetPosYDefault(&g_scene.paddle2);
	g_scene.paddle2_ctrl = GameController_Player_Create(&g_scene.paddle2, &g_scene.field, &g_scene.ball, &g_scene.paddle1, PAD_INPUT_UP, PAD_INPUT_DOWN);

	// ���\�[�X
	g_resource = GameResource_Create();

	// ���_
	g_scene.score = GameScore_Create(&g_scene.field);

	// �T�[�u�ҋ@
	g_counter = 0;

	// ���j���[
	g_menu = GameMenu_Create(&g_scene);
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
		if (IsButtonDown(PAD_INPUT_10)&&GameMenu_OnPressed(&g_menu))
		{
			// �_�����Z�b�g
			GameScore_Clear(&g_scene.score);

			// X���W����ʒ����֖߂�
			GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);

			// �p�h���������ʒu��
			GameObject_Paddle_SetPosYDefault(&g_scene.paddle1);
			GameObject_Paddle_SetPosYDefault(&g_scene.paddle2);

			// �V�[�����v���C�ɕύX
			g_game_state = STATE_PLAY;
		}
	}

	// ���W�X�V
	GameObject_UpdatePosition(&g_scene.ball);

	// �����蔻��
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, TRUE);
	GameObject_Field_CollisionHorizontal(&g_scene.field, &g_scene.ball, TRUE);

	// ���j���[�X�V
	GameMenu_Update(&g_menu);
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
		GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);

		// �V�[�����v���C�ɕύX
		g_game_state = STATE_PLAY;

		g_counter = 0;
	}

	// ����
	GameController_Update(&g_scene.paddle1_ctrl);
	GameController_Update(&g_scene.paddle2_ctrl);

	// ���W�X�V
	GameObject_UpdatePosition(&g_scene.ball);
	GameObject_UpdatePosition(&g_scene.paddle1);
	GameObject_UpdatePosition(&g_scene.paddle2);

	// �����蔻��
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, TRUE);
	GameObject_Paddle_CollisionBall(&g_scene.paddle1, &g_scene.ball);
	GameObject_Paddle_CollisionBall(&g_scene.paddle2, &g_scene.ball);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle1, FALSE);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle2, FALSE);
}

// <�Q�[���̍X�V����:�V�[��:�v���C> ------------------------------------
void UpdateGameScenePlay(void)
{
	// ����
	GameController_Update(&g_scene.paddle1_ctrl);
	GameController_Update(&g_scene.paddle2_ctrl);

	// ���W�X�V
	GameObject_UpdatePosition(&g_scene.ball);
	GameObject_UpdatePosition(&g_scene.paddle1);
	GameObject_UpdatePosition(&g_scene.paddle2);

	// �����蔻��
	if (GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, TRUE))
		PlaySoundMem(g_resource.sound_se02, DX_PLAYTYPE_BACK);
	{
		ObjectSide side = GameObject_Field_CollisionHorizontal(&g_scene.field, &g_scene.ball, FALSE);
		if (side)
		{
			UpdateGameScore(side);
			PlaySoundMem(g_resource.sound_se03, DX_PLAYTYPE_BACK);
		}
	}
	if (GameObject_Paddle_CollisionBall(&g_scene.paddle1, &g_scene.ball) || GameObject_Paddle_CollisionBall(&g_scene.paddle2, &g_scene.ball))
		PlaySoundMem(g_resource.sound_se01, DX_PLAYTYPE_BACK);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle1, FALSE);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle2, FALSE);
}

// <�Q�[���̍X�V����:�X�R�A���Z>
void UpdateGameScore(ObjectSide side)
{
	// ���_����
	GameScore_Add(&g_scene.score, side);

	if (GameScore_IsFinished(&g_scene.score))
	{
		GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);
		GameObject_Ball_SetVelXDefault(&g_scene.ball);
		GameObject_Ball_SetVelYDefault(&g_scene.ball);

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
	GameObject_Field_Render(&g_scene.field);
	// �X�R�A�`��
	GameScore_Render(&g_scene.score, g_resource.font);
	// �{�[���`��
	GameObject_Render(&g_scene.ball, COLOR_WHITE);
	// ���j���[�`��
	GameMenu_Render(&g_menu, &g_resource);
}

// <�Q�[���̕`�揈��:�V�[��:�T�[�u> -------------------------------------------
void RenderGameSceneServe(void)
{
	// <�I�u�W�F�N�g�`��>
	// �t�B�[���h�`��
	GameObject_Field_Render(&g_scene.field);
	// �X�R�A�`��
	GameScore_Render(&g_scene.score, g_resource.font);
	// �p�h���`��
	GameObject_Render(&g_scene.paddle1, COLOR_WHITE);
	GameObject_Render(&g_scene.paddle2, COLOR_WHITE);
	// �{�[���`��
	GameObject_Render(&g_scene.ball, COLOR_WHITE);
}

// <�Q�[���̕`�揈��:�V�[��:�v���C> -------------------------------------------
void RenderGameScenePlay(void)
{
	// <�I�u�W�F�N�g�`��>
	// �t�B�[���h�`��
	GameObject_Field_Render(&g_scene.field);
	// �X�R�A�`��
	GameScore_Render(&g_scene.score, g_resource.font);
	// �K�C�h�`��
	GameController_RenderGuide(&g_scene.paddle1_ctrl);
	GameController_RenderGuide(&g_scene.paddle2_ctrl);
	// �p�h���`��
	GameObject_Render(&g_scene.paddle1, COLOR_WHITE);
	GameObject_Render(&g_scene.paddle2, COLOR_WHITE);
	// �{�[���`��
	GameObject_Render(&g_scene.ball, COLOR_WHITE);
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
