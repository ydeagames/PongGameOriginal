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

// <ゲームの更新処理:オブジェクト:当たり判定> --------------------------
int UpdateGameObjectCollisionBallTopBottom(void);
int UpdateGameObjectCollisionBallLeftRight(void);
int UpdateGameObjectCollisionBallLeftRightScoring(void);
int UpdateGameObjectCollisionBallPaddle(void);
void UpdateGameObjectCollisionPaddleTopBottom(void);

// <ゲームの更新処理:ユーティリティ> -----------------------------------
float GameObject_Paddle_GetBallVelX(float ball_vel_x, float paddle_vel_y);
float GameObject_Paddle_GetVelYFromPaddlePosY(float ball_pos_y, float paddle_pos_y);

// <ゲームの描画処理> --------------------------------------------------
void RenderGameSceneDemo(void);
void RenderGameSceneServe(void);
void RenderGameScenePlay(void);

// <ゲームの描画処理:オブジェクト> -------------------------------------
void RenderGameObjectField(void);
void RenderGameObjectScore(void);
void RenderGameObjectPaddleGuide(void);
void RenderGameObjectPaddle(void);
void RenderGameObjectBall(void);

// <ゲームの描画処理:ユーティリティ> -----------------------------------
void RenderObj(float x, float y, float w, float h, unsigned int color);


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

	// ボール
	g_ball = GameObject_Ball_Create();
	GameObject_Ball_SetPosXDefault(&g_ball);
	GameObject_Ball_SetPosYDefault(&g_ball);
	GameObject_Ball_SetVelDefault(&g_ball);

	// パドル1
	g_paddle1 = GameObject_Paddle_Create();
	GameObject_SetLeft(&g_paddle1, SCREEN_LEFT, 64);
	GameObject_Paddle_SetPosYDefault(&g_paddle1);
	g_paddle1_ctrl = GameController_Bot_Create(&g_paddle1, &g_ball, &g_paddle2);

	// パドル2
	g_paddle2 = GameObject_Paddle_Create();
	GameObject_SetRight(&g_paddle2, SCREEN_RIGHT, 64);
	GameObject_Paddle_SetPosYDefault(&g_paddle2);
	g_paddle2_ctrl = GameController_Player_Create(&g_paddle2, PAD_INPUT_UP, PAD_INPUT_DOWN);

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
	UpdateGameObjectCollisionBallTopBottom();
	UpdateGameObjectCollisionBallLeftRight();
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
	g_paddle1_ctrl.UpdateControl(&g_paddle1_ctrl);
	g_paddle2_ctrl.UpdateControl(&g_paddle2_ctrl);

	// 座標更新
	GameObject_UpdatePosition(&g_ball);
	GameObject_UpdatePosition(&g_paddle1);
	GameObject_UpdatePosition(&g_paddle2);

	// 当たり判定
	UpdateGameObjectCollisionBallTopBottom();
	UpdateGameObjectCollisionBallPaddle();
	UpdateGameObjectCollisionPaddleTopBottom();
}

// <ゲームの更新処理:シーン:プレイ> ------------------------------------
void UpdateGameScenePlay(void)
{
	// 操作
	g_paddle1_ctrl.UpdateControl(&g_paddle1_ctrl);
	g_paddle2_ctrl.UpdateControl(&g_paddle2_ctrl);

	// 座標更新
	GameObject_UpdatePosition(&g_ball);
	GameObject_UpdatePosition(&g_paddle1);
	GameObject_UpdatePosition(&g_paddle2);

	// 当たり判定
	if (UpdateGameObjectCollisionBallTopBottom())
		PlaySoundMem(g_sound_se02, DX_PLAYTYPE_BACK);
	if (UpdateGameObjectCollisionBallLeftRightScoring())
		PlaySoundMem(g_sound_se03, DX_PLAYTYPE_BACK);
	if (UpdateGameObjectCollisionBallPaddle())
		PlaySoundMem(g_sound_se01, DX_PLAYTYPE_BACK);
	UpdateGameObjectCollisionPaddleTopBottom();
}

// <>
void UpdateGameScore()
{
	// 得点処理
	if (g_ball.pos.x < padding_left)
		g_score2++;
	if (padding_right <= g_ball.pos.x)
		g_score1++;

	if (g_score1 >= SCORE_GOAL || g_score2 >= SCORE_GOAL)
	{
		// 初期化ボール座標
		g_ball.pos.x = (float)(SCREEN_CENTER_X);
		// 初期化ボール速度
		g_ball.vel.y = -BALL_VEL_Y;
		g_ball.vel.x = BALL_VEL_X_MIN;

		// シーンをデモに変更
		g_game_state = STATE_DEMO;
	}
	else
		// シーンをサーブに変更
		g_game_state = STATE_SERVE;
}

// <ゲームの更新処理:衝突:パドル×ボール> ------------------------------
int UpdateGameObjectCollisionBallPaddle(void)
{
	return
		GameObject_Paddle_CollisionBall(&g_paddle1, &g_ball) ||
		GameObject_Paddle_CollisionBall(&g_paddle2, &g_ball);
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
	// オブジェクト描画
	RenderGameObjectField();
	RenderGameObjectScore();
	RenderGameObjectBall();
}

// <ゲームの描画処理:シーン:サーブ> -------------------------------------------
void RenderGameSceneServe(void)
{
	// オブジェクト描画
	RenderGameObjectField();
	RenderGameObjectScore();
	RenderGameObjectPaddle();
	RenderGameObjectBall();
}

// <ゲームの描画処理:シーン:プレイ> -------------------------------------------
void RenderGameScenePlay(void)
{
	// オブジェクト描画
	RenderGameObjectField();
	RenderGameObjectScore();
	RenderGameObjectPaddleGuide();
	RenderGameObjectPaddle();
	RenderGameObjectBall();
}

// <ゲームの描画処理:コート> -------------------------------------------
void RenderGameObjectField(void)
{
	// コート表示
	DrawDashedLine(SCREEN_CENTER_X, SCREEN_TOP, SCREEN_CENTER_X, SCREEN_BOTTOM, COLOR_WHITE, 8, 2);
}

// <ゲームの描画処理:スコア> -------------------------------------------
void RenderGameObjectScore(void)
{
	// スコア表示

	// フォントを使用した文字の幅を取得
	int width_score1 = GetDrawFormatStringWidthToHandle(g_font, "%2d", g_score1);

	DrawFormatStringToHandle(SCREEN_CENTER_X - 100 - width_score1, 10, COLOR_WHITE, g_font, "%2d", g_score1);
	DrawFormatStringToHandle(SCREEN_CENTER_X + 100, 10, COLOR_WHITE, g_font, "%2d", g_score2);
}

// <ゲームの描画処理:パドルガイド> -------------------------------------
void RenderGameObjectPaddleGuide(void)
{
	// ガイド表示
	RenderObj(g_paddle1_ctrl.bot_target_pos.x, g_paddle1_ctrl.bot_target_pos.y, PADDLE_WIDTH, PADDLE_HEIGHT, 0x222222);
	//RenderObj(g_paddle2_target_pos.x, g_paddle2_target_pos.y, PADDLE_WIDTH, PADDLE_HEIGHT, 0x222222);
}

// <ゲームの描画処理:パドル> -------------------------------------------
void RenderGameObjectPaddle(void)
{
	// パドル表示
	RenderObj(g_paddle1.pos.x, g_paddle1.pos.y, PADDLE_WIDTH, PADDLE_HEIGHT, COLOR_WHITE);
	RenderObj(g_paddle2.pos.x, g_paddle2.pos.y, PADDLE_WIDTH, PADDLE_HEIGHT, COLOR_WHITE);
}

// <ゲームの描画処理:ボール> -------------------------------------------
void RenderGameObjectBall(void)
{
	// ボール表示
	RenderObj(g_ball.pos.x, g_ball.pos.y, BALL_SIZE, BALL_SIZE, COLOR_WHITE);
}

//----------------------------------------------------------------------
//! @brief オブジェクト描画
//!
//! @param[x] X座標
//! @param[y] Y座標
//! @param[w] 幅
//! @param[h] 高さ
//! @param[color] 色
//!
//! @return なし
//----------------------------------------------------------------------
void RenderObj(float x, float y, float w, float h, unsigned int color)
{
	DrawBox((int)(x - w / 2), (int)(y - h / 2), (int)(x + w / 2), (int)(y + h / 2), color, TRUE);
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
