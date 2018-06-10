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
#include "BallObject.h"
#include "PaddleObject.h"


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
#define SCORE_GOAL 11

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
BallObject g_ball;

// <パドル1> -----------------------------------------------------------
PaddleObject g_paddle1;
Vec2 g_paddle1_target_pos;

// <パドル2> -----------------------------------------------------------
PaddleObject g_paddle2;
Vec2 g_paddle2_target_pos;

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

// <ゲームの更新処理:操作:座標> ----------------------------------------
void UpdateGameControlPaddle1(void);
void UpdateGameControlPaddle2(void);
void UpdateGameControlPaddlePlayer1(void);
void UpdateGameControlPaddlePlayer2(void);
void UpdateGameControlPaddleBot1(void);
void UpdateGameControlPaddleBot2(void);

// <ゲームの更新処理:オブジェクト:座標> --------------------------------
void UpdateGameObjectPositionBall(void);
void UpdateGameObjectPositionPaddle(void);
void UpdateGameObjectPositionPaddleTarget(void);

// <ゲームの更新処理:オブジェクト:当たり判定> --------------------------
int UpdateGameObjectCollisionBallTopBottom(void);
int UpdateGameObjectCollisionBallLeftRight(void);
int UpdateGameObjectCollisionBallLeftRightScoring(void);
int UpdateGameObjectCollisionBallPaddle(void);
void UpdateGameObjectCollisionPaddleTopBottom(void);

// <ゲームの更新処理:ユーティリティ> -----------------------------------
float getTargetY(float paddle_enemy_pos_x, float paddle_myself_pos_x, int k);

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
	g_ball = {
		{
			(float)(SCREEN_CENTER_X),
			(float)(SCREEN_CENTER_Y),
		},
		{
			g_ball.BallObject::BALL_VEL_X_MIN,
			-g_ball.BallObject::BALL_VEL_Y,
		},
	};

	// パドル1
	g_paddle1 = {
		{
			(float)(SCREEN_LEFT + (PaddleObject::PaddleObject::PADDLE_WIDTH / 2 + 64)),
			(float)(SCREEN_BOTTOM - 30),
		},
		{
			0.f,
			0.f,
		},
	};
	g_paddle1_target_pos = g_paddle1.pos;

	// パドル2
	g_paddle2 = {
		{
			(float)(SCREEN_RIGHT - (PaddleObject::PaddleObject::PADDLE_WIDTH / 2 + 64)),
			(float)(SCREEN_BOTTOM - 30),
		},
		{
			0.f,
			0.f,
		},
	};
	g_paddle2_target_pos = g_paddle2.pos;

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
			g_ball.pos.x = (float)(SCREEN_CENTER_X);

			// パドルを初期位置へ
			g_paddle1.pos.y = (float)(SCREEN_BOTTOM - 30);
			g_paddle2.pos.y = (float)(SCREEN_BOTTOM - 30);

			// シーンをプレイに変更
			g_game_state = STATE_PLAY;
		}
	}

	// 座標更新
	UpdateGameObjectPositionBall();

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
		g_ball.pos.x = (float)(SCREEN_CENTER_X);

		// シーンをプレイに変更
		g_game_state = STATE_PLAY;

		g_counter = 0;
	}

	// 操作
	UpdateGameControlPaddle1();
	UpdateGameControlPaddle2();

	// 座標更新
	UpdateGameObjectPositionBall();
	UpdateGameObjectPositionPaddle();

	// 当たり判定
	UpdateGameObjectCollisionBallTopBottom();
	UpdateGameObjectCollisionBallPaddle();
	UpdateGameObjectCollisionPaddleTopBottom();
}

// <ゲームの更新処理:シーン:プレイ> ------------------------------------
void UpdateGameScenePlay(void)
{
	// ターゲット座標更新
	UpdateGameObjectPositionPaddleTarget();

	// 操作
	UpdateGameControlPaddle1();
	UpdateGameControlPaddle2();

	// 座標更新
	UpdateGameObjectPositionBall();
	UpdateGameObjectPositionPaddle();

	// 当たり判定
	if (UpdateGameObjectCollisionBallTopBottom())
		PlaySoundMem(g_sound_se02, DX_PLAYTYPE_BACK);
	if (UpdateGameObjectCollisionBallLeftRightScoring())
		PlaySoundMem(g_sound_se03, DX_PLAYTYPE_BACK);
	if (UpdateGameObjectCollisionBallPaddle())
		PlaySoundMem(g_sound_se01, DX_PLAYTYPE_BACK);
	UpdateGameObjectCollisionPaddleTopBottom();
}

// <ゲームの更新処理:操作:パドル1> -------------------------------------
void UpdateGameControlPaddle1(void)
{
	// 操作

	//UpdateGameControlPaddlePlayer1();
	UpdateGameControlPaddleBot1();
}

// <ゲームの更新処理:操作:パドル2> -------------------------------------
void UpdateGameControlPaddle2(void)
{
	// 操作

	UpdateGameControlPaddlePlayer2();
	//UpdateGameControlPaddleBot2();
}

// <ゲームの更新処理:操作:プレイヤー1> ---------------------------------
void UpdateGameControlPaddlePlayer1(void)
{
	// キー入力でパドル1を操作
	g_paddle1.vel.y = 0.f;
	if (g_input_state & (PAD_INPUT_8 | PAD_INPUT_4))
		g_paddle1.vel.y += -PaddleObject::PaddleObject::PADDLE_VEL;
	if (g_input_state & (PAD_INPUT_5 | PAD_INPUT_1))
		g_paddle1.vel.y += PaddleObject::PaddleObject::PADDLE_VEL;
}

// <ゲームの更新処理:操作:プレイヤー2> ---------------------------------
void UpdateGameControlPaddlePlayer2(void)
{
	// キー入力でパドル2を操作
	g_paddle2.vel.y = 0.f;
	if (g_input_state & PAD_INPUT_UP)
		g_paddle2.vel.y += -PaddleObject::PaddleObject::PADDLE_VEL;
	if (g_input_state & PAD_INPUT_DOWN)
		g_paddle2.vel.y += PaddleObject::PaddleObject::PADDLE_VEL;
}

// <ゲームの更新処理:操作:Bot1> ----------------------------------------
void UpdateGameControlPaddleBot1(void)
{
	// Botが動き始めるしきい値
	float padding = 260 * BallObject::BallObject::BALL_VEL_X_MIN / PaddleObject::PaddleObject::PADDLE_VEL;

	g_paddle1.vel.y = 0.f;

	// 自分向きかつしきい値より近かったら動く
	if (g_ball.vel.x < 0 && g_ball.pos.x < g_paddle1.pos.x + padding)
	{
		// Botがパドル1を操作
		float target1_pos_y = g_paddle1_target_pos.y;

		// 死んだら中央に戻る
		if (g_ball.pos.x < SCREEN_LEFT)
			target1_pos_y = ClampF(g_paddle1.pos.y, SCREEN_CENTER_Y - 80.f, SCREEN_CENTER_Y + 80.f);

		// Botが移動できる幅を制限
		//target1_pos_y = ClampF(target1_pos_y, SCREEN_TOP + 50, SCREEN_BOTTOM - 50);

		if (g_paddle1.pos.y - target1_pos_y > 1.2f * PaddleObject::PADDLE_VEL)
			g_paddle1.vel.y += -PaddleObject::PADDLE_VEL;
		else if (g_paddle1.pos.y - target1_pos_y < -1.2f * PaddleObject::PADDLE_VEL)
			g_paddle1.vel.y += PaddleObject::PADDLE_VEL;
	}
}

// <ゲームの更新処理:操作:Bot2> ----------------------------------------
void UpdateGameControlPaddleBot2(void)
{
	// Botが動き始めるしきい値
	float padding = 260 * BallObject::BALL_VEL_X_MIN / PaddleObject::PADDLE_VEL;

	g_paddle2.vel.y = 0.f;

	// 自分向きかつしきい値より近かったら動く
	if (g_ball.vel.x > 0 && g_ball.pos.x > g_paddle2.pos.x - padding)
	{
		// Botがパドル2を操作
		float target2_pos_y = g_paddle2_target_pos.y;

		// 死んだら中央に戻る
		if (g_ball.pos.x > SCREEN_RIGHT)
			target2_pos_y = ClampF(g_paddle2.pos.y, SCREEN_CENTER_Y - 80.f, SCREEN_CENTER_Y + 80.f);

		// Botが移動できる幅を制限
		//target2_pos_y = ClampF(target2_pos_y, SCREEN_TOP + 50, SCREEN_BOTTOM - 50);

		if (g_paddle2.pos.y - target2_pos_y > 1.2f * PaddleObject::PADDLE_VEL)
			g_paddle2.vel.y += -PaddleObject::PADDLE_VEL;
		else if (g_paddle2.pos.y - target2_pos_y < -1.2f * PaddleObject::PADDLE_VEL)
			g_paddle2.vel.y += PaddleObject::PADDLE_VEL;
	}
}

// <ゲームの更新処理:座標:ボール> --------------------------------------
void UpdateGameObjectPositionBall(void)
{
	// 座標更新

	// posにvelを足す
	g_ball.UpdatePosition();
}

// <ゲームの更新処理:座標:パドル> --------------------------------------
void UpdateGameObjectPositionPaddle(void)
{
	// 座標更新

	// posにvelを足す
	g_paddle1.UpdatePosition();

	// posにvelを足す
	g_paddle2.UpdatePosition();
}

// <ゲームの更新処理:座標:パドルターゲット> ----------------------------
void UpdateGameObjectPositionPaddleTarget(void)
{
	// ターゲット計算
	g_paddle1_target_pos.y = getTargetY(g_paddle1.pos.x, g_paddle2.pos.x, 1);
	g_paddle2_target_pos.y = getTargetY(g_paddle2.pos.x, g_paddle1.pos.x, -1);
}

// <ゲームの更新処理:衝突:ボール×壁上下> ------------------------------
int UpdateGameObjectCollisionBallTopBottom(void)
{
	// ヒットフラグ
	int flag_hit = 0;

	// ボール・上下壁当たり判定
	{
		float padding_top = SCREEN_TOP + (BallObject::BALL_SIZE / 2);
		float padding_bottom = SCREEN_BOTTOM - (BallObject::BALL_SIZE / 2);

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
		float padding_left = SCREEN_LEFT + (BallObject::BALL_SIZE / 2);
		float padding_right = SCREEN_RIGHT - (BallObject::BALL_SIZE / 2);

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
		float padding_left = SCREEN_LEFT + (BallObject::BALL_SIZE / 2);
		float padding_right = SCREEN_RIGHT - (BallObject::BALL_SIZE / 2);

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
				g_ball.vel.y = -BallObject::BALL_VEL_Y;
				g_ball.vel.x = BallObject::BALL_VEL_X_MIN;

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
	// ボール・パドル当たり判定
	return
		g_paddle1.Collision(g_ball) ||
		g_paddle2.Collision(g_ball);
}

// <ゲームの更新処理:衝突:ボール×パドル> ------------------------------
void UpdateGameObjectCollisionPaddleTopBottom(void)
{
	// パドル・上下壁当たり判定

	float padding_top = SCREEN_TOP + (PaddleObject::PADDLE_HEIGHT / 2);
	float padding_bottom = SCREEN_BOTTOM - (PaddleObject::PADDLE_HEIGHT / 2);

	// 壁にめり込まないように調整
	g_paddle1.pos.y = ClampF(g_paddle1.pos.y, padding_top, padding_bottom);

	// 壁にめり込まないように調整
	g_paddle2.pos.y = ClampF(g_paddle2.pos.y, padding_top, padding_bottom);
}

//----------------------------------------------------------------------
//! @brief Bot AI
//! ボール着弾点予想アルゴリズム
//!
//! @param[paddle_enemy_pos_x] 敵パドルのX座標
//! @param[paddle_myself_pos_x] 自パドルのX座標
//! @param[k] 自パドルから放つボールの向き (1: 右向き, 2: 左向き)
//!
//! @return 予測Y座標
//----------------------------------------------------------------------
float getTargetY(float paddle_enemy_pos_x, float paddle_myself_pos_x, int k)
{
	// ボール、パドルサイズを考慮した敵パドル、自パドルのX座標
	float enemy_pos_x, myself_pos_x;
	// ボールから何pxで自パドルに到着するのかを算出
	float length_x, length_y;
	// ボールの基準位置
	float ball_base_y;
	// 目標の座標
	float ball_absolute_y;
	// ボールの移動可能範囲
	float screen_top_y, screen_bottom_y, screen_height;
	// 目標の画面内座標
	float target_pos_y;

	// ボール、パドルサイズを考慮した敵パドル、自パドルのX座標
	{
		enemy_pos_x = paddle_myself_pos_x - k * (BallObject::BALL_SIZE / 2 + PaddleObject::PADDLE_WIDTH / 2);
		myself_pos_x = paddle_enemy_pos_x + k * (BallObject::BALL_SIZE / 2 + PaddleObject::PADDLE_WIDTH / 2);
	}

	{
		// ボールから何pxで自パドルに到着するのかを算出
		{
			length_x = 0;
			if (k*g_ball.vel.x > 0)
			{
				// ボールが右に進んでいるとき 自分→敵→自分
				// ボール〜敵までの距離 (行き)
				length_x += k * (enemy_pos_x - g_ball.pos.x);
				// 敵〜自分の距離 (帰り)
				length_x += k * (enemy_pos_x - myself_pos_x);
			}
			else
			{
				// ボールが左に進んでいるとき 敵→自分
				// ボール〜自分までの距離
				length_x += k * (g_ball.pos.x - myself_pos_x);
			}
		}

		// 跳ね返りを無視したとき、自パドルに到着時ボールのY座標
		{
			length_y = length_x / g_ball.vel.x * g_ball.vel.y;
			if (length_y < 0)
				length_y *= -1; // 絶対値
		}
	}

	// ボールのY座標
	{
		ball_base_y = g_ball.pos.y;
		if (g_ball.vel.y < 0)
			ball_base_y *= -1; // 速度が上向きのとき、上にターゲットが存在する
	}

	// ターゲットの算出
	ball_absolute_y = ball_base_y + length_y;

	{
		// ボールサイズを考慮した上下の壁
		screen_bottom_y = SCREEN_BOTTOM - BallObject::BALL_SIZE / 2;
		screen_top_y = SCREEN_TOP + BallObject::BALL_SIZE / 2;
		// ボールサイズを考慮したボールの移動範囲
		screen_height = screen_bottom_y - screen_top_y;
	}

	// 画面の範囲外だったらheightずつ足して範囲内にする
	// このとき、操作の回数を数える
	{
		int count = 0;
		float pos_y_loop = ball_absolute_y;
		while (pos_y_loop < screen_top_y)
		{
			pos_y_loop += screen_height;
			count++;
		}
		while (pos_y_loop > screen_bottom_y)
		{
			pos_y_loop -= screen_height;
			count++;
		}

		// 範囲を調整する回数が奇数であればY軸を中心に反転させる
		if (count % 2 == 0)
			target_pos_y = pos_y_loop;
		else
			target_pos_y = screen_bottom_y - pos_y_loop;
	}

	return target_pos_y;
}

//----------------------------------------------------------------------
//! @brief パドルの速度からボールX速度を求める
//!
//! @param[ball_vel_x] 現在のボールのX速度
//! @param[PaddleObject::PADDLE_VEL_y] パドルのY速度
//!
//! @return 新しいボールのX速度
//----------------------------------------------------------------------
float GetVelXFromPaddleVelY(float ball_vel_x, float paddle_vel_y)
{
	float ball_vel_diff_x, ball_vel_new_x;

	ball_vel_diff_x = paddle_vel_y / PaddleObject::PADDLE_VEL * (BallObject::BALL_VEL_X_MAX - BallObject::BALL_VEL_X_MIN);
	if (ball_vel_diff_x < 0)
		ball_vel_diff_x *= -1;
	ball_vel_new_x = ball_vel_diff_x + BallObject::BALL_VEL_X_MIN;
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
	float range_top = paddle_pos_y - (PaddleObject::PADDLE_HEIGHT / 2 - BallObject::BALL_SIZE / 2);
	float range_bottom = paddle_pos_y + (PaddleObject::PADDLE_HEIGHT / 2 - BallObject::BALL_SIZE / 2);
	float range_height = range_bottom - range_top;

	return ((((ball_pos_y - range_top) / range_height) * 2 - 1)*BallObject::BALL_VEL_Y);
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
	RenderObj(g_paddle1_target_pos.x, g_paddle1_target_pos.y, PaddleObject::PADDLE_WIDTH, PaddleObject::PADDLE_HEIGHT, 0x222222);
	RenderObj(g_paddle2_target_pos.x, g_paddle2_target_pos.y, PaddleObject::PADDLE_WIDTH, PaddleObject::PADDLE_HEIGHT, 0x222222);
}

// <ゲームの描画処理:パドル> -------------------------------------------
void RenderGameObjectPaddle(void)
{
	// パドル表示
	RenderObj(g_paddle1.pos.x, g_paddle1.pos.y, PaddleObject::PADDLE_WIDTH, PaddleObject::PADDLE_HEIGHT, COLOR_WHITE);
	RenderObj(g_paddle2.pos.x, g_paddle2.pos.y, PaddleObject::PADDLE_WIDTH, PaddleObject::PADDLE_HEIGHT, COLOR_WHITE);
}

// <ゲームの描画処理:ボール> -------------------------------------------
void RenderGameObjectBall(void)
{
	// ボール表示
	RenderObj(g_ball.pos.x, g_ball.pos.y, BallObject::BALL_SIZE, BallObject::BALL_SIZE, COLOR_WHITE);
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
