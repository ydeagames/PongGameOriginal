#include "GameResource.h"

// 定数の定義 ==============================================================

// <フォント> ----------------------------------------------------------
#define FONT_CUSTOM_FILE "Resources\\Fonts\\TheSlavicFont-Regular.ttf"
#define FONT_CUSTOM_NAME "The Slavic Font"
#define FONT_NAME "HGP創英角ｺﾞｼｯｸUB"

// <サウンド> ----------------------------------------------------------
#define SOUND_SE01 "Resources\\Audio\\SE01.ogg"
#define SOUND_SE02 "Resources\\Audio\\SE02.ogg"
#define SOUND_SE03 "Resources\\Audio\\SE03.ogg"

// 関数の定義 ==============================================================

// <<リソース>> --------------------------------------------------

// <リソース作成読み込み>
GameResource GameResource_Create(void)
{
	GameResource res;

	// フォント
	if (AddFontResourceEx(FONT_CUSTOM_FILE, FR_PRIVATE, NULL) > 0)
		res.font = CreateFontToHandle(FONT_CUSTOM_NAME, FONT_SIZE_SCORE, 3, DX_FONTTYPE_ANTIALIASING_4X4);
	else
		res.font = CreateFontToHandle(FONT_NAME, FONT_SIZE_SCORE, 3, DX_FONTTYPE_ANTIALIASING_4X4);

	// サウンド
	res.sound_se01 = LoadSoundMem(SOUND_SE01);
	res.sound_se02 = LoadSoundMem(SOUND_SE02);
	res.sound_se03 = LoadSoundMem(SOUND_SE03);

	return res;
}

// <リソース開放>
void GameResource_Delete(GameResource* res)
{
	// フォント
	DeleteFontToHandle(res->font);
	RemoveFontResourceEx(FONT_CUSTOM_FILE, FR_PRIVATE, NULL);

	// サウンド
	DeleteSoundMem(res->sound_se01);
	DeleteSoundMem(res->sound_se02);
	DeleteSoundMem(res->sound_se03);
}
