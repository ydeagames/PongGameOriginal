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

// <���_> --------------------------------------------------------------
#define SCORE_GOAL 11		// �ő�X�R�A

// <�t�H���g> ----------------------------------------------------------
#define FONT_CUSTOM_FILE "Resources\\Fonts\\TheSlavicFont-Regular.ttf"
#define FONT_CUSTOM_NAME "The Slavic Font"
#define FONT_NAME "HGP�n�p�p�޼��UB"
#define FONT_SIZE_SCORE 100

// <�T�E���h> ----------------------------------------------------------
#define SOUND_SE01 "Resources\\Audio\\SE01.ogg"
#define SOUND_SE02 "Resources\\Audio\\SE02.ogg"
#define SOUND_SE03 "Resources\\Audio\\SE03.ogg"

// <�T�[�u�ҋ@> --------------------------------------------------------
#define SERVE_WAIT_TIME 2*60


// �O���[�o���ϐ��̐錾 ====================================================

// <����> --------------------------------------------------------------
int g_input_state;

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

// <���_> --------------------------------------------------------------
int g_score1;
int g_score2;

// <�t�H���g> ----------------------------------------------------------
HFNT g_font;

// <�T�E���h> ----------------------------------------------------------
HSND g_sound_se01;
HSND g_sound_se02;
HSND g_sound_se03;

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

// <�Q�[���̕`�揈��:�I�u�W�F�N�g> -------------------------------------
void RenderGameObjectScore(void);
void RenderGameObjectPaddleGuide(void);
void RenderGameObjectPaddle(void);
void RenderGameObjectBall(void);

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
	GameObject_Ball_SetPosXDefault(&g_ball);
	GameObject_Ball_SetPosYDefault(&g_ball);
	GameObject_Ball_SetVelXDefault(&g_ball);
	GameObject_Ball_SetVelYDefault(&g_ball);

	// �p�h��1
	g_paddle1 = GameObject_Paddle_Create();
	GameObject_SetX(&g_paddle1, RIGHT, SCREEN_LEFT, 64);
	GameObject_Paddle_SetPosYDefault(&g_paddle1);
	g_paddle1_ctrl = GameController_Bot_Create(&g_paddle1, &g_ball, &g_paddle2);

	// �p�h��2
	g_paddle2 = GameObject_Paddle_Create();
	GameObject_SetX(&g_paddle2, LEFT, SCREEN_RIGHT, 64);
	GameObject_Paddle_SetPosYDefault(&g_paddle2);
	g_paddle2_ctrl = GameController_Player_Create(&g_paddle2, &g_ball, &g_paddle1, PAD_INPUT_UP, PAD_INPUT_DOWN);

	// ���_
	g_score1 = 0;
	g_score2 = 0;

	// �t�H���g
	if (AddFontResourceEx(FONT_CUSTOM_FILE, FR_PRIVATE, NULL) > 0)
		g_font = CreateFontToHandle(FONT_CUSTOM_NAME, FONT_SIZE_SCORE, 3, DX_FONTTYPE_ANTIALIASING_4X4);
	else
		g_font = CreateFontToHandle(FONT_NAME, FONT_SIZE_SCORE, 3, DX_FONTTYPE_ANTIALIASING_4X4);

	// �T�E���h
	g_sound_se01 = LoadSoundMem(SOUND_SE01);
	g_sound_se02 = LoadSoundMem(SOUND_SE02);
	g_sound_se03 = LoadSoundMem(SOUND_SE03);

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
	// �L�[�{�[�h�擾
	g_input_state = GetJoypadInputState(DX_INPUT_KEY_PAD1);

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
		if (g_input_state & PAD_INPUT_10)
		{
			// �_�����Z�b�g
			g_score1 = 0;
			g_score2 = 0;

			// X���W����ʒ����֖߂�
			GameObject_Ball_SetPosXDefault(&g_ball);

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
		GameObject_Ball_SetPosXDefault(&g_ball);

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
		PlaySoundMem(g_sound_se02, DX_PLAYTYPE_BACK);
	{
		ObjectSide side = GameObject_Field_CollisionHorizontal(&g_field, &g_ball, FALSE);
		if (side)
		{
			UpdateGameScore(side);
			PlaySoundMem(g_sound_se03, DX_PLAYTYPE_BACK);
		}
	}
	if (GameObject_Paddle_CollisionBall(&g_paddle1, &g_ball) || GameObject_Paddle_CollisionBall(&g_paddle2, &g_ball))
		PlaySoundMem(g_sound_se01, DX_PLAYTYPE_BACK);
	GameObject_Field_CollisionVertical(&g_field, &g_paddle1, FALSE);
	GameObject_Field_CollisionVertical(&g_field, &g_paddle2, FALSE);
}

// <�Q�[���̍X�V����:�X�R�A���Z>
void UpdateGameScore(ObjectSide side)
{
	// ���_����
	switch (side)
	{
	case LEFT:
		g_score2++;
		break;
	case RIGHT:
		g_score1++;
		break;
	}

	if (g_score1 >= SCORE_GOAL || g_score2 >= SCORE_GOAL)
	{
		GameObject_Ball_SetPosXDefault(&g_ball);
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
	RenderGameObjectScore();
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
	RenderGameObjectScore();
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
	RenderGameObjectScore();
	RenderGameObjectPaddleGuide();
	// �p�h���`��
	GameObject_Render(&g_paddle1, COLOR_WHITE);
	GameObject_Render(&g_paddle2, COLOR_WHITE);
	// �{�[���`��
	GameObject_Render(&g_ball, COLOR_WHITE);
}

// <�Q�[���̕`�揈��:�X�R�A> -------------------------------------------
void RenderGameObjectScore(void)
{
	// �X�R�A�`��

	// �t�H���g���g�p���������̕����擾
	int width_score1 = GetDrawFormatStringWidthToHandle(g_font, "%2d", g_score1);

	DrawFormatStringToHandle(SCREEN_CENTER_X - 100 - width_score1, 10, COLOR_WHITE, g_font, "%2d", g_score1);
	DrawFormatStringToHandle(SCREEN_CENTER_X + 100, 10, COLOR_WHITE, g_font, "%2d", g_score2);
}

// <�Q�[���̕`�揈��:�p�h���K�C�h> -------------------------------------
void RenderGameObjectPaddleGuide(void)
{
	// �K�C�h�\��
	GameObject paddle1_target = g_paddle1;
	GameObject paddle2_target = g_paddle2;
	paddle1_target.pos = g_paddle1_ctrl.target_pos;
	paddle2_target.pos = g_paddle2_ctrl.target_pos;
	GameObject_Render(&paddle1_target, 0x222222);
	GameObject_Render(&paddle2_target, 0x222222);
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
	// �t�H���g
	DeleteFontToHandle(g_font);
	RemoveFontResourceEx(FONT_CUSTOM_FILE, FR_PRIVATE, NULL);

	// �T�E���h
	DeleteSoundMem(g_sound_se01);
	DeleteSoundMem(g_sound_se02);
	DeleteSoundMem(g_sound_se03);
}
