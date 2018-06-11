//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.cpp
//!
//! @brief  Pong ノーマル課題
//!
//! @date   2018/06/01
//!
//! @author GF1 26 山口 寛雅
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================
#include "GameMain.h"
#include "GameObject.h"
#include "GameController.h"


// 列挙型の定義 ============================================================

// <シーン状態> --------------------------------------------------------
enum GameState
{
	// デモ
	STATE_DEMO,

	// サーブ
	STATE_SERVE,

	// プレイ
	STATE_PLAY
};


// 定数の定義 ==============================================================

// <得点> --------------------------------------------------------------
#define SCORE_GOAL 11		// 最大スコア

// <フォント> ----------------------------------------------------------
#define FONT_CUSTOM_FILE "Resources\\Fonts\\TheSlavicFont-Regular.ttf"
#define FONT_CUSTOM_NAME "The Slavic Font"
#define FONT_NAME "HGP創英角ｺﾞｼｯｸUB"
#define FONT_SIZE_SCORE 100

// <サウンド> ----------------------------------------------------------
#define SOUND_SE01 "Resources\\Audio\\SE01.ogg"
#define SOUND_SE02 "Resources\\Audio\\SE02.ogg"
#define SOUND_SE03 "Resources\\Audio\\SE03.ogg"

// <サーブ待機> --------------------------------------------------------
#define SERVE_WAIT_TIME 2*60


// グローバル変数の宣言 ====================================================

// <入力> --------------------------------------------------------------
int g_input_state;

// <シーン状態> --------------------------------------------------------
int g_game_state;

// <フィールド> --------------------------------------------------------
GameObject g_field;

// <ボール> ------------------------------------------------------------
GameObject g_ball;

// <パドル1> -----------------------------------------------------------
GameObject g_paddle1;
GameController g_paddle1_ctrl;

// <パドル2> -----------------------------------------------------------
GameObject g_paddle2;
GameController g_paddle2_ctrl;

// <得点> --------------------------------------------------------------
int g_score1;
int g_score2;

// <フォント> ----------------------------------------------------------
HFNT g_font;

// <サウンド> ----------------------------------------------------------
HSND g_sound_se01;
HSND g_sound_se02;
HSND g_sound_se03;

// <サーブ待機> --------------------------------------------------------
int g_counter;


// 関数の宣言 ==============================================================

// <ゲームの更新処理:シーン> -------------------------------------------
void UpdateGameSceneDemo(void);
void UpdateGameSceneServe(void);
void UpdateGameScenePlay(void);

// <ゲームの描画処理> --------------------------------------------------
void RenderGameSceneDemo(void);
void RenderGameSceneServe(void);
void RenderGameScenePlay(void);

void UpdateGameScore(ObjectSide side);

// <ゲームの描画処理:オブジェクト> -------------------------------------
void RenderGameObjectScore(void);
void RenderGameObjectPaddleGuide(void);
void RenderGameObjectPaddle(void);
void RenderGameObjectBall(void);

// 関数の定義 ==============================================================

//----------------------------------------------------------------------
//! @brief ゲームの初期化処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void InitializeGame(void)
{
	// シーン状態
	g_game_state = STATE_DEMO;

	// フィールド
	g_field = GameObject_Field_Create();

	// ボール
	g_ball = GameObject_Ball_Create();
	GameObject_Ball_SetPosXDefault(&g_ball);
	GameObject_Ball_SetPosYDefault(&g_ball);
	GameObject_Ball_SetVelXDefault(&g_ball);
	GameObject_Ball_SetVelYDefault(&g_ball);

	// パドル1
	g_paddle1 = GameObject_Paddle_Create();
	GameObject_SetX(&g_paddle1, RIGHT, SCREEN_LEFT, 64);
	GameObject_Paddle_SetPosYDefault(&g_paddle1);
	g_paddle1_ctrl = GameController_Bot_Create(&g_paddle1, &g_ball, &g_paddle2);

	// パドル2
	g_paddle2 = GameObject_Paddle_Create();
	GameObject_SetX(&g_paddle2, LEFT, SCREEN_RIGHT, 64);
	GameObject_Paddle_SetPosYDefault(&g_paddle2);
	g_paddle2_ctrl = GameController_Player_Create(&g_paddle2, &g_ball, &g_paddle1, PAD_INPUT_UP, PAD_INPUT_DOWN);

	// 得点
	g_score1 = 0;
	g_score2 = 0;

	// フォント
	if (AddFontResourceEx(FONT_CUSTOM_FILE, FR_PRIVATE, NULL) > 0)
		g_font = CreateFontToHandle(FONT_CUSTOM_NAME, FONT_SIZE_SCORE, 3, DX_FONTTYPE_ANTIALIASING_4X4);
	else
		g_font = CreateFontToHandle(FONT_NAME, FONT_SIZE_SCORE, 3, DX_FONTTYPE_ANTIALIASING_4X4);

	// サウンド
	g_sound_se01 = LoadSoundMem(SOUND_SE01);
	g_sound_se02 = LoadSoundMem(SOUND_SE02);
	g_sound_se03 = LoadSoundMem(SOUND_SE03);

	// サーブ待機
	g_counter = 0;
}


//----------------------------------------------------------------------
//! @brief ゲームの更新処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void UpdateGame(void)
{
	// キーボード取得
	g_input_state = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	switch (g_game_state)
	{
	case STATE_DEMO:
		UpdateGameSceneDemo();
		break;
	case STATE_SERVE:
		UpdateGameSceneServe();
		break;
	case STATE_PLAY:
		UpdateGameScenePlay();
		break;
	}
}

// <ゲームの更新処理:シーン:デモ> --------------------------------------
void UpdateGameSceneDemo(void)
{
	// 待機&初期化
	{
		// 入力されたら
		if (g_input_state & PAD_INPUT_10)
		{
			// 点数リセット
			g_score1 = 0;
			g_score2 = 0;

			// X座標を画面中央へ戻す
			GameObject_Ball_SetPosXDefault(&g_ball);

			// パドルを初期位置へ
			GameObject_Paddle_SetPosYDefault(&g_paddle1);
			GameObject_Paddle_SetPosYDefault(&g_paddle2);

			// シーンをプレイに変更
			g_game_state = STATE_PLAY;
		}
	}

	// 座標更新
	GameObject_UpdatePosition(&g_ball);

	// 当たり判定
	GameObject_Field_CollisionVertical(&g_field, &g_ball, TRUE);
	GameObject_Field_CollisionHorizontal(&g_field, &g_ball, TRUE);
}

// <ゲームの更新処理:シーン:サーブ> ------------------------------------
void UpdateGameSceneServe(void)
{
	// 待機&初期化

	g_counter++;

	// 時間経過で
	if (g_counter >= SERVE_WAIT_TIME)
	{
		// X座標を画面中央へ戻す
		GameObject_Ball_SetPosXDefault(&g_ball);

		// シーンをプレイに変更
		g_game_state = STATE_PLAY;

		g_counter = 0;
	}

	// 操作
	GameController_Update(&g_paddle1_ctrl);
	GameController_Update(&g_paddle2_ctrl);

	// 座標更新
	GameObject_UpdatePosition(&g_ball);
	GameObject_UpdatePosition(&g_paddle1);
	GameObject_UpdatePosition(&g_paddle2);

	// 当たり判定
	GameObject_Field_CollisionVertical(&g_field, &g_ball, TRUE);
	GameObject_Paddle_CollisionBall(&g_paddle1, &g_ball);
	GameObject_Paddle_CollisionBall(&g_paddle2, &g_ball);
	GameObject_Field_CollisionVertical(&g_field, &g_paddle1, FALSE);
	GameObject_Field_CollisionVertical(&g_field, &g_paddle2, FALSE);
}

// <ゲームの更新処理:シーン:プレイ> ------------------------------------
void UpdateGameScenePlay(void)
{
	// 操作
	GameController_Update(&g_paddle1_ctrl);
	GameController_Update(&g_paddle2_ctrl);

	// 座標更新
	GameObject_UpdatePosition(&g_ball);
	GameObject_UpdatePosition(&g_paddle1);
	GameObject_UpdatePosition(&g_paddle2);

	// 当たり判定
	if (GameObject_Field_CollisionVertical(&g_field, &g_ball, TRUE))
		PlaySoundMem(g_sound_se02, DX_PLAYTYPE_BACK);
	{
		ObjectSide side = GameObject_Field_CollisionHorizontal(&g_field, &g_ball, FALSE);
		if (side)
		{
			UpdateGameScore(side);
			PlaySoundMem(g_sound_se03, DX_PLAYTYPE_BACK);
		}
	}
	if (GameObject_Paddle_CollisionBall(&g_paddle1, &g_ball) || GameObject_Paddle_CollisionBall(&g_paddle2, &g_ball))
		PlaySoundMem(g_sound_se01, DX_PLAYTYPE_BACK);
	GameObject_Field_CollisionVertical(&g_field, &g_paddle1, FALSE);
	GameObject_Field_CollisionVertical(&g_field, &g_paddle2, FALSE);
}

// <ゲームの更新処理:スコア加算>
void UpdateGameScore(ObjectSide side)
{
	// 得点処理
	switch (side)
	{
	case LEFT:
		g_score2++;
		break;
	case RIGHT:
		g_score1++;
		break;
	}

	if (g_score1 >= SCORE_GOAL || g_score2 >= SCORE_GOAL)
	{
		GameObject_Ball_SetPosXDefault(&g_ball);
		GameObject_Ball_SetVelXDefault(&g_ball);
		GameObject_Ball_SetVelYDefault(&g_ball);

		// シーンをデモに変更
		g_game_state = STATE_DEMO;
	}
	else
		// シーンをサーブに変更
		g_game_state = STATE_SERVE;
}

//----------------------------------------------------------------------
//! @brief ゲームの描画処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void RenderGame(void)
{
	switch (g_game_state)
	{
	case STATE_DEMO:
		RenderGameSceneDemo();
		break;
	case STATE_SERVE:
		RenderGameSceneServe();
		break;
	case STATE_PLAY:
		RenderGameScenePlay();
		break;
	}
}

// <ゲームの描画処理:シーン:デモ> ---------------------------------------------
void RenderGameSceneDemo(void)
{
	// <オブジェクト描画>
	// フィールド描画
	GameObject_Field_Render(&g_field);
	// スコア描画
	RenderGameObjectScore();
	// ボール描画
	GameObject_Render(&g_ball, COLOR_WHITE);
}

// <ゲームの描画処理:シーン:サーブ> -------------------------------------------
void RenderGameSceneServe(void)
{
	// <オブジェクト描画>
	// フィールド描画
	GameObject_Field_Render(&g_field);
	// スコア描画
	RenderGameObjectScore();
	// パドル描画
	GameObject_Render(&g_paddle1, COLOR_WHITE);
	GameObject_Render(&g_paddle2, COLOR_WHITE);
	// ボール描画
	GameObject_Render(&g_ball, COLOR_WHITE);
}

// <ゲームの描画処理:シーン:プレイ> -------------------------------------------
void RenderGameScenePlay(void)
{
	// <オブジェクト描画>
	// フィールド描画
	GameObject_Field_Render(&g_field);
	// スコア描画
	RenderGameObjectScore();
	RenderGameObjectPaddleGuide();
	// パドル描画
	GameObject_Render(&g_paddle1, COLOR_WHITE);
	GameObject_Render(&g_paddle2, COLOR_WHITE);
	// ボール描画
	GameObject_Render(&g_ball, COLOR_WHITE);
}

// <ゲームの描画処理:スコア> -------------------------------------------
void RenderGameObjectScore(void)
{
	// スコア描画

	// フォントを使用した文字の幅を取得
	int width_score1 = GetDrawFormatStringWidthToHandle(g_font, "%2d", g_score1);

	DrawFormatStringToHandle(SCREEN_CENTER_X - 100 - width_score1, 10, COLOR_WHITE, g_font, "%2d", g_score1);
	DrawFormatStringToHandle(SCREEN_CENTER_X + 100, 10, COLOR_WHITE, g_font, "%2d", g_score2);
}

// <ゲームの描画処理:パドルガイド> -------------------------------------
void RenderGameObjectPaddleGuide(void)
{
	// ガイド表示
	GameObject paddle1_target = g_paddle1;
	GameObject paddle2_target = g_paddle2;
	paddle1_target.pos = g_paddle1_ctrl.target_pos;
	paddle2_target.pos = g_paddle2_ctrl.target_pos;
	GameObject_Render(&paddle1_target, 0x222222);
	GameObject_Render(&paddle2_target, 0x222222);
}

//----------------------------------------------------------------------
//! @brief ゲームの終了処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void FinalizeGame(void)
{
	// フォント
	DeleteFontToHandle(g_font);
	RemoveFontResourceEx(FONT_CUSTOM_FILE, FR_PRIVATE, NULL);

	// サウンド
	DeleteSoundMem(g_sound_se01);
	DeleteSoundMem(g_sound_se02);
	DeleteSoundMem(g_sound_se03);
}
