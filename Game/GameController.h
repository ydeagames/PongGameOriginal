#pragma once
#include "GameScene.h"

// 構造体の宣言 ============================================================

// <ゲームコントローラー>
typedef struct tag_GameController GameController;
struct tag_GameController
{
	GameObject* object;
	void(*Update)(GameController*);
	void(*UpdateControl)(GameController*);

	Vec2 target_pos;
	GameScene* scene;
	GameObject* enemy;

	// 継承ができないので仕方なくPlayer用の変数
	int player_key_up;
	int player_key_down;

	// 継承ができないので仕方なくネットワーク用の変数
	BOOL network_server_flag;
	HNET network_handle;
};

// 関数の宣言 ==============================================================

// <<コントローラー>> --------------------------------------------------

// <コントローラー作成>
GameController GameController_Create(GameObject* object, void(*updateFunc)(GameController*), void(*updateCtrlFunc)(GameController*), GameScene* scene, GameObject* enemy);

// <コントローラー更新>
void GameController_Update(GameController* ctrl);

// <コントローラー操作更新>
void GameController_UpdateControl(GameController* ctrl);

// <ボール着弾点予想アルゴリズム>
float GameController_GetTargetY(GameObject* field, GameObject* ball, GameObject* paddle_myself, GameObject* paddle_enemy);

// <<プレイヤーコントローラー>> ----------------------------------------

// <コントローラー作成>
GameController GameController_Player_Create(GameObject* object, GameScene* scene, GameObject* enemy, int key_up, int key_down);

// <<Botコントローラー>> -----------------------------------------------

// <Botコントローラー作成>
GameController GameController_Bot_Create(GameObject* object, GameScene* scene, GameObject* enemy);

// <Botガイド描画>
void GameController_RenderGuide(GameController* ctrl);


// <<ネットワークコントローラー>> --------------------------------------

// <コントローラー作成>
GameController GameController_Network_Create(GameObject* object, GameScene* scene, GameObject* enemy, BOOL server_flag, HNET handle);
