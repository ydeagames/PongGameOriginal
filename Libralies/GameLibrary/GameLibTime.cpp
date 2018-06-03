//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameLibTime.cpp
//!
//! @brief  DxLib�̎��Ԋ֘A�ւ̒ǉ��@�\�̃\�[�X�t�@�C��
//!
//! @date   2017/3/27
//!
//! @author ���� �W
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include <DxLib.h>




// �萔�̒�` ==============================================================
#define ONE_SECOND 1000000    // 1�b (=1,000,000�ʕb)




// �t�@�C���X�R�[�v�ϐ��̒�` ==============================================
static float s_frameDeltaTime = 0.0f;    // �t���[���̌o�ߎ���[s]
static float s_frameRate      = 0.0f;    // �t���[�����[�g[fps]




// �֐��̒�` ==============================================================

//----------------------------------------------------------------------
//! @brief �t���[���^�C�}�[�̍X�V
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void UpdateFrameTimer(void)
{
	static LONGLONG previousTime = 0;                             // �O��̌v������
	static LONGLONG currentTime  = GetNowHiPerformanceCount();    // ���݂̌v������
	static LONGLONG startTime    = 0;                             // �t���[�����[�g�v���J�n����
	static int      frameCount   = 0;                             // �t���[����


	// ���Ԃ̌v��
	previousTime = currentTime;
	currentTime  = GetNowHiPerformanceCount();


	// �t���[���̌o�ߎ��Ԃ̍X�V
	s_frameDeltaTime = (float)(currentTime - previousTime) / ONE_SECOND;


	// 1�b���Ƀt���[�����[�g�̍X�V
	frameCount++;
	if ((currentTime - startTime) > ONE_SECOND)
	{
		s_frameRate = (float)(frameCount * ONE_SECOND) / (currentTime - startTime);
		startTime   = currentTime;
		frameCount  = 0;
	}
}



//------------------------------------------------------------------
//! @brief �t���[���̌o�ߎ��Ԃ̎擾
//!
//! @param[in] �Ȃ�
//!
//! @return �t���[���̌o�ߎ���[s]
//------------------------------------------------------------------
float GetFrameDeltaTime(void)
{
	return s_frameDeltaTime;
}



//------------------------------------------------------------------
//! @brief �t���[�����[�g�̎擾
//!
//! @param[in] �Ȃ�
//!
//! @return �t���[�����[�g[fps]
//------------------------------------------------------------------
float GetFrameRate(void)
{
	return s_frameRate;
}
