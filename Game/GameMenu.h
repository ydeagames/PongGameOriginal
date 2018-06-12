#pragma once
#include "GameScene.h"
#include "GameResource.h"
#include "GameControllers.h"

// �\���̂̐錾 ============================================================

// <���j���[>
typedef struct
{
	GameScene* scene;
	GameControllers* controllers;

	int selected;
	BOOL listening;
	BOOL connected;
	int failmsg_frame;

	GameResource* resources;
} GameMenu;

// �֐��̐錾 ==============================================================

// <<���j���[>> --------------------------------------------------------

// <���j���[�쐬>
GameMenu GameMenu_Create(GameScene* scene, GameControllers* controllers, GameResource* resources);

// <���j���[�X�V>
void GameMenu_Update(GameMenu* menu);

// <���j���[���肪�����ꂽ�Ƃ�>
BOOL GameMenu_OnPressed(GameMenu* menu);

// <���j���[�`��>
void GameMenu_Render(GameMenu* menu);
