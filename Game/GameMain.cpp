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

// <�Q�[���̍X�V����:�I�u�W�F�N�g:�����蔻��> --------------------------
int UpdateGameObjectCollisionBallTopBottom(void);
int UpdateGameObjectCollisionBallLeftRight(void);
int UpdateGameObjectCollisionBallLeftRightScoring(void);
int UpdateGameObjectCollisionBallPaddle(void);
void UpdateGameObjectCollisionPaddleTopBottom(void);

// <�Q�[���̍X�V����:���[�e�B���e�B> -----------------------------------
float GameObject_Paddle_GetBallVelX(float ball_vel_x, float paddle_vel_y);
float GameObject_Paddle_GetVelYFromPaddlePosY(float ball_pos_y, float paddle_pos_y);

// <�Q�[���̕`�揈��> --------------------------------------------------
void RenderGameSceneDemo(void);
void RenderGameSceneServe(void);
void RenderGameScenePlay(void);

// <�Q�[���̕`�揈��:�I�u�W�F�N�g> -------------------------------------
void RenderGameObjectField(void);
void RenderGameObjectScore(void);
void RenderGameObjectPaddleGuide(void);
void RenderGameObjectPaddle(void);
void RenderGameObjectBall(void);

// <�Q�[���̕`�揈��:���[�e�B���e�B> -----------------------------------
void RenderObj(float x, float y, float w, float h, unsigned int color);


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

	// �{�[��
	g_ball = GameObject_Ball_Create();
	GameObject_Ball_SetPosXDefault(&g_ball);
	GameObject_Ball_SetPosYDefault(&g_ball);
	GameObject_Ball_SetVelDefault(&g_ball);

	// �p�h��1
	g_paddle1 = GameObject_Paddle_Create();
	GameObject_SetLeft(&g_paddle1, SCREEN_LEFT, 64);
	GameObject_Paddle_SetPosYDefault(&g_paddle1);
	g_paddle1_ctrl = GameController_Bot_Create(&g_paddle1, &g_ball, &g_paddle2);

	// �p�h��2
	g_paddle2 = GameObject_Paddle_Create();
	GameObject_SetRight(&g_paddle2, SCREEN_RIGHT, 64);
	GameObject_Paddle_SetPosYDefault(&g_paddle2);
	g_paddle2_ctrl = GameController_Player_Create(&g_paddle2, PAD_INPUT_UP, PAD_INPUT_DOWN);

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
	UpdateGameObjectCollisionBallTopBottom();
	UpdateGameObjectCollisionBallLeftRight();
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
	g_paddle1_ctrl.UpdateControl(&g_paddle1_ctrl);
	g_paddle2_ctrl.UpdateControl(&g_paddle2_ctrl);

	// ���W�X�V
	GameObject_UpdatePosition(&g_ball);
	GameObject_UpdatePosition(&g_paddle1);
	GameObject_UpdatePosition(&g_paddle2);

	// �����蔻��
	UpdateGameObjectCollisionBallTopBottom();
	UpdateGameObjectCollisionBallPaddle();
	UpdateGameObjectCollisionPaddleTopBottom();
}

// <�Q�[���̍X�V����:�V�[��:�v���C> ------------------------------------
void UpdateGameScenePlay(void)
{
	// ����
	g_paddle1_ctrl.UpdateControl(&g_paddle1_ctrl);
	g_paddle2_ctrl.UpdateControl(&g_paddle2_ctrl);

	// ���W�X�V
	GameObject_UpdatePosition(&g_ball);
	GameObject_UpdatePosition(&g_paddle1);
	GameObject_UpdatePosition(&g_paddle2);

	// �����蔻��
	if (UpdateGameObjectCollisionBallTopBottom())
		PlaySoundMem(g_sound_se02, DX_PLAYTYPE_BACK);
	if (UpdateGameObjectCollisionBallLeftRightScoring())
		PlaySoundMem(g_sound_se03, DX_PLAYTYPE_BACK);
	if (UpdateGameObjectCollisionBallPaddle())
		PlaySoundMem(g_sound_se01, DX_PLAYTYPE_BACK);
	UpdateGameObjectCollisionPaddleTopBottom();
}

// <>
void UpdateGameScore()
{
	// ���_����
	if (g_ball.pos.x < padding_left)
		g_score2++;
	if (padding_right <= g_ball.pos.x)
		g_score1++;

	if (g_score1 >= SCORE_GOAL || g_score2 >= SCORE_GOAL)
	{
		// �������{�[�����W
		g_ball.pos.x = (float)(SCREEN_CENTER_X);
		// �������{�[�����x
		g_ball.vel.y = -BALL_VEL_Y;
		g_ball.vel.x = BALL_VEL_X_MIN;

		// �V�[�����f���ɕύX
		g_game_state = STATE_DEMO;
	}
	else
		// �V�[�����T�[�u�ɕύX
		g_game_state = STATE_SERVE;
}

// <�Q�[���̍X�V����:�Փ�:�p�h���~�{�[��> ------------------------------
int UpdateGameObjectCollisionBallPaddle(void)
{
	return
		GameObject_Paddle_CollisionBall(&g_paddle1, &g_ball) ||
		GameObject_Paddle_CollisionBall(&g_paddle2, &g_ball);
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
	// �I�u�W�F�N�g�`��
	RenderGameObjectField();
	RenderGameObjectScore();
	RenderGameObjectBall();
}

// <�Q�[���̕`�揈��:�V�[��:�T�[�u> -------------------------------------------
void RenderGameSceneServe(void)
{
	// �I�u�W�F�N�g�`��
	RenderGameObjectField();
	RenderGameObjectScore();
	RenderGameObjectPaddle();
	RenderGameObjectBall();
}

// <�Q�[���̕`�揈��:�V�[��:�v���C> -------------------------------------------
void RenderGameScenePlay(void)
{
	// �I�u�W�F�N�g�`��
	RenderGameObjectField();
	RenderGameObjectScore();
	RenderGameObjectPaddleGuide();
	RenderGameObjectPaddle();
	RenderGameObjectBall();
}

// <�Q�[���̕`�揈��:�R�[�g> -------------------------------------------
void RenderGameObjectField(void)
{
	// �R�[�g�\��
	DrawDashedLine(SCREEN_CENTER_X, SCREEN_TOP, SCREEN_CENTER_X, SCREEN_BOTTOM, COLOR_WHITE, 8, 2);
}

// <�Q�[���̕`�揈��:�X�R�A> -------------------------------------------
void RenderGameObjectScore(void)
{
	// �X�R�A�\��

	// �t�H���g���g�p���������̕����擾
	int width_score1 = GetDrawFormatStringWidthToHandle(g_font, "%2d", g_score1);

	DrawFormatStringToHandle(SCREEN_CENTER_X - 100 - width_score1, 10, COLOR_WHITE, g_font, "%2d", g_score1);
	DrawFormatStringToHandle(SCREEN_CENTER_X + 100, 10, COLOR_WHITE, g_font, "%2d", g_score2);
}

// <�Q�[���̕`�揈��:�p�h���K�C�h> -------------------------------------
void RenderGameObjectPaddleGuide(void)
{
	// �K�C�h�\��
	RenderObj(g_paddle1_ctrl.bot_target_pos.x, g_paddle1_ctrl.bot_target_pos.y, PADDLE_WIDTH, PADDLE_HEIGHT, 0x222222);
	//RenderObj(g_paddle2_target_pos.x, g_paddle2_target_pos.y, PADDLE_WIDTH, PADDLE_HEIGHT, 0x222222);
}

// <�Q�[���̕`�揈��:�p�h��> -------------------------------------------
void RenderGameObjectPaddle(void)
{
	// �p�h���\��
	RenderObj(g_paddle1.pos.x, g_paddle1.pos.y, PADDLE_WIDTH, PADDLE_HEIGHT, COLOR_WHITE);
	RenderObj(g_paddle2.pos.x, g_paddle2.pos.y, PADDLE_WIDTH, PADDLE_HEIGHT, COLOR_WHITE);
}

// <�Q�[���̕`�揈��:�{�[��> -------------------------------------------
void RenderGameObjectBall(void)
{
	// �{�[���\��
	RenderObj(g_ball.pos.x, g_ball.pos.y, BALL_SIZE, BALL_SIZE, COLOR_WHITE);
}

//----------------------------------------------------------------------
//! @brief �I�u�W�F�N�g�`��
//!
//! @param[x] X���W
//! @param[y] Y���W
//! @param[w] ��
//! @param[h] ����
//! @param[color] �F
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void RenderObj(float x, float y, float w, float h, unsigned int color)
{
	DrawBox((int)(x - w / 2), (int)(y - h / 2), (int)(x + w / 2), (int)(y + h / 2), color, TRUE);
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
