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
#include "BallObject.h"
#include "PaddleObject.h"


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
#define SCORE_GOAL 11

// <�t�H���g> ----------------------------------------------------------
#define FONT_CUSTOM_FILE "Resources\\Fonts\\TheSlavicFont-Regular.ttf"
#define FONT_CUSTOM_NAME "The Slavic Font"
#define FONT_NAME "HGP�n�p�p�޼��UB"
#define FONT_SIZE_SCORE 100

// <�T�E���h> ----------------------------------------------------------
#define SOUND_SE01 "Resources\\Sounds\\SE01.ogg"
#define SOUND_SE02 "Resources\\Sounds\\SE02.ogg"
#define SOUND_SE03 "Resources\\Sounds\\SE03.ogg"

// <�T�[�u�ҋ@> --------------------------------------------------------
#define SERVE_WAIT_TIME 2*60


// �O���[�o���ϐ��̐錾 ====================================================

// <����> --------------------------------------------------------------
int g_input_state;

// <�V�[�����> --------------------------------------------------------
int g_game_state;

// <�{�[��> ------------------------------------------------------------
BallObject g_ball;

// <�p�h��1> -----------------------------------------------------------
PaddleObject g_paddle1;
Vec2 g_paddle1_target_pos;

// <�p�h��2> -----------------------------------------------------------
PaddleObject g_paddle2;
Vec2 g_paddle2_target_pos;

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

// <�Q�[���̍X�V����:����:���W> ----------------------------------------
void UpdateGameControlPaddle1(void);
void UpdateGameControlPaddle2(void);
void UpdateGameControlPaddlePlayer1(void);
void UpdateGameControlPaddlePlayer2(void);
void UpdateGameControlPaddleBot1(void);
void UpdateGameControlPaddleBot2(void);

// <�Q�[���̍X�V����:�I�u�W�F�N�g:���W> --------------------------------
void UpdateGameObjectPositionBall(void);
void UpdateGameObjectPositionPaddle(void);
void UpdateGameObjectPositionPaddleTarget(void);

// <�Q�[���̍X�V����:�I�u�W�F�N�g:�����蔻��> --------------------------
int UpdateGameObjectCollisionBallTopBottom(void);
int UpdateGameObjectCollisionBallLeftRight(void);
int UpdateGameObjectCollisionBallLeftRightScoring(void);
int UpdateGameObjectCollisionBallPaddle(void);
void UpdateGameObjectCollisionPaddleTopBottom(void);

// <�Q�[���̍X�V����:���[�e�B���e�B> -----------------------------------
float getTargetY(float paddle_enemy_pos_x, float paddle_myself_pos_x, int k);

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
	g_ball = {
		{
			(float)(SCREEN_CENTER_X),
			(float)(SCREEN_CENTER_Y),
		},
		{
			g_ball.BallObject::BALL_VEL_X_MIN,
			-g_ball.BallObject::BALL_VEL_Y,
		},
	};

	// �p�h��1
	g_paddle1 = {
		{
			(float)(SCREEN_LEFT + (PaddleObject::PaddleObject::PADDLE_WIDTH / 2 + 64)),
			(float)(SCREEN_BOTTOM - 30),
		},
		{
			0.f,
			0.f,
		},
	};
	g_paddle1_target_pos = g_paddle1.pos;

	// �p�h��2
	g_paddle2 = {
		{
			(float)(SCREEN_RIGHT - (PaddleObject::PaddleObject::PADDLE_WIDTH / 2 + 64)),
			(float)(SCREEN_BOTTOM - 30),
		},
		{
			0.f,
			0.f,
		},
	};
	g_paddle2_target_pos = g_paddle2.pos;

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
			g_ball.pos.x = (float)(SCREEN_CENTER_X);

			// �p�h���������ʒu��
			g_paddle1.pos.y = (float)(SCREEN_BOTTOM - 30);
			g_paddle2.pos.y = (float)(SCREEN_BOTTOM - 30);

			// �V�[�����v���C�ɕύX
			g_game_state = STATE_PLAY;
		}
	}

	// ���W�X�V
	UpdateGameObjectPositionBall();

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
		g_ball.pos.x = (float)(SCREEN_CENTER_X);

		// �V�[�����v���C�ɕύX
		g_game_state = STATE_PLAY;

		g_counter = 0;
	}

	// ����
	UpdateGameControlPaddle1();
	UpdateGameControlPaddle2();

	// ���W�X�V
	UpdateGameObjectPositionBall();
	UpdateGameObjectPositionPaddle();

	// �����蔻��
	UpdateGameObjectCollisionBallTopBottom();
	UpdateGameObjectCollisionBallPaddle();
	UpdateGameObjectCollisionPaddleTopBottom();
}

// <�Q�[���̍X�V����:�V�[��:�v���C> ------------------------------------
void UpdateGameScenePlay(void)
{
	// �^�[�Q�b�g���W�X�V
	UpdateGameObjectPositionPaddleTarget();

	// ����
	UpdateGameControlPaddle1();
	UpdateGameControlPaddle2();

	// ���W�X�V
	UpdateGameObjectPositionBall();
	UpdateGameObjectPositionPaddle();

	// �����蔻��
	if (UpdateGameObjectCollisionBallTopBottom())
		PlaySoundMem(g_sound_se02, DX_PLAYTYPE_BACK);
	if (UpdateGameObjectCollisionBallLeftRightScoring())
		PlaySoundMem(g_sound_se03, DX_PLAYTYPE_BACK);
	if (UpdateGameObjectCollisionBallPaddle())
		PlaySoundMem(g_sound_se01, DX_PLAYTYPE_BACK);
	UpdateGameObjectCollisionPaddleTopBottom();
}

// <�Q�[���̍X�V����:����:�p�h��1> -------------------------------------
void UpdateGameControlPaddle1(void)
{
	// ����

	//UpdateGameControlPaddlePlayer1();
	UpdateGameControlPaddleBot1();
}

// <�Q�[���̍X�V����:����:�p�h��2> -------------------------------------
void UpdateGameControlPaddle2(void)
{
	// ����

	UpdateGameControlPaddlePlayer2();
	//UpdateGameControlPaddleBot2();
}

// <�Q�[���̍X�V����:����:�v���C���[1> ---------------------------------
void UpdateGameControlPaddlePlayer1(void)
{
	// �L�[���͂Ńp�h��1�𑀍�
	g_paddle1.vel.y = 0.f;
	if (g_input_state & (PAD_INPUT_8 | PAD_INPUT_4))
		g_paddle1.vel.y += -PaddleObject::PaddleObject::PADDLE_VEL;
	if (g_input_state & (PAD_INPUT_5 | PAD_INPUT_1))
		g_paddle1.vel.y += PaddleObject::PaddleObject::PADDLE_VEL;
}

// <�Q�[���̍X�V����:����:�v���C���[2> ---------------------------------
void UpdateGameControlPaddlePlayer2(void)
{
	// �L�[���͂Ńp�h��2�𑀍�
	g_paddle2.vel.y = 0.f;
	if (g_input_state & PAD_INPUT_UP)
		g_paddle2.vel.y += -PaddleObject::PaddleObject::PADDLE_VEL;
	if (g_input_state & PAD_INPUT_DOWN)
		g_paddle2.vel.y += PaddleObject::PaddleObject::PADDLE_VEL;
}

// <�Q�[���̍X�V����:����:Bot1> ----------------------------------------
void UpdateGameControlPaddleBot1(void)
{
	// Bot�������n�߂邵�����l
	float padding = 260 * BallObject::BallObject::BALL_VEL_X_MIN / PaddleObject::PaddleObject::PADDLE_VEL;

	g_paddle1.vel.y = 0.f;

	// �����������������l���߂������瓮��
	if (g_ball.vel.x < 0 && g_ball.pos.x < g_paddle1.pos.x + padding)
	{
		// Bot���p�h��1�𑀍�
		float target1_pos_y = g_paddle1_target_pos.y;

		// ���񂾂璆���ɖ߂�
		if (g_ball.pos.x < SCREEN_LEFT)
			target1_pos_y = ClampF(g_paddle1.pos.y, SCREEN_CENTER_Y - 80.f, SCREEN_CENTER_Y + 80.f);

		// Bot���ړ��ł��镝�𐧌�
		//target1_pos_y = ClampF(target1_pos_y, SCREEN_TOP + 50, SCREEN_BOTTOM - 50);

		if (g_paddle1.pos.y - target1_pos_y > 1.2f * PaddleObject::PADDLE_VEL)
			g_paddle1.vel.y += -PaddleObject::PADDLE_VEL;
		else if (g_paddle1.pos.y - target1_pos_y < -1.2f * PaddleObject::PADDLE_VEL)
			g_paddle1.vel.y += PaddleObject::PADDLE_VEL;
	}
}

// <�Q�[���̍X�V����:����:Bot2> ----------------------------------------
void UpdateGameControlPaddleBot2(void)
{
	// Bot�������n�߂邵�����l
	float padding = 260 * BallObject::BALL_VEL_X_MIN / PaddleObject::PADDLE_VEL;

	g_paddle2.vel.y = 0.f;

	// �����������������l���߂������瓮��
	if (g_ball.vel.x > 0 && g_ball.pos.x > g_paddle2.pos.x - padding)
	{
		// Bot���p�h��2�𑀍�
		float target2_pos_y = g_paddle2_target_pos.y;

		// ���񂾂璆���ɖ߂�
		if (g_ball.pos.x > SCREEN_RIGHT)
			target2_pos_y = ClampF(g_paddle2.pos.y, SCREEN_CENTER_Y - 80.f, SCREEN_CENTER_Y + 80.f);

		// Bot���ړ��ł��镝�𐧌�
		//target2_pos_y = ClampF(target2_pos_y, SCREEN_TOP + 50, SCREEN_BOTTOM - 50);

		if (g_paddle2.pos.y - target2_pos_y > 1.2f * PaddleObject::PADDLE_VEL)
			g_paddle2.vel.y += -PaddleObject::PADDLE_VEL;
		else if (g_paddle2.pos.y - target2_pos_y < -1.2f * PaddleObject::PADDLE_VEL)
			g_paddle2.vel.y += PaddleObject::PADDLE_VEL;
	}
}

// <�Q�[���̍X�V����:���W:�{�[��> --------------------------------------
void UpdateGameObjectPositionBall(void)
{
	// ���W�X�V

	// pos��vel�𑫂�
	g_ball.UpdatePosition();
}

// <�Q�[���̍X�V����:���W:�p�h��> --------------------------------------
void UpdateGameObjectPositionPaddle(void)
{
	// ���W�X�V

	// pos��vel�𑫂�
	g_paddle1.UpdatePosition();

	// pos��vel�𑫂�
	g_paddle2.UpdatePosition();
}

// <�Q�[���̍X�V����:���W:�p�h���^�[�Q�b�g> ----------------------------
void UpdateGameObjectPositionPaddleTarget(void)
{
	// �^�[�Q�b�g�v�Z
	g_paddle1_target_pos.y = getTargetY(g_paddle1.pos.x, g_paddle2.pos.x, 1);
	g_paddle2_target_pos.y = getTargetY(g_paddle2.pos.x, g_paddle1.pos.x, -1);
}

// <�Q�[���̍X�V����:�Փ�:�{�[���~�Ǐ㉺> ------------------------------
int UpdateGameObjectCollisionBallTopBottom(void)
{
	// �q�b�g�t���O
	int flag_hit = 0;

	// �{�[���E�㉺�Ǔ����蔻��
	{
		float padding_top = SCREEN_TOP + (BallObject::BALL_SIZE / 2);
		float padding_bottom = SCREEN_BOTTOM - (BallObject::BALL_SIZE / 2);

		// ��ʊO�ɏo���Ƃ��̏���
		if (g_ball.pos.y < padding_top || padding_bottom <= g_ball.pos.y)
		{
			g_ball.vel.y *= -1.f;

			flag_hit = 1;
		}

		// �ǂɂ߂荞�܂Ȃ��悤�ɒ���
		g_ball.pos.y = ClampF(g_ball.pos.y, padding_top, padding_bottom);
	}

	return flag_hit;
}

// <�Q�[���̍X�V����:�Փ�:�{�[���~�Ǎ��E> ------------------------------
int UpdateGameObjectCollisionBallLeftRight(void)
{
	// �q�b�g�t���O
	int flag_hit = 0;

	// �{�[���E���E�Ǔ����蔻��
	{
		float padding_left = SCREEN_LEFT + (BallObject::BALL_SIZE / 2);
		float padding_right = SCREEN_RIGHT - (BallObject::BALL_SIZE / 2);

		// ��ʊO�ɏo���Ƃ��̏���
		if (g_ball.pos.x < padding_left || padding_right <= g_ball.pos.x)
		{
			g_ball.vel.x *= -1.f;

			flag_hit = 1;
		}

		// �ǂɂ߂荞�܂Ȃ��悤�ɒ���
		g_ball.pos.x = ClampF(g_ball.pos.x, padding_left, padding_right);
	}

	return flag_hit;
}

// <�Q�[���̍X�V����:�Փ�:�{�[���~�Ǎ��E(�X�R�A)> ----------------------
int UpdateGameObjectCollisionBallLeftRightScoring(void)
{
	// �q�b�g�t���O
	int flag_hit = 0;

	// �p�h���E���E�Ǔ����蔻��
	{
		float padding_left = SCREEN_LEFT + (BallObject::BALL_SIZE / 2);
		float padding_right = SCREEN_RIGHT - (BallObject::BALL_SIZE / 2);

		if (g_ball.pos.x < padding_left || padding_right <= g_ball.pos.x)
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
				g_ball.vel.y = -BallObject::BALL_VEL_Y;
				g_ball.vel.x = BallObject::BALL_VEL_X_MIN;

				// �V�[�����f���ɕύX
				g_game_state = STATE_DEMO;
			}
			else
				// �V�[�����T�[�u�ɕύX
				g_game_state = STATE_SERVE;

			flag_hit = 1;
		}
	}

	return flag_hit;
}

// <�Q�[���̍X�V����:�Փ�:�p�h���~�{�[��> ------------------------------
int UpdateGameObjectCollisionBallPaddle(void)
{
	// �{�[���E�p�h�������蔻��
	return
		g_paddle1.Collision(g_ball) ||
		g_paddle2.Collision(g_ball);
}

// <�Q�[���̍X�V����:�Փ�:�{�[���~�p�h��> ------------------------------
void UpdateGameObjectCollisionPaddleTopBottom(void)
{
	// �p�h���E�㉺�Ǔ����蔻��

	float padding_top = SCREEN_TOP + (PaddleObject::PADDLE_HEIGHT / 2);
	float padding_bottom = SCREEN_BOTTOM - (PaddleObject::PADDLE_HEIGHT / 2);

	// �ǂɂ߂荞�܂Ȃ��悤�ɒ���
	g_paddle1.pos.y = ClampF(g_paddle1.pos.y, padding_top, padding_bottom);

	// �ǂɂ߂荞�܂Ȃ��悤�ɒ���
	g_paddle2.pos.y = ClampF(g_paddle2.pos.y, padding_top, padding_bottom);
}

//----------------------------------------------------------------------
//! @brief Bot AI
//! �{�[�����e�_�\�z�A���S���Y��
//!
//! @param[paddle_enemy_pos_x] �G�p�h����X���W
//! @param[paddle_myself_pos_x] ���p�h����X���W
//! @param[k] ���p�h��������{�[���̌��� (1: �E����, 2: ������)
//!
//! @return �\��Y���W
//----------------------------------------------------------------------
float getTargetY(float paddle_enemy_pos_x, float paddle_myself_pos_x, int k)
{
	// �{�[���A�p�h���T�C�Y���l�������G�p�h���A���p�h����X���W
	float enemy_pos_x, myself_pos_x;
	// �{�[�����牽px�Ŏ��p�h���ɓ�������̂����Z�o
	float length_x, length_y;
	// �{�[���̊�ʒu
	float ball_base_y;
	// �ڕW�̍��W
	float ball_absolute_y;
	// �{�[���̈ړ��\�͈�
	float screen_top_y, screen_bottom_y, screen_height;
	// �ڕW�̉�ʓ����W
	float target_pos_y;

	// �{�[���A�p�h���T�C�Y���l�������G�p�h���A���p�h����X���W
	{
		enemy_pos_x = paddle_myself_pos_x - k * (BallObject::BALL_SIZE / 2 + PaddleObject::PADDLE_WIDTH / 2);
		myself_pos_x = paddle_enemy_pos_x + k * (BallObject::BALL_SIZE / 2 + PaddleObject::PADDLE_WIDTH / 2);
	}

	{
		// �{�[�����牽px�Ŏ��p�h���ɓ�������̂����Z�o
		{
			length_x = 0;
			if (k*g_ball.vel.x > 0)
			{
				// �{�[�����E�ɐi��ł���Ƃ� �������G������
				// �{�[���`�G�܂ł̋��� (�s��)
				length_x += k * (enemy_pos_x - g_ball.pos.x);
				// �G�`�����̋��� (�A��)
				length_x += k * (enemy_pos_x - myself_pos_x);
			}
			else
			{
				// �{�[�������ɐi��ł���Ƃ� �G������
				// �{�[���`�����܂ł̋���
				length_x += k * (g_ball.pos.x - myself_pos_x);
			}
		}

		// ���˕Ԃ�𖳎������Ƃ��A���p�h���ɓ������{�[����Y���W
		{
			length_y = length_x / g_ball.vel.x * g_ball.vel.y;
			if (length_y < 0)
				length_y *= -1; // ��Βl
		}
	}

	// �{�[����Y���W
	{
		ball_base_y = g_ball.pos.y;
		if (g_ball.vel.y < 0)
			ball_base_y *= -1; // ���x��������̂Ƃ��A��Ƀ^�[�Q�b�g�����݂���
	}

	// �^�[�Q�b�g�̎Z�o
	ball_absolute_y = ball_base_y + length_y;

	{
		// �{�[���T�C�Y���l�������㉺�̕�
		screen_bottom_y = SCREEN_BOTTOM - BallObject::BALL_SIZE / 2;
		screen_top_y = SCREEN_TOP + BallObject::BALL_SIZE / 2;
		// �{�[���T�C�Y���l�������{�[���̈ړ��͈�
		screen_height = screen_bottom_y - screen_top_y;
	}

	// ��ʂ͈̔͊O��������height�������Ĕ͈͓��ɂ���
	// ���̂Ƃ��A����̉񐔂𐔂���
	{
		int count = 0;
		float pos_y_loop = ball_absolute_y;
		while (pos_y_loop < screen_top_y)
		{
			pos_y_loop += screen_height;
			count++;
		}
		while (pos_y_loop > screen_bottom_y)
		{
			pos_y_loop -= screen_height;
			count++;
		}

		// �͈͂𒲐�����񐔂���ł����Y���𒆐S�ɔ��]������
		if (count % 2 == 0)
			target_pos_y = pos_y_loop;
		else
			target_pos_y = screen_bottom_y - pos_y_loop;
	}

	return target_pos_y;
}

//----------------------------------------------------------------------
//! @brief �p�h���̑��x����{�[��X���x�����߂�
//!
//! @param[ball_vel_x] ���݂̃{�[����X���x
//! @param[PaddleObject::PADDLE_VEL_y] �p�h����Y���x
//!
//! @return �V�����{�[����X���x
//----------------------------------------------------------------------
float GetVelXFromPaddleVelY(float ball_vel_x, float paddle_vel_y)
{
	float ball_vel_diff_x, ball_vel_new_x;

	ball_vel_diff_x = paddle_vel_y / PaddleObject::PADDLE_VEL * (BallObject::BALL_VEL_X_MAX - BallObject::BALL_VEL_X_MIN);
	if (ball_vel_diff_x < 0)
		ball_vel_diff_x *= -1;
	ball_vel_new_x = ball_vel_diff_x + BallObject::BALL_VEL_X_MIN;
	if (ball_vel_x < 0)
		ball_vel_new_x *= -1;

	return ball_vel_new_x;
}

//----------------------------------------------------------------------
//! @brief �p�h���ɂ��������ʒu����{�[��Y���x�����߂�
//!
//! @param[ball_pos_y] �{�[����Y���W
//! @param[paddle_pos_y] �p�h����Y���W
//!
//! @return �V�����{�[����Y���x
//----------------------------------------------------------------------
float GetVelYFromPaddlePosY(float ball_pos_y, float paddle_pos_y)
{
	float range_top = paddle_pos_y - (PaddleObject::PADDLE_HEIGHT / 2 - BallObject::BALL_SIZE / 2);
	float range_bottom = paddle_pos_y + (PaddleObject::PADDLE_HEIGHT / 2 - BallObject::BALL_SIZE / 2);
	float range_height = range_bottom - range_top;

	return ((((ball_pos_y - range_top) / range_height) * 2 - 1)*BallObject::BALL_VEL_Y);
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
	RenderObj(g_paddle1_target_pos.x, g_paddle1_target_pos.y, PaddleObject::PADDLE_WIDTH, PaddleObject::PADDLE_HEIGHT, 0x222222);
	RenderObj(g_paddle2_target_pos.x, g_paddle2_target_pos.y, PaddleObject::PADDLE_WIDTH, PaddleObject::PADDLE_HEIGHT, 0x222222);
}

// <�Q�[���̕`�揈��:�p�h��> -------------------------------------------
void RenderGameObjectPaddle(void)
{
	// �p�h���\��
	RenderObj(g_paddle1.pos.x, g_paddle1.pos.y, PaddleObject::PADDLE_WIDTH, PaddleObject::PADDLE_HEIGHT, COLOR_WHITE);
	RenderObj(g_paddle2.pos.x, g_paddle2.pos.y, PaddleObject::PADDLE_WIDTH, PaddleObject::PADDLE_HEIGHT, COLOR_WHITE);
}

// <�Q�[���̕`�揈��:�{�[��> -------------------------------------------
void RenderGameObjectBall(void)
{
	// �{�[���\��
	RenderObj(g_ball.pos.x, g_ball.pos.y, BallObject::BALL_SIZE, BallObject::BALL_SIZE, COLOR_WHITE);
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
