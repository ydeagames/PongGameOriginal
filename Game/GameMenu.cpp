#include "GameMenu.h"
#include "GameMain.h"

// �񋓌^�̒�` ============================================================

// <���j���[���> -----------------------------------------------------
enum GameMenuState
{
	// �ҋ@��
	STATE_MENU_IDLE,

	// �ڑ��Ҏ�
	STATE_MENU_WAITING,

	// �ڑ���
	STATE_MENU_CONNECTING
};

// �֐��̒�` ==============================================================

// <<���j���[>> --------------------------------------------------------

// <���j���[�쐬>
GameMenu GameMenu_Create(GameScene* scene, GameControllers* controllers)
{
	return { scene, controllers, 0, FALSE };
}

// <���j���[�X�V>
void GameMenu_Update(GameMenu* menu)
{
	if (menu->listening)
	{
		HNET handle = GetNewAcceptNetWork();

		if (handle != -1)
		{
			menu->controllers->paddle1 = GameController_Network_Create(&menu->scene->paddle1, menu->scene, &menu->scene->paddle2, TRUE, handle);

			// �ڑ��҂�������
			StopListenNetWork();

			menu->listening = FALSE;
			menu->connected = TRUE;
		}
	}

	{
		int sel = menu->connected ? 1 : 3;
		if (IsButtonPressed(PAD_INPUT_UP))
			menu->selected--;
		if (IsButtonPressed(PAD_INPUT_DOWN))
			menu->selected++;
		menu->selected = ((menu->selected % sel) + sel) % sel;
	}
}

IPDATA GameMenu_InputIP(GameMenu* menu, int x, int y)
{
	char StrBuf[81];
	// �h�o�̓��͂��s��
	KeyInputSingleCharString(x, y, 80, StrBuf, FALSE);

	IPDATA IP;
	{
		char StrBuf2[81];
		int i, j, k;
		// �s���I�h���R���邩���ׂ�
		j = 0;
		for (i = 0; i < 80; i++)
		{
			if (StrBuf[i] == '.') j++;
		}

		// �����R�s���I�h���Ȃ������ꍇ�͓��͂̂�����
		if (j == 3)
		{
			// �����񂩂�h�o�𔲂��o��
			j = 0;
			k = 0;
			i = 0;
			while (!ProcessMessage())
			{
				if (StrBuf[i] == '.' || StrBuf[i] == '\0')
				{
					StrBuf2[j] = '\0';
					switch (k)
					{
					case 0:IP.d1 = atoi(StrBuf2); break;
					case 1:IP.d2 = atoi(StrBuf2); break;
					case 2:IP.d3 = atoi(StrBuf2); break;
					case 3:IP.d4 = atoi(StrBuf2); break;
					}
					k++;
					if (k == 4) break;

					j = 0;
				}
				else
				{
					StrBuf2[j] = StrBuf[i];
					j++;
				}
				i++;
			}
		}
	}
	return IP;
}

// <���j���[���肪�����ꂽ�Ƃ�>
BOOL GameMenu_OnPressed(GameMenu* menu)
{
	if (IsButtonPressed(PAD_INPUT_10))
	{
		if (menu->selected == 1)
		{
			GameObject inner = menu->scene->field;
			inner.size.x -= 80;
			inner.size.y -= 160;
			inner.pos.y += 40;

			{
				IPDATA ip = GameMenu_InputIP(menu, (int)GameObject_GetX(&inner, LEFT, -100), (int)(inner.pos.y + 100));
				HNET handle = ConnectNetWork(ip, 9850);
				if (handle != -1)
				{
					menu->controllers->paddle1 = GameController_Player_Create(&menu->scene->paddle1, menu->scene, &menu->scene->paddle2, PAD_INPUT_UP, PAD_INPUT_DOWN);
					menu->controllers->paddle2 = GameController_Network_Create(&menu->scene->paddle2, menu->scene, &menu->scene->paddle1, FALSE, handle);

					menu->connected = TRUE;
				}
			}
		}
		else if (menu->selected == 2)
		{
			// �ڑ��҂���Ԃɂ���
			PreparationListenNetWork(9850);

			menu->listening = TRUE;
		}
	}

	return menu->selected == 0;
}

// <���j���[�`��>
void GameMenu_Render(GameMenu* menu, GameResource* res)
{
	GameObject inner = menu->scene->field;
	inner.size.x -= 80;
	inner.size.y -= 160;
	inner.pos.y += 10;

	{
		SetDrawBlendMode(DX_BLENDMODE_INVDESTCOLOR, 255);
		GameObject_Render(&menu->scene->field, 0x222222);
		GameObject_Render(&inner, COLOR_WHITE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	DrawFormatStringToHandle(
		(int)(menu->scene->field.pos.x - GetDrawFormatStringWidthToHandle(res->font_pong, GAME_TITLE) / 2),
		(int)(GameObject_GetY(&inner, TOP, -20)),
		COLOR_BLACK, res->font_pong, GAME_TITLE
	);

	{
		IPDATA ip;
		GetMyIPAddress(&ip);
		DrawFormatStringToHandle((int)GameObject_GetX(&inner, LEFT, -320), (int)(inner.pos.y + 120), COLOR_BLACK, res->font_menu, "IP�A�h���X: %d.%d.%d.%d", ip.d1, ip.d2, ip.d3, ip.d4);
	}

	{
		float padding_x = -50;
		float padding_y = 20;
		DrawFormatStringToHandle((int)GameObject_GetX(&inner, LEFT, padding_x), (int)(inner.pos.y - 20 + padding_y), COLOR_BLACK, res->font_menu,
			"%s �v���C�J�n", menu->selected == 0 ? "��" : "�@");
		if (menu->connected)
			DrawFormatStringToHandle((int)GameObject_GetX(&inner, LEFT, padding_x), (int)(inner.pos.y + 20 + padding_y), COLOR_BLACK, res->font_menu,
				"   �ڑ�����!");
		else if (menu->listening)
			DrawFormatStringToHandle((int)GameObject_GetX(&inner, LEFT, padding_x), (int)(inner.pos.y + 20 + padding_y), COLOR_BLACK, res->font_menu,
				"   ���̃v���C���[��҂��Ă��܂�...\n"
				"�@�@�@IP�A�h���X�𑼂̐l�ɓ`���ĉ�����\n");
		else
		{
			DrawFormatStringToHandle((int)GameObject_GetX(&inner, LEFT, padding_x), (int)(inner.pos.y + 20 + padding_y), COLOR_BLACK, res->font_menu,
				"%s ���̃v���C���[�ɎQ������", menu->selected == 1 ? "��" : "�@");
			DrawFormatStringToHandle((int)GameObject_GetX(&inner, LEFT, padding_x), (int)(inner.pos.y + 60 + padding_y), COLOR_BLACK, res->font_menu,
				"%s ���̃v���C���[��҂�", menu->selected == 2 ? "��" : "�@");
		}
	}

	DrawFormatStringToHandle((int)GameObject_GetX(&menu->scene->field, LEFT, -10), (int)GameObject_GetY(&menu->scene->field, BOTTOM, -40), COLOR_WHITE, res->font_note,
		"�� �ݒ�u���[�J���̃A�h���X�ɂ̓v���L�V�T�[�o�[���g��Ȃ��v�Ƀ`�F�b�N��t���ĉ�����\n"
		"�� �t�@�C�A�E�H�[���̉�ʂ��o�܂�����A�u�A�N�Z�X��������v��I�����ĉ�����\n");
}
