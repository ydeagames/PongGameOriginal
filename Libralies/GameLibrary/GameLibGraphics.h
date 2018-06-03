//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameLibGraphics.h
//!
//! @brief  DxLib�̃O���t�B�b�N�֘A�ւ̒ǉ��@�\�̃w�b�_�t�@�C��
//!
//! @date   2017/3/27
//!
//! @author ���� �W
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once




// �萔�̒�` ==============================================================

// <�F�R�[�h(�W���J���[16�F)> ----------------------------------------------
enum ColorCode : unsigned int
{
	COLOR_BLACK   = 0xFF000000,    // ���F
	COLOR_NAVY    = 0xFF000080,    // �Z���F
	COLOR_GREEN   = 0xFF008000,    // �ΐF
	COLOR_TEAL    = 0xFF008080,    // �ΐF
	COLOR_MAROON  = 0xFF800000,    // �}���[���F
	COLOR_PURPLE  = 0xFF800080,    // ���F
	COLOR_OLIVE   = 0xFF808000,    // �I���[�u�F
	COLOR_GRAY    = 0xFF808080,    // �D�F
	COLOR_SILVER  = 0xFFC0C0C0,    // ��F
	COLOR_BLUE    = 0xFF0000FF,    // �F
	COLOR_LIME    = 0xFF00FF00,    // ���C���O���[���F
	COLOR_AQUA    = 0xFF00FFFF,    // ���F
	COLOR_RED     = 0xFFFF0000,    // �ԐF
	COLOR_FUCHSIA = 0xFFFF00FF,    // �Ԏ��F
	COLOR_YELLOW  = 0xFFFFFF00,    // ���F
	COLOR_WHITE   = 0xFFFFFFFF,    // ���F
};




// �֐��̐錾 ==============================================================

// <�}�`�`��֘A> ------------------------------------------------------

// �j����`��
int DrawDashedLine(int x1, int y1, int x2, int y2, unsigned int color, int length, int thickness);

// �j����`��(�A���`�G�C���A�X���ʕt��)
int DrawDashedLineAA(float x1, float y1, float x2, float y2, unsigned int color, float length, float thickness);


// <�O���t�B�b�N�f�[�^����֘A> ----------------------------------------

// �摜�t�@�C���̃������ւ̓ǂ݂���(�G���[�����t��)
int LoadGraphWithErrorHandling(const char* filename);
