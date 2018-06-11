#pragma once
#include "GameObject.h"

// �\���̂̐錾 ============================================================

// <�Q�[���R���g���[���[>
typedef struct tag_GameController GameController;
struct tag_GameController
{
	GameObject* object;
	void(*UpdateControl)(GameController*);

	// �p�����ł��Ȃ��̂Ŏd���Ȃ�Player�p�̕ϐ�
	int player_key_up;
	int player_key_down;

	// �p�����ł��Ȃ��̂Ŏd���Ȃ�Bot�p�̕ϐ�
	Vec2 bot_target_pos;
	GameObject* bot_ball;
	GameObject* bot_enemy;
};

// �֐��̐錾 ==============================================================

// <<�R���g���[���[>> --------------------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Create(GameObject* object, void(*ctrlFunc)(GameController*));

// <<�v���C���[�R���g���[���[>> ----------------------------------------

// <�v���C���[�R���g���[���[�쐬>
GameController GameController_Player_Create(GameObject* object, int key_up, int key_down);

// <<Bot�R���g���[���[>> -----------------------------------------------

// <Bot�R���g���[���[�쐬>
GameController GameController_Bot_Create(GameObject* object, GameObject* ball, GameObject* enemy);
