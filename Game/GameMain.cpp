//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.cpp
//!
//! @brief  Pong Online! オリジナル課題
//!
//! @date   2018/06/13
//!
//! @author GF1 26 山口 寛雅
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================
#include "GameMain.h"
#include "GameObject.h"
#include "GameControllers.h"
#include "GameScore.h"
#include "GameResource.h"
#include "GameMenu.h"
#include "GameScene.h"


// 定数の定義 ==============================================================

// <サーブ待機> --------------------------------------------------------
#define SERVE_WAIT_TIME 2*60


// グローバル変数の宣言 ====================================================

// <シーン> ------------------------------------------------------------
GameScene g_scene;

// <コントローラー> ----------------------------------------------------
GameControllers g_controllers;

// <リソース> ----------------------------------------------------------
GameResource g_resources;

// <メニュー> ----------------------------------------------------------
GameMenu g_menu;


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
	g_scene.game_state = STATE_DEMO;

	// フィールド
	g_scene.field = GameObject_Field_Create();

	// ボール
	g_scene.ball = GameObject_Ball_Create();
	GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);
	GameObject_Ball_SetPosYDefault(&g_scene.ball, &g_scene.field);
	GameObject_Ball_SetVelXDefault(&g_scene.ball);
	GameObject_Ball_SetVelYDefault(&g_scene.ball);

	// パドル1
	g_scene.paddle1 = GameObject_Paddle_Create();
	GameObject_SetX(&g_scene.paddle1, RIGHT, SCREEN_LEFT, 64);
	GameObject_Paddle_SetPosYDefault(&g_scene.paddle1);
	g_controllers.paddle1 = GameController_Bot_Create(&g_scene.paddle1, &g_scene, &g_scene.paddle2);

	// パドル2
	g_scene.paddle2 = GameObject_Paddle_Create();
	GameObject_SetX(&g_scene.paddle2, LEFT, SCREEN_RIGHT, 64);
	GameObject_Paddle_SetPosYDefault(&g_scene.paddle2);
	g_controllers.paddle2 = GameController_Player_Create(&g_scene.paddle2, &g_scene, &g_scene.paddle1, PAD_INPUT_UP, PAD_INPUT_DOWN);
	//g_controllers.paddle2 = GameController_Bot_Create(&g_scene.paddle2, &g_scene, &g_scene.paddle1);

	// リソース
	g_resources = GameResource_Create();

	// 得点
	g_scene.score = GameScore_Create();

	// サーブ待機
	g_scene.counter = 0;

	// メニュー
	g_menu = GameMenu_Create(&g_scene, &g_controllers, &g_resources);
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
	switch (g_scene.game_state)
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
		if (IsButtonDown(PAD_INPUT_10)&&GameMenu_OnPressed(&g_menu))
		{
			// 点数リセット
			GameScore_Clear(&g_scene.score);

			// X座標を画面中央へ戻す
			GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);

			// パドルを初期位置へ
			GameObject_Paddle_SetPosYDefault(&g_scene.paddle1);
			GameObject_Paddle_SetPosYDefault(&g_scene.paddle2);

			// シーンをプレイに変更
			g_scene.game_state = STATE_PLAY;
		}
	}

	// コントローラー更新
	GameController_Update(&g_controllers.paddle1);
	GameController_Update(&g_controllers.paddle2);

	// 座標更新
	GameObject_UpdatePosition(&g_scene.ball);

	// 当たり判定
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, TRUE);
	GameObject_Field_CollisionHorizontal(&g_scene.field, &g_scene.ball, TRUE);

	// メニュー更新
	GameMenu_Update(&g_menu);
}

// <ゲームの更新処理:シーン:サーブ> ------------------------------------
void UpdateGameSceneServe(void)
{
	// 待機&初期化
	g_scene.counter++;

	// 時間経過で
	if (g_scene.counter >= SERVE_WAIT_TIME)
	{
		// X座標を画面中央へ戻す
		GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);

		// シーンをプレイに変更
		g_scene.game_state = STATE_PLAY;

		g_scene.counter = 0;
	}

	// コントローラー更新
	GameController_Update(&g_controllers.paddle1);
	GameController_Update(&g_controllers.paddle2);

	// 操作
	GameController_UpdateControl(&g_controllers.paddle1);
	GameController_UpdateControl(&g_controllers.paddle2);

	// 座標更新
	GameObject_UpdatePosition(&g_scene.ball);
	GameObject_UpdatePosition(&g_scene.paddle1);
	GameObject_UpdatePosition(&g_scene.paddle2);

	// 当たり判定
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, TRUE);
	GameObject_Paddle_CollisionBall(&g_scene.paddle1, &g_scene.ball);
	GameObject_Paddle_CollisionBall(&g_scene.paddle2, &g_scene.ball);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle1, FALSE);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle2, FALSE);
}

// <ゲームの更新処理:シーン:プレイ> ------------------------------------
void UpdateGameScenePlay(void)
{
	// コントローラー更新
	GameController_Update(&g_controllers.paddle1);
	GameController_Update(&g_controllers.paddle2);

	// 操作
	GameController_UpdateControl(&g_controllers.paddle1);
	GameController_UpdateControl(&g_controllers.paddle2);

	// 座標更新
	GameObject_UpdatePosition(&g_scene.ball);
	GameObject_UpdatePosition(&g_scene.paddle1);
	GameObject_UpdatePosition(&g_scene.paddle2);

	// 当たり判定
	if (GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, TRUE))
		PlaySoundMem(g_resources.sound_se02, DX_PLAYTYPE_BACK);
	{
		ObjectSide side = GameObject_Field_CollisionHorizontal(&g_scene.field, &g_scene.ball, FALSE);
		if (side)
		{
			UpdateGameScore(side);
			PlaySoundMem(g_resources.sound_se03, DX_PLAYTYPE_BACK);
		}
	}
	if (GameObject_Paddle_CollisionBall(&g_scene.paddle1, &g_scene.ball) || GameObject_Paddle_CollisionBall(&g_scene.paddle2, &g_scene.ball))
		PlaySoundMem(g_resources.sound_se01, DX_PLAYTYPE_BACK);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle1, FALSE);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle2, FALSE);
}

// <ゲームの更新処理:スコア加算>
void UpdateGameScore(ObjectSide side)
{
	// 得点処理
	GameScore_Add(&g_scene.score, side);

	if (GameScore_IsFinished(&g_scene.score))
	{
		GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);
		GameObject_Ball_SetVelXDefault(&g_scene.ball);
		GameObject_Ball_SetVelYDefault(&g_scene.ball);

		// シーンをデモに変更
		g_scene.game_state = STATE_DEMO;
	}
	else
		// シーンをサーブに変更
		g_scene.game_state = STATE_SERVE;
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
	switch (g_scene.game_state)
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
	GameObject_Field_Render(&g_scene.field);
	// ボール描画
	GameObject_Render(&g_scene.ball, COLOR_WHITE);
	// メニュー描画
	GameMenu_Render(&g_menu);
	// スコア描画
	GameScore_Render(&g_scene.score, &g_scene.field, g_resources.font_pong);
}

// <ゲームの描画処理:シーン:サーブ> -------------------------------------------
void RenderGameSceneServe(void)
{
	// <オブジェクト描画>
	// フィールド描画
	GameObject_Field_Render(&g_scene.field);
	// スコア描画
	GameScore_Render(&g_scene.score, &g_scene.field, g_resources.font_pong);
	// パドル描画
	GameObject_Render(&g_scene.paddle1, COLOR_WHITE);
	GameObject_Render(&g_scene.paddle2, COLOR_WHITE);
	// ボール描画
	GameObject_Render(&g_scene.ball, COLOR_WHITE);
}

// <ゲームの描画処理:シーン:プレイ> -------------------------------------------
void RenderGameScenePlay(void)
{
	// <オブジェクト描画>
	// フィールド描画
	GameObject_Field_Render(&g_scene.field);
	// スコア描画
	GameScore_Render(&g_scene.score, &g_scene.field, g_resources.font_pong);
	// ガイド描画
	GameController_RenderGuide(&g_controllers.paddle1);
	GameController_RenderGuide(&g_controllers.paddle2);
	// パドル描画
	GameObject_Render(&g_scene.paddle1, COLOR_WHITE);
	GameObject_Render(&g_scene.paddle2, COLOR_WHITE);
	// ボール描画
	GameObject_Render(&g_scene.ball, COLOR_WHITE);
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
	GameResource_Delete(&g_resources);
}
