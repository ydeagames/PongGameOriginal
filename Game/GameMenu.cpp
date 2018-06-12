#include "GameMenu.h"
#include "GameMain.h"

// �֐��̒�` ==============================================================

// <<���j���[>> --------------------------------------------------------

// <���j���[�쐬>
GameMenu GameMenu_Create(GameScene* scene, GameControllers* controllers, GameResource* resources)
{
	return { scene, controllers, 0, FALSE, FALSE, 0, resources };
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
			menu->controllers->paddle2 = GameController_Player_Create(&menu->scene->paddle2, menu->scene, &menu->scene->paddle1, PAD_INPUT_UP, PAD_INPUT_DOWN);

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

// <IP����͂���>
BOOL GameMenu_InputIP(GameMenu* menu, int x, int y, IPDATA* IP)
{
	char StrBuf[81];

	// IP�̓��͂���
	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_STR, COLOR_BLACK);
	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_CURSOR, COLOR_BLACK);
	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_SELECT_STR, COLOR_BLACK);
	if (KeyInputSingleCharString(x, y, 30, StrBuf, TRUE) != 1)
		return FALSE;

	{
		char StrBuf2[81];
		int i, j, k;
		// �s���I�h���R���邩���ׂ�
		j = 0;
		for (i = 0; i < 80; i++)
		{
			if (StrBuf[i] == '.') j++;
		}

		// �����R�s���I�h���������ꍇ
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
					case 0:IP->d1 = atoi(StrBuf2); break;
					case 1:IP->d2 = atoi(StrBuf2); break;
					case 2:IP->d3 = atoi(StrBuf2); break;
					case 3:IP->d4 = atoi(StrBuf2); break;
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
			return TRUE;
		}
	}

	return FALSE;
}

// <���j���[���肪�����ꂽ�Ƃ�>
BOOL GameMenu_OnPressed(GameMenu* menu)
{
	if (IsButtonPressed(PAD_INPUT_10))
	{
		if (menu->selected == 1)
		{
			// IP�A�h���X
			IPDATA ip;
			// �L����IP�A�h���X
			BOOL ip_valid = FALSE;

			{
				GameObject inner = menu->scene->field;
				inner.size.x -= 80;
				inner.size.y -= 160;
				inner.pos.y += 40;

				// KeyInputSingleCharString ���Q�[�����[�v���~�߂Ă��܂��̂ł����ŕ`��
				{
					GameObject banner = menu->scene->field;
					banner.size.y = 115;
					{
						GameObject_Render(&banner, COLOR_WHITE);
						DrawFormatStringToHandle((int)(GameObject_GetX(&banner, CENTER_X) - 200), (int)GameObject_GetY(&banner, TOP, -20), COLOR_BLACK, menu->resources->font_menu, "���ʐM�����IP�A�h���X����͂��ĉ�����");
						DrawFormatStringToHandle((int)(GameObject_GetX(&banner, CENTER_X) - 200), (int)GameObject_GetY(&banner, TOP, -46), COLOR_BLACK, menu->resources->font_note, "(��) 127.0.0.1�@�@Enter�Ŋm��@�@�s���I�h��3�܂݂܂�");
						DrawBox((int)GameObject_GetX(&banner, CENTER_X) - 200, (int)GameObject_GetY(&banner, TOP, -60), (int)GameObject_GetX(&banner, CENTER_X) + 200, (int)GameObject_GetY(&banner, TOP, -90), 0xeeeeee, TRUE);
					}
					ip_valid = GameMenu_InputIP(menu, (int)GameObject_GetX(&banner, CENTER_X) - 200 + 6, (int)GameObject_GetY(&banner, TOP, -60) + 6, &ip);
					{
						GameObject_Render(&banner, COLOR_WHITE);
						DrawFormatStringToHandle((int)(GameObject_GetX(&banner, CENTER_X) - 200), (int)GameObject_GetY(&banner, TOP, -40), COLOR_BLACK, menu->resources->font_menu, "�ڑ���....");
						ScreenFlip();
					}
				}
			}

			// �L����IP�A�h���X���ǂ���
			if (ip_valid)
			{
				HNET handle = ConnectNetWork(ip, 9850);
				if (handle != -1)
				{
					menu->controllers->paddle1 = GameController_Player_Create(&menu->scene->paddle1, menu->scene, &menu->scene->paddle2, PAD_INPUT_UP, PAD_INPUT_DOWN);
					menu->controllers->paddle2 = GameController_Network_Create(&menu->scene->paddle2, menu->scene, &menu->scene->paddle1, FALSE, handle);

					menu->connected = TRUE;
				}
			}

			if (!menu->connected)
				menu->failmsg_frame = 4 * 60;
		}
		else if (menu->selected == 2)
		{
			// �ڑ��҂���Ԃɂ���
			PreparationListenNetWork(9850);

			menu->listening = TRUE;
		}
	}

	if (menu->selected == 0)
	{
		StopListenNetWork();

		menu->listening = FALSE;

		return TRUE;
	}
	return FALSE;
}

// <���j���[�`��>
void GameMenu_Render(GameMenu* menu)
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

	{
		int width_pong = GetDrawFormatStringWidthToHandle(menu->resources->font_pong, GAME_TITLE);
		int width_c = GetDrawFormatStringWidthToHandle(menu->resources->font_note, "(c) 2018 YdeaGames");
		DrawFormatStringToHandle(
			(int)(menu->scene->field.pos.x - width_pong / 2),
			(int)(GameObject_GetY(&inner, TOP, -20)),
			COLOR_BLACK, menu->resources->font_pong, GAME_TITLE
		);
		DrawFormatStringToHandle(
			(int)(menu->scene->field.pos.x + width_pong / 2 - width_c),
			(int)(GameObject_GetY(&inner, TOP, -20)) + FONT_SIZE_PONG - 10,
			COLOR_BLACK, menu->resources->font_note, "(c) 2018 YdeaGames"
		);
	}

	{
		IPDATA ip;
		GetMyIPAddress(&ip);
		DrawFormatStringToHandle((int)GameObject_GetX(&inner, LEFT, -320), (int)(inner.pos.y + 120), COLOR_BLACK, menu->resources->font_menu, "IP�A�h���X: %d.%d.%d.%d", ip.d1, ip.d2, ip.d3, ip.d4);
	}

	{
		float padding_x = -50;
		float padding_y = 20;
		DrawFormatStringToHandle((int)GameObject_GetX(&inner, LEFT, padding_x), (int)(inner.pos.y - 20 + padding_y), COLOR_BLACK, menu->resources->font_menu,
			"%s �v���C�J�n", menu->selected == 0 ? "��" : "�@");
		if (menu->connected)
			DrawFormatStringToHandle((int)GameObject_GetX(&inner, LEFT, padding_x), (int)(inner.pos.y + 20 + padding_y), COLOR_BLACK, menu->resources->font_menu,
				"   �ڑ�����!");
		else if (menu->listening)
			DrawFormatStringToHandle((int)GameObject_GetX(&inner, LEFT, padding_x), (int)(inner.pos.y + 20 + padding_y), COLOR_BLACK, menu->resources->font_menu,
				"   ���̃v���C���[��҂��Ă��܂�...\n"
				"�@�@�@IP�A�h���X�𑼂̐l�ɓ`���ĉ�����\n");
		else
		{
			DrawFormatStringToHandle((int)GameObject_GetX(&inner, LEFT, padding_x), (int)(inner.pos.y + 20 + padding_y), COLOR_BLACK, menu->resources->font_menu,
				"%s ���̃v���C���[�ɎQ������", menu->selected == 1 ? "��" : "�@");
			DrawFormatStringToHandle((int)GameObject_GetX(&inner, LEFT, padding_x), (int)(inner.pos.y + 60 + padding_y), COLOR_BLACK, menu->resources->font_menu,
				"%s ���̃v���C���[��҂�", menu->selected == 2 ? "��" : "�@");
		}
	}

	DrawFormatStringToHandle((int)GameObject_GetX(&menu->scene->field, LEFT, -10), (int)GameObject_GetY(&menu->scene->field, BOTTOM, -50), COLOR_WHITE, menu->resources->font_note,
		"�� �����L�[: ���j���[�I���A�p�h������@�@�X�y�[�X�L�[: ���j���[����\n"
		"�� �ݒ�u���[�J���̃A�h���X�ɂ̓v���L�V�T�[�o�[���g��Ȃ��v�Ƀ`�F�b�N��t���ĉ�����\n"
		"�� �t�@�C�A�E�H�[���̉�ʂ��o�܂�����A�u�A�N�Z�X��������v��I�����ĉ�����\n");

	if (menu->failmsg_frame-- > 0)
	{
		GameObject alert = menu->scene->field;
		alert.pos.x += 160;
		alert.pos.y += 80;
		alert.size.x = 160;
		alert.size.y = 40;

		GameObject_Render(&alert, COLOR_RED);
		DrawFormatStringToHandle((int)GameObject_GetX(&alert, LEFT, -40), (int)GameObject_GetY(&alert, TOP, -10), COLOR_WHITE, menu->resources->font_menu,
			"�ڑ����s");
	}
}
