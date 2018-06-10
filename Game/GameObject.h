#pragma once
#include "Vec2.h"

// �\���̂̐錾 ============================================================

// <�Q�[���I�u�W�F�N�g>
typedef struct
{
	Vec2 pos;	// <�ʒu>
	Vec2 vel;	// <���x>
	Vec2 size;	// <�傫��>
} GameObject;

// �֐��̐錾 ==============================================================

// <<�I�u�W�F�N�g>> ----------------------------------------------------

// <�I�u�W�F�N�g�쐬>
GameObject GameObject_Create(Vec2 pos, Vec2 vel, Vec2 size);

// <�I�u�W�F�N�g���ʒu�Z�b�g>
void GameObject_SetLeft(GameObject* obj, float left, float padding = 0.f);

// <�I�u�W�F�N�g���ʒu�Z�b�g>
void GameObject_SetRight(GameObject* obj, float right, float padding = 0.f);

// <�I�u�W�F�N�g���ʒu�Z�b�g>
void GameObject_SetTop(GameObject* obj, float top, float padding = 0.f);

// <�I�u�W�F�N�g���ʒu�Z�b�g>
void GameObject_SetBottom(GameObject* obj, float bottom, float padding = 0.f);

// <<�{�[���I�u�W�F�N�g>> ----------------------------------------------

// <�{�[���I�u�W�F�N�g�쐬>
GameObject GameObject_Ball_Create(void);

// <�{�[���I�u�W�F�N�g���WX�f�t�H���g>
void GameObject_Ball_SetPosXDefault(GameObject* obj);

// <�{�[���I�u�W�F�N�g���WY�f�t�H���g>
void GameObject_Ball_SetPosYDefault(GameObject* obj);

// <�{�[���I�u�W�F�N�g�ړ��f�t�H���g>
void GameObject_Ball_SetVelDefault(GameObject* obj);

// <<�p�h���I�u�W�F�N�g>> ----------------------------------------------

// <�p�h���I�u�W�F�N�g�쐬>
GameObject GameObject_Paddle_Create(void);

// <�p�h���I�u�W�F�N�g���WY�f�t�H���g>
void GameObject_Paddle_SetPosYDefault(GameObject* obj);