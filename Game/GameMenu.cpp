#include "GameMenu.h"
#include "GameMain.h"

// 関数の定義 ==============================================================

// <<メニュー>> --------------------------------------------------------

// <メニュー作成>
GameMenu GameMenu_Create(GameObject* field)
{
	return { field };
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

// <メニュー描画>
void GameMenu_Render(GameMenu* menu, GameResource* res)
{
	{
		GameObject inner = *menu->field;
		inner.size.x -= 80;
		inner.size.y -= 160;
		inner.pos.y += 40;

		{
			SetDrawBlendMode(DX_BLENDMODE_INVDESTCOLOR, 255);
			GameObject_Render(menu->field, 0x222222);
			GameObject_Render(&inner, COLOR_WHITE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		DrawFormatStringToHandle(
			(int)(menu->field->pos.x - GetDrawFormatStringWidthToHandle(res->font, GAME_TITLE) / 2),
			(int)(GameObject_GetY(&inner, TOP, -20)),
			COLOR_BLACK, res->font, GAME_TITLE
		);

		{
			IPDATA ip;
			GetMyIPAddress(&ip);
			DrawFormatString((int)GameObject_GetX(&inner, LEFT, -100), (int)(inner.pos.y - 60), COLOR_BLACK, "%d.%d.%d.%d", ip.d1, ip.d2, ip.d3, ip.d4);
		}

		{
			DrawFormatString((int)GameObject_GetX(&inner, LEFT, -100), (int)(inner.pos.y - 20), COLOR_BLACK, "%s PLAY SOLO", menu->selected == 0 ? "→" : "　");
			DrawFormatString((int)GameObject_GetX(&inner, LEFT, -100), (int)(inner.pos.y + 20), COLOR_BLACK, "%s PLAY FOR WAIT", menu->selected == 1 ? "→" : "　");
			DrawFormatString((int)GameObject_GetX(&inner, LEFT, -100), (int)(inner.pos.y + 60), COLOR_BLACK, "%s PLAY TO JOIN", menu->selected == 2 ? "→" : "　");
		}
	}
}
