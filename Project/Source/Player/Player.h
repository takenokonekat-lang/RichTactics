#pragma once
#include "../Collision/Collision.h"
#include "../Animation/Animation.h"
#include "../Map/MapParameter.h"
#include "../Collision/CollisionParameter.h"
#include "../Weapon/Gun.h"

// プレイヤーサイズ
#define PLAYER_WIDTH	(60.0f)
#define PLAYER_HEIGHT	(60.0f)
#define PLAYER_RADIUS	(30.0f)

// プレイヤーデータ
struct PlayerData
{
	bool active;				// 生存フラグ
	bool invisible;				// 無敵フラグ
	bool isTurn;				//左を向いてるかどうか
	int handle;					// 画像ハンドル
	int bulletIntervalTimer;	// 弾の発射間隔タイマー
	int respawnTimer;			// リスポーン待機タイマー
	int invisibleTimer;			// 無敵時間タイマー
	int life;					// 残機数
	float angle;

	Body body;
	VECTOR dir;

	GunType gunType;			//銃の種類
	GunType guns[2];			//持てる銃の数
	int gunCount;				//持っている銃の数
	int currentGun;				//装備している武器

	BoxCollision boxCollision;
};

// 関数のプロトタイプ宣言
void InitPlayer();
void LoadPlayer();
void StartPlayer();
void StepPlayer();
void UpdatePlayer();
void DrawPlayer();
void FinPlayer();

void FireBullet();

// 取得関数
PlayerData GetPlayer();

// リスポーン処理(リスポーンできなかったらfalseを返す)
bool RespawnPlayer();
