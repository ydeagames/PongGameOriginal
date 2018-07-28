//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.cpp
//!
//! @brief  Pong Online! �I���W�i���ۑ�
//!
//! @date   2018/06/13
//!
//! @author GF1 26 �R������
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "GameMain.h"
#include "GameObject.h"
#include "GameObjects.h"
#include "GameControllers.h"
#include "GameScore.h"
#include "GameResource.h"
#include "GameMenu.h"
#include "GameScene.h"


// �萔�̒�` ==============================================================

// <�_���K�C�h> --------------------------------------------------------
#define SCORE_TO_GUID 6

// <�T�[�u�ҋ@> --------------------------------------------------------
#define SERVE_WAIT_TIME 2*60


// �O���[�o���ϐ��̐錾 ====================================================

// <�V�[��> ------------------------------------------------------------
GameScene g_scene;

// <�R���g���[���[> ----------------------------------------------------
GameControllers g_controllers;

// <���\�[�X> ----------------------------------------------------------
GameResource g_resources;

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
	g_scene.game_state = STATE_DEMO;

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
	g_scene.paddle1.pos.x = GameObject_OffsetX(&g_scene.paddle1, LEFT, GameObject_GetX(&g_scene.field, LEFT), -64);
	GameObject_Paddle_SetPosYDefault(&g_scene.paddle1, &g_scene.field);
	g_controllers.paddle1 = GameController_Bot_Create(&g_scene.paddle1, &g_scene, &g_scene.paddle2);

	// �p�h��2
	g_scene.paddle2 = GameObject_Paddle_Create();
	g_scene.paddle2.pos.x = GameObject_OffsetX(&g_scene.paddle2, RIGHT, GameObject_GetX(&g_scene.field, RIGHT), -64);
	GameObject_Paddle_SetPosYDefault(&g_scene.paddle2, &g_scene.field);
	g_controllers.paddle2 = GameController_Player_Create(&g_scene.paddle2, &g_scene, &g_scene.paddle1, PAD_INPUT_UP, PAD_INPUT_DOWN);
	//g_controllers.paddle2 = GameController_Bot_Create(&g_scene.paddle2, &g_scene, &g_scene.paddle1);

	// ���\�[�X
	g_resources = GameResource_Create();

	// ���_
	g_scene.score = GameScore_Create();

	// �T�[�u�ҋ@
	g_scene.counter = 0;

	// ���j���[
	g_menu = GameMenu_Create(&g_scene, &g_controllers, &g_resources);
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
	GameTick_Update();

	switch (g_scene.game_state)
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
		if (IsButtonDown(PAD_INPUT_10) && GameMenu_OnPressed(&g_menu))
		{
			// �_�����Z�b�g
			GameScore_Clear(&g_scene.score);

			// X���W����ʒ����֖߂�
			GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);

			// �p�h���������ʒu��
			GameObject_Paddle_SetPosYDefault(&g_scene.paddle1, &g_scene.field);
			GameObject_Paddle_SetPosYDefault(&g_scene.paddle2, &g_scene.field);

			// �V�[�����v���C�ɕύX
			g_scene.game_state = STATE_PLAY;
			g_scene.packet = PACKET_START;
		}
	}

	// �R���g���[���[�X�V
	GameController_Update(&g_controllers.paddle1);
	GameController_Update(&g_controllers.paddle2);

	// ���W�X�V
	GameObject_UpdatePosition(&g_scene.ball);

	// �����蔻��
	if (GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, CONNECTION_BARRIER, EDGESIDE_INNER))
		g_scene.ball.vel.y *= -1;
	if (GameObject_Field_CollisionHorizontal(&g_scene.field, &g_scene.ball, CONNECTION_BARRIER, EDGESIDE_INNER))
		g_scene.ball.vel.x *= -1;

	// ���j���[�X�V
	GameMenu_Update(&g_menu);
}

// <�Q�[���̍X�V����:�V�[��:�T�[�u> ------------------------------------
void UpdateGameSceneServe(void)
{
	// �ҋ@&������
	g_scene.counter++;

	// ���Ԍo�߂�
	if (g_scene.counter >= SERVE_WAIT_TIME)
	{
		// X���W����ʒ����֖߂�
		GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);

		// �V�[�����v���C�ɕύX
		g_scene.game_state = STATE_PLAY;
		g_scene.packet = PACKET_SERVE;

		g_scene.counter = 0;
	}

	// �R���g���[���[�X�V
	GameController_Update(&g_controllers.paddle1);
	GameController_Update(&g_controllers.paddle2);

	// ����
	GameController_UpdateControl(&g_controllers.paddle1);
	GameController_UpdateControl(&g_controllers.paddle2);

	// ���W�X�V
	GameObject_UpdatePosition(&g_scene.ball);
	GameObject_UpdatePosition(&g_scene.paddle1);
	GameObject_UpdatePosition(&g_scene.paddle2);

	// �����蔻��
	if (GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, CONNECTION_BARRIER, EDGESIDE_INNER))
		g_scene.ball.vel.y *= -1;
	GameObject_Paddle_CollisionBall(&g_scene.paddle1, &g_scene.ball);
	GameObject_Paddle_CollisionBall(&g_scene.paddle2, &g_scene.ball);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle1, CONNECTION_BARRIER, EDGESIDE_INNER);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle2, CONNECTION_BARRIER, EDGESIDE_INNER);
}

// <�Q�[���̍X�V����:�V�[��:�v���C> ------------------------------------
void UpdateGameScenePlay(void)
{
	// �R���g���[���[�X�V
	GameController_Update(&g_controllers.paddle1);
	GameController_Update(&g_controllers.paddle2);

	// ����
	GameController_UpdateControl(&g_controllers.paddle1);
	GameController_UpdateControl(&g_controllers.paddle2);

	// ���W�X�V
	GameObject_UpdatePosition(&g_scene.ball);
	GameObject_UpdatePosition(&g_scene.paddle1);
	GameObject_UpdatePosition(&g_scene.paddle2);

	// �����蔻��
	if (GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, CONNECTION_BARRIER, EDGESIDE_INNER))
	{
		g_scene.ball.vel.y *= -1;
		PlaySoundMem(g_resources.sound_se02, DX_PLAYTYPE_BACK);
	}
	{
		ObjectSide side = GameObject_Field_CollisionHorizontal(&g_scene.field, &g_scene.ball, CONNECTION_NONE, EDGESIDE_OUTER);
		if (side)
		{
			UpdateGameScore(side);
			PlaySoundMem(g_resources.sound_se03, DX_PLAYTYPE_BACK);
		}
	}
	if (GameObject_Paddle_CollisionBall(&g_scene.paddle1, &g_scene.ball) || GameObject_Paddle_CollisionBall(&g_scene.paddle2, &g_scene.ball))
		PlaySoundMem(g_resources.sound_se01, DX_PLAYTYPE_BACK);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle1, CONNECTION_BARRIER, EDGESIDE_INNER);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle2, CONNECTION_BARRIER, EDGESIDE_INNER);
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
		g_scene.game_state = STATE_DEMO;
		g_scene.packet = PACKET_END;
	}
	else
	{
		// �V�[�����T�[�u�ɕύX
		g_scene.game_state = STATE_SERVE;
		g_scene.packet = PACKET_SCORE;
	}
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
	switch (g_scene.game_state)
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
	// �{�[���`��
	GameObject_Render(&g_scene.ball);
	// ���j���[�`��
	GameMenu_Render(&g_menu);
	// �X�R�A�`��
	GameScore_Render(&g_scene.score, &g_scene.field, g_resources.font_pong);
}

// <�Q�[���̕`�揈��:�V�[��:�T�[�u> -------------------------------------------
void RenderGameSceneServe(void)
{
	// <�I�u�W�F�N�g�`��>
	// �t�B�[���h�`��
	GameObject_Field_Render(&g_scene.field);
	// �X�R�A�`��
	GameScore_Render(&g_scene.score, &g_scene.field, g_resources.font_pong);
	// �p�h���`��
	GameObject_Render(&g_scene.paddle1);
	GameObject_Render(&g_scene.paddle2);
	// �{�[���`��
	GameObject_Render(&g_scene.ball);
}

// <�Q�[���̕`�揈��:�V�[��:�v���C> -------------------------------------------
void RenderGameScenePlay(void)
{
	// <�I�u�W�F�N�g�`��>
	// �t�B�[���h�`��
	GameObject_Field_Render(&g_scene.field);
	// �X�R�A�`��
	GameScore_Render(&g_scene.score, &g_scene.field, g_resources.font_pong);
	// �K�C�h�`��
	if (g_scene.score.score2 - g_scene.score.score1 >= SCORE_TO_GUID)
		GameController_RenderGuide(&g_controllers.paddle1);
	if (g_scene.score.score1 - g_scene.score.score2 >= SCORE_TO_GUID)
		GameController_RenderGuide(&g_controllers.paddle2);
	// �p�h���`��
	GameObject_Render(&g_scene.paddle1);
	GameObject_Render(&g_scene.paddle2);
	// �{�[���`��
	GameObject_Render(&g_scene.ball);
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
	GameResource_Delete(&g_resources);
}
