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
float GetVelXFromPaddleVelY(float ball_vel_x, float paddle_vel_y);
float GetVelYFromPaddlePosY(float ball_pos_y, float paddle_pos_y);

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

// <ゲームの更新処理:衝突:ボール×壁上下> ------------------------------
int UpdateGameObjectCollisionBallTopBottom(void)
{
	// ヒットフラグ
	int flag_hit = 0;

	// ボール・上下壁当たり判定
	{
		float padding_top = SCREEN_TOP + (g_ball.size.y / 2);
		float padding_bottom = SCREEN_BOTTOM - (g_ball.size.y / 2);

		// 画面外に出たときの処理
		if (g_ball.pos.y < padding_top || padding_bottom <= g_ball.pos.y)
		{
			g_ball.vel.y *= -1.f;

			flag_hit = 1;
		}

		// 壁にめり込まないように調整
		g_ball.pos.y = ClampF(g_ball.pos.y, padding_top, padding_bottom);
	}

	return flag_hit;
}

// <ゲームの更新処理:衝突:ボール×壁左右> ------------------------------
int UpdateGameObjectCollisionBallLeftRight(void)
{
	// ヒットフラグ
	int flag_hit = 0;

	// ボール・左右壁当たり判定
	{
		float padding_left = SCREEN_LEFT + (g_ball.size.x / 2);
		float padding_right = SCREEN_RIGHT - (g_ball.size.x / 2);

		// 画面外に出たときの処理
		if (g_ball.pos.x < padding_left || padding_right <= g_ball.pos.x)
		{
			g_ball.vel.x *= -1.f;

			flag_hit = 1;
		}

		// 壁にめり込まないように調整
		g_ball.pos.x = ClampF(g_ball.pos.x, padding_left, padding_right);
	}

	return flag_hit;
}

// <ゲームの更新処理:衝突:ボール×壁左右(スコア)> ----------------------
int UpdateGameObjectCollisionBallLeftRightScoring(void)
{
	// ヒットフラグ
	int flag_hit = 0;

	// パドル・左右壁当たり判定
	{
		float padding_left = SCREEN_LEFT + (g_ball.size.x / 2);
		float padding_right = SCREEN_RIGHT - (g_ball.size.x / 2);

		if (g_ball.pos.x < padding_left || padding_right <= g_ball.pos.x)
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

			flag_hit = 1;
		}
	}

	return flag_hit;
}

// <ゲームの更新処理:衝突:パドル×ボール> ------------------------------
int UpdateGameObjectCollisionBallPaddle(void)
{
	// ヒットフラグ
	int flag_hit = 0;

	// ボール・パドル当たり判定
	{
		if (GameObject_IsHit(&g_ball, &g_paddle1))
		{
			g_ball.vel.x = GetVelXFromPaddleVelY(-g_ball.vel.x, g_paddle1.vel.y);

			g_ball.vel.y = GetVelYFromPaddlePosY(g_ball.pos.y, g_paddle1.pos.y);

			if (g_ball.vel.x < 0)
				g_ball.pos.x = g_paddle1.pos.x - g_paddle1.size.x / 2 - g_ball.size.x / 2;
			else
				g_ball.pos.x = g_paddle1.pos.x + g_paddle1.size.x / 2 + g_ball.size.x / 2;

			flag_hit = 1;
		}
		else if (GameObject_IsHit(&g_ball, &g_paddle2))
		{
			g_ball.vel.x = GetVelXFromPaddleVelY(-g_ball.vel.x, g_paddle2.vel.y);

			g_ball.vel.y = GetVelYFromPaddlePosY(g_ball.pos.y, g_paddle2.pos.y);

			if (g_ball.vel.x < 0)
				g_ball.pos.x = g_paddle2.pos.x - PADDLE_WIDTH / 2 - g_ball.size.x / 2;
			else
				g_ball.pos.x = g_paddle2.pos.x + PADDLE_WIDTH / 2 + g_ball.size.x / 2;

			flag_hit = 1;
		}
	}

	return flag_hit;
}

// <ゲームの更新処理:衝突:ボール×パドル> ------------------------------
void UpdateGameObjectCollisionPaddleTopBottom(void)
{
	// パドル・上下壁当たり判定

	float padding_top = SCREEN_TOP + (PADDLE_HEIGHT / 2);
	float padding_bottom = SCREEN_BOTTOM - (PADDLE_HEIGHT / 2);

	// 壁にめり込まないように調整
	g_paddle1.pos.y = ClampF(g_paddle1.pos.y, padding_top, padding_bottom);

	// 壁にめり込まないように調整
	g_paddle2.pos.y = ClampF(g_paddle2.pos.y, padding_top, padding_bottom);
}

//----------------------------------------------------------------------
//! @brief パドルの速度からボールX速度を求める
//!
//! @param[ball_vel_x] 現在のボールのX速度
//! @param[paddle_vel_y] パドルのY速度
//!
//! @return 新しいボールのX速度
//----------------------------------------------------------------------
float GetVelXFromPaddleVelY(float ball_vel_x, float paddle_vel_y)
{
	float ball_vel_diff_x, ball_vel_new_x;

	ball_vel_diff_x = paddle_vel_y / PADDLE_VEL * (BALL_VEL_X_MAX - BALL_VEL_X_MIN);
	if (ball_vel_diff_x < 0)
		ball_vel_diff_x *= -1;
	ball_vel_new_x = ball_vel_diff_x + BALL_VEL_X_MIN;
	if (ball_vel_x < 0)
		ball_vel_new_x *= -1;

	return ball_vel_new_x;
}

//----------------------------------------------------------------------
//! @brief パドルにあたった位置からボールY速度を求める
//!
//! @param[ball_pos_y] ボールのY座標
//! @param[paddle_pos_y] パドルのY座標
//!
//! @return 新しいボールのY速度
//----------------------------------------------------------------------
float GetVelYFromPaddlePosY(float ball_pos_y, float paddle_pos_y)
{
	float range_top = paddle_pos_y - (PADDLE_HEIGHT / 2 - g_ball.size.y / 2);
	float range_bottom = paddle_pos_y + (PADDLE_HEIGHT / 2 - g_ball.size.y / 2);
	float range_height = range_bottom - range_top;

	return ((((ball_pos_y - range_top) / range_height) * 2 - 1)*BALL_VEL_Y);
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
