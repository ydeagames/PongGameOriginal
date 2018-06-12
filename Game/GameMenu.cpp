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
GameMenu GameMenu_Create(GameScene* scene)
{
	return { scene };
}

// <���j���[�X�V>
void GameMenu_Update(GameMenu* menu)
{
	{
		if (IsButtonPressed(PAD_INPUT_UP))
			menu->selected--;
		if (IsButtonPressed(PAD_INPUT_DOWN))
			menu->selected++;
		menu->selected = ((menu->selected % 3) + 3) % 3;
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
		if (menu->selected == 2)
		{
			GameObject inner = menu->scene->field;
			inner.size.x -= 80;
			inner.size.y -= 160;
			inner.pos.y += 40;

			IPDATA ip = GameMenu_InputIP(menu, (int)GameObject_GetX(&inner, LEFT, -100), (int)(inner.pos.y + 100));
		}
	}

	return menu->selected == 0;
}

// <���j���[�`��>
void GameMenu_Render(GameMenu* menu, GameResource* res)
{
	{
		GameObject inner = menu->scene->field;
		inner.size.x -= 80;
		inner.size.y -= 160;
		inner.pos.y += 40;

		{
			SetDrawBlendMode(DX_BLENDMODE_INVDESTCOLOR, 255);
			GameObject_Render(&menu->scene->field, 0x222222);
			GameObject_Render(&inner, COLOR_WHITE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		DrawFormatStringToHandle(
			(int)(menu->scene->field.pos.x - GetDrawFormatStringWidthToHandle(res->font, GAME_TITLE) / 2),
			(int)(GameObject_GetY(&inner, TOP, -20)),
			COLOR_BLACK, res->font, GAME_TITLE
		);

		{
			IPDATA ip;
			GetMyIPAddress(&ip);
			DrawFormatString((int)GameObject_GetX(&inner, LEFT, -100), (int)(inner.pos.y - 60), COLOR_BLACK, "%d.%d.%d.%d", ip.d1, ip.d2, ip.d3, ip.d4);
		}

		{
			DrawFormatString((int)GameObject_GetX(&inner, LEFT, -100), (int)(inner.pos.y - 20), COLOR_BLACK, "%s PLAY", menu->selected == 0 ? "��" : "�@");
			DrawFormatString((int)GameObject_GetX(&inner, LEFT, -100), (int)(inner.pos.y + 20), COLOR_BLACK, "%s PLAY FOR WAIT", menu->selected == 1 ? "��" : "�@");
			DrawFormatString((int)GameObject_GetX(&inner, LEFT, -100), (int)(inner.pos.y + 60), COLOR_BLACK, "%s PLAY TO JOIN", menu->selected == 2 ? "��" : "�@");
		}
	}
}
