#include "GameController.h"

// 関数の宣言 ==============================================================

void GameController_Player_Update(GameController* ctrl);
void GameController_Player_UpdateControl(GameController* ctrl);
void GameController_Bot_Update(GameController* ctrl);
void GameController_Bot_UpdateControl(GameController* ctrl);
void GameController_Network_Update(GameController* ctrl);
void GameController_Network_UpdateControl(GameController* ctrl);

inline float abs(float a)
{
	return a > 0 ? a : -a;
}

// 関数の定義 ==============================================================

// <<コントローラー>> --------------------------------------------------

// <コントローラー作成>
GameController GameController_Create(GameObject* object, void(*updateFunc)(GameController*), void(*updateCtrlFunc)(GameController*), GameScene* scene, GameObject* enemy)
{
	return { object, updateFunc, updateCtrlFunc, object->pos, scene, enemy };
}

// <コントローラー更新>
void GameController_Update(GameController* ctrl)
{
	ctrl->Update(ctrl);
}

// <コントローラー操作更新>
void GameController_UpdateControl(GameController* ctrl)
{
	ctrl->target_pos.y = GameController_GetTargetY(&ctrl->scene->field, &ctrl->scene->ball, ctrl->object, ctrl->enemy);

	ctrl->UpdateControl(ctrl);
}

// <ボール着弾点予想アルゴリズム>
float GameController_GetTargetY(GameObject* field, GameObject* ball, GameObject* paddle_myself, GameObject* paddle_enemy)
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
	// 自パドルから放つボールの向き (1: 右向き, -1: 左向き)
	int k = (paddle_myself->pos.x < paddle_enemy->pos.x) ? 1 : -1;

	// ボール、パドルサイズを考慮した敵パドル、自パドルのX座標
	{
		myself_pos_x = paddle_myself->pos.x - k * (ball->size.x / 2 + paddle_myself->size.x / 2);
		enemy_pos_x = paddle_enemy->pos.x + k * (ball->size.x / 2 + paddle_enemy->size.x / 2);
	}

	{
		// ボールから何pxで自パドルに到着するのかを算出
		{
			length_x = 0;
			if (k*ball->vel.x > 0)
			{
				// ボールが右に進んでいるとき 自分→敵→自分
				// ボール〜敵までの距離 (行き)
				length_x += k * (enemy_pos_x - ball->pos.x);
				// 敵〜自分の距離 (帰り)
				length_x += k * (enemy_pos_x - myself_pos_x);
			}
			else
			{
				// ボールが左に進んでいるとき 敵→自分
				// ボール〜自分までの距離
				length_x += k * (ball->pos.x - myself_pos_x);
			}
		}

		// 跳ね返りを無視したとき、自パドルに到着時ボールのY座標
		{
			length_y = length_x / ball->vel.x * ball->vel.y;
			if (length_y < 0)
				length_y *= -1; // 絶対値
		}
	}

	{
		// ボールサイズを考慮した上下の壁
		screen_top_y = GameObject_OffsetY(ball, BOTTOM, GameObject_GetY(field, TOP));
		screen_bottom_y = GameObject_OffsetY(ball, TOP, GameObject_GetY(field, BOTTOM));
		// ボールサイズを考慮したボールの移動範囲
		screen_height = screen_bottom_y - screen_top_y;
	}

	// ボールのY座標
	{
		ball_base_y = ball->pos.y - screen_top_y;
		if (ball->vel.y < 0)
			ball_base_y *= -1; // 速度が上向きのとき、上にターゲットが存在する
	}

	// ターゲットの算出
	ball_absolute_y = ball_base_y + length_y;

	// 画面の範囲外だったらheightずつ足して範囲内にする
	// このとき、操作の回数を数える
	{
		int count = 0;
		float pos_y_loop = ball_absolute_y;
		while (pos_y_loop < 0)
		{
			pos_y_loop += screen_height;
			count++;
		}
		while (pos_y_loop > screen_height)
		{
			pos_y_loop -= screen_height;
			count++;
		}

		// 範囲を調整する回数が奇数であればY軸を中心に反転させる
		if (count % 2 == 0)
			target_pos_y = pos_y_loop;
		else
			target_pos_y = screen_height - pos_y_loop;
	}

	return target_pos_y + screen_top_y;
}

// <<プレイヤーコントローラー>> ----------------------------------------

// <コントローラー作成>
GameController GameController_Player_Create(GameObject* object, GameScene* scene, GameObject* enemy, int key_up, int key_down)
{
	GameController ctrl = GameController_Create(object, GameController_Player_Update, GameController_Player_UpdateControl, scene, enemy);
	ctrl.player_key_up = key_up;
	ctrl.player_key_down = key_down;
	return ctrl;
}

void GameController_Player_Update(GameController* ctrl)
{
}

// キー入力でパドルを操作
void GameController_Player_UpdateControl(GameController* ctrl)
{
	ctrl->object->vel.y = 0.f;
	if (IsButtonDown(ctrl->player_key_up))
		ctrl->object->vel.y += -PADDLE_VEL;
	if (IsButtonDown(ctrl->player_key_down))
		ctrl->object->vel.y += PADDLE_VEL;
}

// <<Botコントローラー>> -----------------------------------------------

// <コントローラー作成>
GameController GameController_Bot_Create(GameObject* object, GameScene* scene, GameObject* enemy)
{
	return  GameController_Create(object, GameController_Bot_Update, GameController_Bot_UpdateControl, scene, enemy);
}

void GameController_Bot_Update(GameController* ctrl)
{
}

// Botがパドルを操作
void GameController_Bot_UpdateControl(GameController* ctrl)
{
	// Botが動き始めるしきい値
	float padding = 260 * BALL_VEL_X_MIN / PADDLE_VEL;

	int k = (ctrl->object->pos.x < ctrl->enemy->pos.x) ? 1 : -1;

	ctrl->object->vel.y = 0.f;

	// 自分向きかつしきい値より近かったら動く
	if (k * (ctrl->scene->ball.vel.x) < 0 && abs(ctrl->scene->ball.pos.x - ctrl->object->pos.x) < padding)
	{
		// Botがパドルを操作
		float pos_y = ctrl->target_pos.y;

		// 死んだら中央に戻る
		if (k * ctrl->scene->ball.pos.x < k * GameObject_GetX(&ctrl->scene->field, k > 0 ? LEFT : RIGHT))
			pos_y = ClampF(ctrl->scene->ball.pos.y, ctrl->scene->field.pos.y - 80.f, ctrl->scene->field.pos.y + 80.f);

		// Botが移動できる幅を制限
		//target1_pos_y = ClampF(target1_pos_y, SCREEN_TOP + 50, SCREEN_BOTTOM - 50);

		if (ctrl->object->pos.y - pos_y > 1.2f * PADDLE_VEL)
			ctrl->object->vel.y += -PADDLE_VEL;
		else if (ctrl->object->pos.y - pos_y < -1.2f * PADDLE_VEL)
			ctrl->object->vel.y += PADDLE_VEL;
	}
}

// <Botガイド描画>
void GameController_RenderGuide(GameController* ctrl)
{
	GameObject target = *ctrl->object;
	target.pos = ctrl->target_pos;
	GameObject_Render(&target, 0x222222);
}

// <<ネットワークコントローラー>> --------------------------------------

// <コントローラー作成>
GameController GameController_Network_Create(GameObject* object, GameScene* scene, GameObject* enemy, BOOL server_flag, HNET handle)
{
	GameController ctrl = GameController_Create(object, GameController_Network_Update, GameController_Network_UpdateControl, scene, enemy);
	ctrl.network_server_flag = server_flag;
	ctrl.network_handle = handle;
	return ctrl;
}

// パケットの定義
typedef struct
{
	GameObject ball;
	GameObject paddle1;
	GameObject paddle2;
} NetworkPacket;

// 通信相手がパドルとボールを操作
void GameController_Network_Update(GameController* ctrl)
{
	// フレームカウンタ
	static int frame = 0;
	frame++;

	// 切断確認 → Botに変更
	if (GetLostNetWork() == ctrl->network_handle)
	{
		ctrl->Update = GameController_Bot_Update;
		ctrl->UpdateControl = GameController_Bot_UpdateControl;
	}
	else
	{
		// パケットが届いていたら
		if (GetNetWorkDataLength(ctrl->network_handle) >= sizeof(GameScene))
		{
			// 受信パケット
			GameScene yourpacket;
			NetWorkRecv(ctrl->network_handle, &yourpacket, sizeof(GameScene));

			// 通信相手が操作するパドルを取得
			GameObject paddle;
			{
				// 自分の情報の中での位置関係
				BOOL b1 = (ctrl->enemy->pos.x < ctrl->object->pos.x);
				// 相手の情報の中での位置関係
				BOOL b2 = (yourpacket.paddle1.pos.x < yourpacket.paddle2.pos.x);
				// 位置関係が一致しているパドルを選択
				if ((b1 && b2) || (!b1 && !b2))
					paddle = yourpacket.paddle2;
				else
					paddle = yourpacket.paddle1;
			}

			{
				float left = GameObject_GetX(&ctrl->scene->field, LEFT);
				float center = GameObject_GetX(&ctrl->scene->field, CENTER_X);
				float right = GameObject_GetX(&ctrl->scene->field, RIGHT);
				// サーブ時は優先して相手のパケットのボール情報を取得
				if (yourpacket.packet == PACKET_START || yourpacket.packet == PACKET_END || yourpacket.packet == PACKET_SERVE)
				{
					ctrl->scene->ball = yourpacket.ball;
				}
				// 画面内かつ半分より遠い位置にボールがある場合、相手のパケットのボール情報を取得
				else if (((center < yourpacket.ball.pos.x && center < paddle.pos.x) && (yourpacket.ball.pos.x < right && paddle.pos.x < right)) ||
					((left < yourpacket.ball.pos.x && left < paddle.pos.x) && (yourpacket.ball.pos.x < center && paddle.pos.x < center)))
				{
					ctrl->scene->ball = yourpacket.ball;
				}
			}

			// 相手のパケットのパドル情報を取得
			*ctrl->object = paddle;

			// その他情報を同期
			if (yourpacket.packet == PACKET_START)
			{
				// X座標を画面中央へ戻す
				GameObject_Ball_SetPosXDefault(&ctrl->scene->ball, &ctrl->scene->field);

				// パドルを初期位置へ
				GameObject_Paddle_SetPosYDefault(&ctrl->scene->paddle1);
				GameObject_Paddle_SetPosYDefault(&ctrl->scene->paddle2);
			}
			if (yourpacket.packet == PACKET_START || yourpacket.packet == PACKET_END)
				ctrl->scene->game_state = yourpacket.game_state;
			if (yourpacket.packet == PACKET_START || yourpacket.packet == PACKET_END || yourpacket.packet == PACKET_SCORE)
				ctrl->scene->score = yourpacket.score;
			ctrl->scene->field = yourpacket.field;
		}

		// 3フレームに1回パケットを送信
		if (frame % 3 == 0)
		{
			// 送信パケット
			GameScene* mypacket = ctrl->scene;
			NetWorkSend(ctrl->network_handle, mypacket, sizeof(GameScene));
			mypacket->packet = PACKET_NONE;
		}
	}
}

void GameController_Network_UpdateControl(GameController* ctrl)
{
}