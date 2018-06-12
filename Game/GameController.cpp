#include "GameController.h"

// �֐��̐錾 ==============================================================

void GameController_Player_Update(GameController* ctrl);
void GameController_Bot_Update(GameController* ctrl);
void GameController_Network_Update(GameController* ctrl);

// �֐��̒�` ==============================================================

// <<�R���g���[���[>> --------------------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Create(GameObject* object, void(*ctrlFunc)(GameController*), GameObject* field, GameObject* ball, GameObject* enemy)
{
	return { object, ctrlFunc, object->pos, field, ball, enemy };
}

// <�R���g���[���[�X�V>
void GameController_Update(GameController* ctrl)
{
	ctrl->target_pos.y = GameController_GetTargetY(ctrl->field, ctrl->ball, ctrl->object, ctrl->enemy);

	ctrl->UpdateControl(ctrl);
}

// <�{�[�����e�_�\�z�A���S���Y��>
float GameController_GetTargetY(GameObject* field, GameObject* ball, GameObject* paddle_myself, GameObject* paddle_enemy)
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

	{
		// �{�[���T�C�Y���l�������㉺�̕�
		screen_top_y = GameObject_OffsetY(ball, BOTTOM, GameObject_GetY(field, TOP));
		screen_bottom_y = GameObject_OffsetY(ball, TOP, GameObject_GetY(field, BOTTOM));
		// �{�[���T�C�Y���l�������{�[���̈ړ��͈�
		screen_height = screen_bottom_y - screen_top_y;
	}

	// �{�[����Y���W
	{
		ball_base_y = ball->pos.y - screen_top_y;
		if (ball->vel.y < 0)
			ball_base_y *= -1; // ���x��������̂Ƃ��A��Ƀ^�[�Q�b�g�����݂���
	}

	// �^�[�Q�b�g�̎Z�o
	ball_absolute_y = ball_base_y + length_y;

	// ��ʂ͈̔͊O��������height�������Ĕ͈͓��ɂ���
	// ���̂Ƃ��A����̉񐔂𐔂���
	{
		int count = 0;
		float pos_y_loop = ball_absolute_y;
		while (pos_y_loop < 0)
		{
			pos_y_loop += screen_height;
			count++;
		}
		while (pos_y_loop > screen_height)
		{
			pos_y_loop -= screen_height;
			count++;
		}

		// �͈͂𒲐�����񐔂���ł����Y���𒆐S�ɔ��]������
		if (count % 2 == 0)
			target_pos_y = pos_y_loop;
		else
			target_pos_y = screen_height - pos_y_loop;
	}

	return target_pos_y + screen_top_y;
}

// <<�v���C���[�R���g���[���[>> ----------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Player_Create(GameObject* object, GameObject* field, GameObject* ball, GameObject* enemy, int key_up, int key_down)
{
	GameController ctrl = GameController_Create(object, GameController_Player_Update, field, ball, enemy);
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

// <�R���g���[���[�쐬>
GameController GameController_Bot_Create(GameObject* object, GameObject* field, GameObject* ball, GameObject* enemy)
{
	return  GameController_Create(object, GameController_Bot_Update, field, ball, enemy);
}

// Bot���p�h���𑀍�
void GameController_Bot_Update(GameController* ctrl)
{
	// Bot�������n�߂邵�����l
	float padding = 260 * BALL_VEL_X_MIN / PADDLE_VEL;

	int k = (ctrl->object->pos.x < ctrl->enemy->pos.x) ? 1 : -1;

	ctrl->object->vel.y = 0.f;

	// �����������������l���߂������瓮��
	if (k * (ctrl->ball->vel.x) < 0 && k * (ctrl->ball->pos.x - (ctrl->object->pos.x + padding)) < 0)
	{
		// Bot���p�h���𑀍�
		float pos_y = ctrl->target_pos.y;

		// ���񂾂璆���ɖ߂�
		if (ctrl->ball->pos.x < GameObject_GetX(ctrl->field, LEFT))
			pos_y = ClampF(ctrl->ball->pos.y, ctrl->field->pos.y - 80.f, ctrl->field->pos.y + 80.f);

		// Bot���ړ��ł��镝�𐧌�
		//target1_pos_y = ClampF(target1_pos_y, SCREEN_TOP + 50, SCREEN_BOTTOM - 50);

		if (ctrl->object->pos.y - pos_y > 1.2f * PADDLE_VEL)
			ctrl->object->vel.y += -PADDLE_VEL;
		else if (ctrl->object->pos.y - pos_y < -1.2f * PADDLE_VEL)
			ctrl->object->vel.y += PADDLE_VEL;
	}
}

// <Bot�K�C�h�`��>
void GameController_RenderGuide(GameController* ctrl)
{
	GameObject target = *ctrl->object;
	target.pos = ctrl->target_pos;
	GameObject_Render(&target, 0x222222);
}

// <<�l�b�g���[�N�R���g���[���[>> --------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Network_Create(GameObject* object, GameObject* field, GameObject* ball, GameObject* enemy, BOOL server_flag, HNET handle)
{
	GameController ctrl = GameController_Create(object, GameController_Network_Update, field, ball, enemy);
	ctrl.network_server_flag = server_flag;
	ctrl.network_handle = handle;
	return ctrl;
}

typedef struct
{
	GameObject ball;
	GameObject paddle1;
	GameObject paddle2;
} NetworkPacket;

// �ʐM���肪�p�h���ƃ{�[���𑀍�
void GameController_Network_Update(GameController* ctrl)
{
	static int frame = 0;
	frame++;

	// �ؒf�m�F �� Bot�ɕύX
	if (GetLostNetWork() == ctrl->network_handle)
		ctrl->UpdateControl = GameController_Bot_Update;

	if (GetNetWorkDataLength(ctrl->network_handle) != 0)
	{
		NetworkPacket yourpacket;
		NetWorkRecv(ctrl->network_handle, &yourpacket, sizeof(NetworkPacket));

		float center = ctrl->field->pos.x;
		GameObject paddle;
		{
			BOOL b1 = (ctrl->enemy->pos.x < ctrl->object->pos.x);
			BOOL b2 = (yourpacket.paddle1.pos.x < yourpacket.paddle2.pos.x);
			if ((b1 && b2) || (!b1 && !b2))
				paddle = yourpacket.paddle2;
			else
				paddle = yourpacket.paddle1;
		}

		if ((center < yourpacket.ball.pos.x && center < paddle.pos.x) ||
			(yourpacket.ball.pos.x < center && paddle.pos.x < center))
		{
			*ctrl->ball = yourpacket.ball;
		}
		*ctrl->object = paddle;
	}

	if (frame % 3 == 0)
	{
		NetworkPacket mypacket = {
			*ctrl->ball,
			*ctrl->enemy,
			*ctrl->object
		};
		NetWorkSend(ctrl->network_handle, &mypacket, sizeof(NetworkPacket));
	}
}