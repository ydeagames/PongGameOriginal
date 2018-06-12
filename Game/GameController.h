#pragma once
#include "GameObject.h"

// �\���̂̐錾 ============================================================

// <�Q�[���R���g���[���[>
typedef struct tag_GameController GameController;
struct tag_GameController
{
	GameObject* object;
	void(*UpdateControl)(GameController*);

	Vec2 target_pos;
	GameObject* field;
	GameObject* ball;
	GameObject* enemy;

	// �p�����ł��Ȃ��̂Ŏd���Ȃ�Player�p�̕ϐ�
	int player_key_up;
	int player_key_down;
};

// �֐��̐錾 ==============================================================

// <<�R���g���[���[>> --------------------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Create(GameObject* object, void(*ctrlFunc)(GameController*), GameObject* field, GameObject* ball, GameObject* enemy);

// <�R���g���[���[�X�V>
void GameController_Update(GameController* ctrl);

// <�{�[�����e�_�\�z�A���S���Y��>
float GameController_GetTargetY(GameObject* field, GameObject* ball, GameObject* paddle_myself, GameObject* paddle_enemy);

// <<�v���C���[�R���g���[���[>> ----------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Player_Create(GameObject* object, GameObject* field, GameObject* ball, GameObject* enemy, int key_up, int key_down);

// <<Bot�R���g���[���[>> -----------------------------------------------

// <Bot�R���g���[���[�쐬>
GameController GameController_Bot_Create(GameObject* object, GameObject* field, GameObject* ball, GameObject* enemy);

// <Bot�K�C�h�`��>
void GameController_RenderGuide(GameController* ctrl);
