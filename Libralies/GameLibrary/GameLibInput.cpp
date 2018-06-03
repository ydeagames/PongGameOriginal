//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameLibInput.cpp
//!
//! @brief  DxLib�̓��͊֘A�ւ̒ǉ��@�\�̃\�[�X�t�@�C��
//!
//! @date   2017/7/1
//!
//! @author ���� �W
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include <DxLib.h>




// �t�@�C���X�R�[�v�ϐ��̒�` ==============================================
static int s_currentInputState;
static int s_lastInputState;




// �֐��̒�` ==============================================================

//------------------------------------------------------------------
//! @brief �W���C�p�b�h�̓��͏�Ԃ̍X�V����
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//------------------------------------------------------------------
void UpdateJoypadInputState(void)
{
	// ��Ԃ̗�����ۑ�
	s_lastInputState = s_currentInputState;


	// �Q�[���p�b�h�̌��݂̏�Ԃ��擾
	s_currentInputState = GetJoypadInputState(DX_INPUT_KEY_PAD1);
}



//------------------------------------------------------------------
//! @brief �w�肳�ꂽ�{�^����������Ă��邩�ǂ����𔻒f
//!
//! @param[in] button ���肵�����{�^���̓��̓}�X�N
//!
//! @retval TRUE  �w�肳�ꂽ�{�^����������Ă���ꍇ
//! @retval FALSE ����ȊO�̏ꍇ
//------------------------------------------------------------------
BOOL IsButtonDown(int button)
{
	if ((s_currentInputState & button) == button)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



//------------------------------------------------------------------
//! @brief �w�肳�ꂽ�{�^����������Ă��Ȃ����ǂ����𔻒f
//!
//! @param[in] button ���肵�����{�^���̓��̓}�X�N
//!
//! @retval TRUE  �w�肳�ꂽ�{�^����������Ă��Ȃ��ꍇ
//! @retval FALSE ����ȊO�̏ꍇ
//------------------------------------------------------------------
BOOL IsButtonUp(int button)
{
	if ((s_currentInputState & button) != button)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



//------------------------------------------------------------------
//! @brief �w�肳�ꂽ�{�^���������ꂽ���ǂ����𔻒f
//!
//! @param[in] button ���肵�����{�^���̓��̓}�X�N
//!
//! @retval TRUE  �w�肳�ꂽ�L�[�������ꂽ�ꍇ
//! @retval FALSE ����ȊO�̏ꍇ
//------------------------------------------------------------------
BOOL IsButtonPressed(int button)
{
	if (((~s_lastInputState & s_currentInputState) & button) == button)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



//------------------------------------------------------------------
//! @brief �w�肳�ꂽ�{�^���������ꂽ���ǂ����𔻒f
//!
//! @param[in] button ���肵�����{�^���̓��̓}�X�N
//!
//! @retval TRUE  �w�肳�ꂽ�L�[�������ꂽ�ꍇ
//! @retval FALSE ����ȊO�̏ꍇ
//------------------------------------------------------------------
BOOL IsButtonReleased(int button)
{
	if (((s_lastInputState & ~s_currentInputState) & button) == button)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
