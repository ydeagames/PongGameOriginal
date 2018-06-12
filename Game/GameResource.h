#pragma once

// 構造体の宣言 ============================================================

// <ゲームリソース>
typedef struct
{
	// <フォント>
	HFNT font_pong;
	HFNT font_menu;
	HFNT font_note;

	// <サウンド>
	HSND sound_se01;
	HSND sound_se02;
	HSND sound_se03;
} GameResource;

// 定数の定義 ==============================================================

// <フォント> ----------------------------------------------------------

#define FONT_SIZE_PONG 100
#define FONT_SIZE_MENU 20
#define FONT_SIZE_NOTE 12

// 関数の宣言 ==============================================================

// <<リソース>> --------------------------------------------------

// <リソース作成読み込み>
GameResource GameResource_Create(void);

// <リソース開放>
void GameResource_Delete(GameResource* res);
