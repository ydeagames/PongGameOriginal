#include "GameMenu.h"
#include "GameMain.h"

// 列挙型の定義 ============================================================

// <メニュー状態> -----------------------------------------------------
enum GameMenuState
{
	// 待機中
	STATE_MENU_IDLE,

	// 接続待受
	STATE_MENU_WAITING,

	// 接続中
	STATE_MENU_CONNECTING
};

// 関数の定義 ==============================================================

// <<メニュー>> --------------------------------------------------------

// <メニュー作成>
GameMenu GameMenu_Create(GameScene* scene, GameControllers* controllers)
{
	return { scene, controllers };
}

// <メニュー更新>
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
	// ＩＰの入力を行う
	KeyInputSingleCharString(x, y, 80, StrBuf, FALSE);

	IPDATA IP;
	{
		char StrBuf2[81];
		int i, j, k;
		// ピリオドが３つあるか調べる
		j = 0;
		for (i = 0; i < 80; i++)
		{
			if (StrBuf[i] == '.') j++;
		}

		// もし３つピリオドがなかった場合は入力のし直し
		if (j == 3)
		{
			// 文字列からＩＰを抜き出す
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

// <メニュー決定が押されたとき>
BOOL GameMenu_OnPressed(GameMenu* menu)
{
	if (IsButtonPressed(PAD_INPUT_10))
	{
		if (menu->selected == 1)
		{
			// 接続待ち状態にする
			PreparationListenNetWork(9850);

			// 接続があるまでここでループ
			while (!ProcessMessage())
			{
				HNET handle = GetNewAcceptNetWork();

				if (handle != -1)
				{
					menu->controllers->paddle1 = GameController_Network_Create(&menu->scene->paddle1, &menu->scene->field, &menu->scene->ball, &menu->scene->paddle2, TRUE, handle);
					break;
				}
			}

			// 接続待ちを解除
			StopListenNetWork();
		}
		else if (menu->selected == 2)
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
					menu->controllers->paddle1 = GameController_Player_Create(&menu->scene->paddle1, &menu->scene->field, &menu->scene->ball, &menu->scene->paddle2, PAD_INPUT_UP, PAD_INPUT_DOWN);
					menu->controllers->paddle2 = GameController_Network_Create(&menu->scene->paddle2, &menu->scene->field, &menu->scene->ball, &menu->scene->paddle1, FALSE, handle);
				}
			}
		}
	}

	return menu->selected == 0;
}

// <メニュー描画>
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
			DrawFormatString((int)GameObject_GetX(&inner, LEFT, -100), (int)(inner.pos.y - 20), COLOR_BLACK, "%d.%d.%d.%d", ip.d1, ip.d2, ip.d3, ip.d4);
		}

		{
			DrawFormatString((int)GameObject_GetX(&inner, LEFT, -100), (int)(inner.pos.y + 20), COLOR_BLACK, "%s PLAY", menu->selected == 0 ? "→" : "　");
			DrawFormatString((int)GameObject_GetX(&inner, LEFT, -100), (int)(inner.pos.y + 60), COLOR_BLACK, "%s PLAY FOR WAIT", menu->selected == 1 ? "→" : "　");
			DrawFormatString((int)GameObject_GetX(&inner, LEFT, -100), (int)(inner.pos.y + 100), COLOR_BLACK, "%s PLAY TO JOIN", menu->selected == 2 ? "→" : "　");
		}
	}
}
