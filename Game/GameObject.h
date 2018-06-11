#pragma once
#include "Vec2.h"

// �񋓌^�̒�` ============================================================

// <�V�[�����> --------------------------------------------------------
enum ObjectSide
{
	NONE = 0,
	CENTER_X,	// X����
	LEFT,		// ��
	RIGHT,		// �E
	CENTER_Y,	// Y����
	TOP,		// ��
	BOTTOM		// ��
};

// �\���̂̐錾 ============================================================

// <�Q�[���I�u�W�F�N�g>
typedef struct
{
	Vec2 pos;	// <�ʒu>
	Vec2 vel;	// <���x>
	Vec2 size;	// <�傫��>
} GameObject;

// �萔�̒�` ==============================================================

// <�{�[��>
#define BALL_VEL_X_MAX 5	// �p�h�����ړ����Ă����ԂŃ{�[���ɂ����������̑��x
#define BALL_VEL_X_MIN 3	// �p�h�����Î~���Ă����ԂŃ{�[���ɂ����������̑��x
#define BALL_VEL_Y 4.5f		// Y�̍ő呬�x

// <�p�h��>
#define PADDLE_VEL 8		// �p�h���̑��x

// �֐��̐錾 ==============================================================

// <<�I�u�W�F�N�g>> ----------------------------------------------------

// <�I�u�W�F�N�g�쐬>
GameObject GameObject_Create(Vec2 pos, Vec2 vel, Vec2 size);

// <�I�u�W�F�N�g���W�X�V>
void GameObject_UpdatePosition(GameObject* obj);

// <�I�u�W�F�N�gX�I�t�Z�b�g>
float GameObject_OffsetX(GameObject* obj, ObjectSide side, float pos = 0.f, float padding = 0.f);

// <�I�u�W�F�N�gX�I�t�Z�b�g>
float GameObject_OffsetY(GameObject* obj, ObjectSide side, float pos = 0.f, float padding = 0.f);

// <�I�u�W�F�N�gX�ʒu�Z�b�g>
void GameObject_SetX(GameObject* obj, ObjectSide side, float pos = 0.f, float padding = 0.f);

// <�I�u�W�F�N�gY�ʒu�Z�b�g>
void GameObject_SetY(GameObject* obj, ObjectSide side, float pos = 0.f, float padding = 0.f);

// <�I�u�W�F�N�gX�ʒu�Q�b�g>
float GameObject_GetX(GameObject* obj, ObjectSide side, float padding = 0.f);

// <�I�u�W�F�N�gY�ʒu�Q�b�g>
float GameObject_GetY(GameObject* obj, ObjectSide side, float padding = 0.f);

// <�I�u�W�F�N�g�����蔻��>
BOOL GameObject_IsHit(GameObject* obj1, GameObject* obj2);

// <�I�u�W�F�N�g�`��>
void GameObject_Render(GameObject* obj, unsigned int color);

// <<�{�[���I�u�W�F�N�g>> ----------------------------------------------

// <�{�[���I�u�W�F�N�g�쐬>
GameObject GameObject_Ball_Create(void);

// <�{�[���I�u�W�F�N�g���WX�f�t�H���g>
void GameObject_Ball_SetPosXDefault(GameObject* obj);

// <�{�[���I�u�W�F�N�g���WY�f�t�H���g>
void GameObject_Ball_SetPosYDefault(GameObject* obj);

// <�{�[���I�u�W�F�N�g�ړ�X�f�t�H���g>
void GameObject_Ball_SetVelXDefault(GameObject* obj);

// <�{�[���I�u�W�F�N�g�ړ�Y�f�t�H���g>
void GameObject_Ball_SetVelYDefault(GameObject* obj);

// <<�p�h���I�u�W�F�N�g>> ----------------------------------------------

// <�p�h���I�u�W�F�N�g�쐬>
GameObject GameObject_Paddle_Create(void);

// <�p�h���I�u�W�F�N�g���WY�f�t�H���g>
void GameObject_Paddle_SetPosYDefault(GameObject* obj);

// <�p�h���I�u�W�F�N�g�{�[���Փˏ���>
BOOL GameObject_Paddle_CollisionBall(GameObject* paddle, GameObject* ball);

// <<�t�B�[���h�I�u�W�F�N�g>> ------------------------------------------

// <�{�[���I�u�W�F�N�g�쐬>
GameObject GameObject_Field_Create(void);

// <�t�B�[���h�㉺�Փˏ���>
ObjectSide GameObject_Field_CollisionVertical(GameObject* field, GameObject* obj, BOOL flag_with_bounce);

// <�t�B�[���h���E�Փˏ���>
ObjectSide GameObject_Field_CollisionHorizontal(GameObject* field, GameObject* obj, BOOL flag_with_bounce);

// <�t�B�[���h�`��>
void GameObject_Field_Render(GameObject* field);
