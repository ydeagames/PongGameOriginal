#pragma once
#include "GameScene.h"

// �\���̂̐錾 ============================================================

// <�Q�[���R���g���[���[>
typedef struct tag_GameController GameController;
struct tag_GameController
{
	GameObject* object;
	void(*Update)(GameController*);
	void(*UpdateControl)(GameController*);

	Vec2 target_pos;
	GameScene* scene;
	GameObject* enemy;

	// �p�����ł��Ȃ��̂Ŏd���Ȃ�Player�p�̕ϐ�
	int player_key_up;
	int player_key_down;

	// �p�����ł��Ȃ��̂Ŏd���Ȃ��l�b�g���[�N�p�̕ϐ�
	BOOL network_server_flag;
	HNET network_handle;
};

// �֐��̐錾 ==============================================================

// <<�R���g���[���[>> --------------------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Create(GameObject* object, void(*updateFunc)(GameController*), void(*updateCtrlFunc)(GameController*), GameScene* scene, GameObject* enemy);

// <�R���g���[���[�X�V>
void GameController_Update(GameController* ctrl);

// <�R���g���[���[����X�V>
void GameController_UpdateControl(GameController* ctrl);

// <�{�[�����e�_�\�z�A���S���Y��>
float GameController_GetTargetY(GameObject* field, GameObject* ball, GameObject* paddle_myself, GameObject* paddle_enemy);

// <<�v���C���[�R���g���[���[>> ----------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Player_Create(GameObject* object, GameScene* scene, GameObject* enemy, int key_up, int key_down);

// <<Bot�R���g���[���[>> -----------------------------------------------

// <Bot�R���g���[���[�쐬>
GameController GameController_Bot_Create(GameObject* object, GameScene* scene, GameObject* enemy);

// <Bot�K�C�h�`��>
void GameController_RenderGuide(GameController* ctrl);


// <<�l�b�g���[�N�R���g���[���[>> --------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Network_Create(GameObject* object, GameScene* scene, GameObject* enemy, BOOL server_flag, HNET handle);
