#pragma once
#include "GameObject.h"

// �萔�̒�` ==============================================================

// <�v���C���[>
#define PLAYER_VEL 4.f		// �v���C���[�̑��x
// <�G>
#define ENEMY_VEL 2			// �G�̑��x
// <�e>
#define BULLET_VEL 3.f		// �G�̑��x

// <�{�[��>
#define BALL_VEL_X_MAX 5	// �p�h�����ړ����Ă����ԂŃ{�[���ɂ����������̑��x
#define BALL_VEL_X_MIN 3	// �p�h�����Î~���Ă����ԂŃ{�[���ɂ����������̑��x
#define BALL_VEL_Y 4.5f		// Y�̍ő呬�x

// <�p�h��>
#define PADDLE_VEL 8		// �p�h���̑��x

// �񋓌^�̒�` ============================================================

// <���> --------------------------------------------------------------

typedef enum
{
	TYPE_UNKNOWN = 0,
	TYPE_PLAYER,
	TYPE_PLAYER_BULLET,
	TYPE_ENEMY1,
	TYPE_ENEMY2
} ObjectType;

// �֐��̐錾 ==============================================================

// <<�{�[���I�u�W�F�N�g>> ----------------------------------------------

// <�{�[���I�u�W�F�N�g�쐬>
GameObject GameObject_Ball_Create(void);

// <�{�[���I�u�W�F�N�g���WX�f�t�H���g>
void GameObject_Ball_SetPosXDefault(GameObject* obj, GameObject* field);

// <�{�[���I�u�W�F�N�g���WY�f�t�H���g>
void GameObject_Ball_SetPosYDefault(GameObject* obj, GameObject* field);

// <�{�[���I�u�W�F�N�g�ړ�X�f�t�H���g>
void GameObject_Ball_SetVelXDefault(GameObject* obj);

// <�{�[���I�u�W�F�N�g�ړ�Y�f�t�H���g>
void GameObject_Ball_SetVelYDefault(GameObject* obj);

// <<�p�h���I�u�W�F�N�g>> ----------------------------------------------

// <�p�h���I�u�W�F�N�g�쐬>
GameObject GameObject_Paddle_Create(void);

// <�p�h���I�u�W�F�N�g���WY�f�t�H���g>
void GameObject_Paddle_SetPosYDefault(GameObject* obj, GameObject* field);

// <�p�h���I�u�W�F�N�g�{�[���Փˏ���>
BOOL GameObject_Paddle_CollisionBall(GameObject* paddle, GameObject* ball);
