#pragma once
#include "GameScene.h"
#include "GameResource.h"

// �\���̂̐錾 ============================================================

// <���j���[>
typedef struct
{
	GameScene* scene;

	int selected;
} GameMenu;

// �֐��̐錾 ==============================================================

// <<���j���[>> --------------------------------------------------------

// <���j���[�쐬>
GameMenu GameMenu_Create(GameScene* scene);

// <���j���[�X�V>
void GameMenu_Update(GameMenu* menu);

// <���j���[���肪�����ꂽ�Ƃ�>
BOOL GameMenu_OnPressed(GameMenu* menu);

// <���j���[�`��>
void GameMenu_Render(GameMenu* menu, GameResource* res);
