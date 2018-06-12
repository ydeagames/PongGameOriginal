#pragma once
#include "GameObject.h"
#include "GameResource.h"

// �\���̂̐錾 ============================================================

// <���j���[>
typedef struct
{
	GameObject* field;

	int selected;
} GameMenu;

// �֐��̐錾 ==============================================================

// <<���j���[>> --------------------------------------------------------

// <���j���[�쐬>
GameMenu GameMenu_Create(GameObject* field);

// <���j���[�X�V>
void GameMenu_Update(GameMenu* menu);

// <���j���[�`��>
void GameMenu_Render(GameMenu* menu, GameResource* res);
