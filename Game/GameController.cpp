#include "GameController.h"
#include "GameMain.h"

// �֐��̐錾 ==============================================================

void GameController_Player_Update(GameController* ctrl);
void GameController_Bot_Update(GameController* ctrl);
float GetTargetY(GameObject* ball, GameObject* paddle_myself, GameObject* paddle_enemy);

// �֐��̒�` ==============================================================

// <<�R���g���[���[>> --------------------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Create(GameObject* object, void(*ctrlFunc)(GameController*))
{
	return { object, ctrlFunc };
}

// <<�v���C���[�R���g���[���[>> ----------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Player_Create(GameObject* object, int key_up, int key_down)
{
	GameController ctrl = GameController_Create(object, GameController_Player_Update);
	ctrl.player_key_up = key_up;
	ctrl.player_key_down = key_down;
	return ctrl;
}

// �L�[���͂Ńp�h���𑀍�
void GameController_Player_Update(GameController* ctrl)
{
	ctrl->object->vel.y = 0.f;
	if (IsButtonDown(ctrl->player_key_up))
		ctrl->object->vel.y += -PADDLE_VEL;
	if (IsButtonDown(ctrl->player_key_down))
		ctrl->object->vel.y += PADDLE_VEL;
}


// <<Bot�R���g���[���[>> -----------------------------------------------

// Bot��������
// <�R���g���[���[�쐬>
GameController GameController_Bot_Create(GameObject* object, GameObject* ball, GameObject* enemy)
{
	GameController ctrl = GameController_Create(object, GameController_Bot_Update);
	ctrl.bot_target_pos = object->pos;
	ctrl.bot_ball = ball;
	ctrl.bot_enemy = enemy;
	return ctrl;
}

void GameController_Bot_Update(GameController* ctrl)
{
	// Bot�������n�߂邵�����l
	float padding = 260 * BALL_VEL_X_MIN / PADDLE_VEL;

	int k = (ctrl->object->pos.x < ctrl->bot_enemy->pos.x) ? 1 : -1;

	ctrl->bot_target_pos.y = GetTargetY(ctrl->bot_ball, ctrl->object, ctrl->bot_enemy);

	ctrl->object->vel.y = 0.f;

	// �����������������l���߂������瓮��
	if (k * (ctrl->bot_ball->vel.x) < 0 && k * (ctrl->bot_ball->pos.x - (ctrl->object->pos.x + padding)) < 0)
	{
		// Bot���p�h���𑀍�
		float pos_y = ctrl->bot_target_pos.y;

		// ���񂾂璆���ɖ߂�
		if (ctrl->bot_ball->pos.x < SCREEN_LEFT)
			pos_y = ClampF(ctrl->bot_ball->pos.y, SCREEN_CENTER_Y - 80.f, SCREEN_CENTER_Y + 80.f);

		// Bot���ړ��ł��镝�𐧌�
		//target1_pos_y = ClampF(target1_pos_y, SCREEN_TOP + 50, SCREEN_BOTTOM - 50);

		if (ctrl->object->pos.y - pos_y > 1.2f * PADDLE_VEL)
			ctrl->object->vel.y += -PADDLE_VEL;
		else if (ctrl->object->pos.y - pos_y < -1.2f * PADDLE_VEL)
			ctrl->object->vel.y += PADDLE_VEL;
	}
}

// <�{�[�����e�_�\�z�A���S���Y��>
float GetTargetY(GameObject* ball, GameObject* paddle_myself, GameObject* paddle_enemy)
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
	// ���p�h��������{�[���̌��� (1: �E����, -1: ������)
	int k = (paddle_myself->pos.x < paddle_enemy->pos.x) ? 1 : -1;

	// �{�[���A�p�h���T�C�Y���l�������G�p�h���A���p�h����X���W
	{
		myself_pos_x = paddle_myself->pos.x - k * (ball->size.x / 2 + paddle_myself->size.x / 2);
		enemy_pos_x = paddle_enemy->pos.x + k * (ball->size.x / 2 + paddle_enemy->size.x / 2);
	}

	{
		// �{�[�����牽px�Ŏ��p�h���ɓ�������̂����Z�o
		{
			length_x = 0;
			if (k*ball->vel.x > 0)
			{
				// �{�[�����E�ɐi��ł���Ƃ� �������G������
				// �{�[���`�G�܂ł̋��� (�s��)
				length_x += k * (enemy_pos_x - ball->pos.x);
				// �G�`�����̋��� (�A��)
				length_x += k * (enemy_pos_x - myself_pos_x);
			}
			else
			{
				// �{�[�������ɐi��ł���Ƃ� �G������
				// �{�[���`�����܂ł̋���
				length_x += k * (ball->pos.x - myself_pos_x);
			}
		}

		// ���˕Ԃ�𖳎������Ƃ��A���p�h���ɓ������{�[����Y���W
		{
			length_y = length_x / ball->vel.x * ball->vel.y;
			if (length_y < 0)
				length_y *= -1; // ��Βl
		}
	}

	// �{�[����Y���W
	{
		ball_base_y = ball->pos.y;
		if (ball->vel.y < 0)
			ball_base_y *= -1; // ���x��������̂Ƃ��A��Ƀ^�[�Q�b�g�����݂���
	}

	// �^�[�Q�b�g�̎Z�o
	ball_absolute_y = ball_base_y + length_y;

	{
		// �{�[���T�C�Y���l�������㉺�̕�
		screen_bottom_y = SCREEN_BOTTOM - ball->size.y / 2;
		screen_top_y = SCREEN_TOP + ball->size.y / 2;
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
