#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"
#include "../Effect/AnimationEffect.h"
#include "../Bullet/BulletManager.h"
#include "../Weapon/Gun.h"
#include "../Scene/SceneManager.h"
#include "../Sound/SoundManager.h"
#include "../Map/MapChip.h"
#include "../Map/MapManager.h"
#include "../Camera/Camera.h"
#include "../Common/Common.h"
#include "../Math/MyMath.h"
#include "../Sound/SoundManager.h"
#include <math.h>
#include "../Weapon/Gun.h"


// プレイヤー設定関連
#define PLAYER_DEFAULT_POS_X		(400.0f)
#define PLAYER_DEFAULT_POS_Y		(400.0f)
#define PLAYER_MOVE_SPEED			(7.0f)

#define PLAYER_FOV_RANGE			(200.0f)
#define PLAYER_VIEWING_ANGLE		(2.5f)

// マップ衝突判定用のプレイヤーサイズ補正
#define PLAYER_MAP_COLLISION_OFFSET		(0.10f)
// 矩形判定の位置補正
#define PLAYER_BOX_COLLISION_OFFSET_X	(24)
#define PLAYER_BOX_COLLISION_OFFSET_Y	(20)
// 矩形判定のサイズ
#define PLAYER_BOX_COLLISION_WIDTH		(20)
#define PLAYER_BOX_COLLISION_HEIGHT		(44)

// 弾関連
#define PLAYER_NORMAL_SHOT_SPEED	(32.0f)
#define PLAYER_NORMAL_SHOT_LIFE		(600)
#define PLAYER_NORMAL_SHOT_OFFSET_X	(55.0f)
#define PLAYER_NORMAL_SHOT_OFFSET_Y	(30.0f)
#define PLAYER_NORMAL_SHOT_INTERVAL (30)
#define PLAYER_NEW_SHOT_INTERVAL	(15)
#define PLAYER_SHOTGUN_INTERVAL		(20)


// プレイヤーの残機
#define PLAYER_DEFAULT_LIFE (1)

// リスポーンまでの待機時間
#define PLAYER_RESPAWN_WAIT (180)

// リスポーン時の無敵時間
#define PLAYER_RESPAWN_INVISIBLE_TIME (180)

// プレイヤー死亡エフェクトインターバル
#define PLAYER_DEAD_EFFECT_INTERVAL (2)

PlayerData g_PlayerData = { 0 };
PlayerData g_PrevPlayerData = { 0 };

void CalcBoxDollision(PlayerData player, float& x, float& y, float& w, float& h);

void InitPlayer()
{
	g_PlayerData.handle = 0;
	g_PlayerData.body.move.x = 0.0f;
	g_PlayerData.body.move.y = 0.0f;
	g_PlayerData.body.width = 60.0f;
	g_PlayerData.body.height = 60.0f;
	g_PlayerData.active = false;
	g_PlayerData.bulletIntervalTimer;
	g_PlayerData.guns[0] = REVOLVER;
	g_PlayerData.gunCount = 1;
	g_PlayerData.currentGun = 0;

	// 構造体はmemsetで0初期化できる
	memset(&g_PlayerData.boxCollision, 0, sizeof(g_PlayerData.boxCollision));
}

void LoadPlayer()
{
	g_PlayerData.handle = LoadGraph("Data/Player/Player.png");
}

void StartPlayer()
{
	// 生存フラグを立てる
	g_PlayerData.active = true;

	//初期位置
	g_PlayerData.body.pos.x = PLAYER_DEFAULT_POS_X;
	g_PlayerData.body.pos.y = PLAYER_DEFAULT_POS_Y;
	g_PlayerData.body.width = PLAYER_WIDTH;
	g_PlayerData.body.height = PLAYER_HEIGHT;

	g_PlayerData.guns[0] = REVOLVER;
	g_PlayerData.gunCount = 1;
	g_PlayerData.currentGun = 0;

	g_PlayerData.GUNTYPE = REVOLVER;
	SetGun(REVOLVER);

	g_PlayerData.life = PLAYER_DEFAULT_LIFE;

	// 矩形判定設定
	g_PlayerData.boxCollision.pos.x = PLAYER_BOX_COLLISION_OFFSET_X;
	g_PlayerData.boxCollision.pos.y = PLAYER_BOX_COLLISION_OFFSET_Y;
	g_PlayerData.boxCollision.width = PLAYER_BOX_COLLISION_WIDTH;
	g_PlayerData.boxCollision.height = PLAYER_BOX_COLLISION_HEIGHT;
}

void StepPlayer()
{
	// 死んでいたら何もしない
	if (!g_PlayerData.active)
	{
		return;
	}

	// 前回のデータとして記憶	
	g_PrevPlayerData = g_PlayerData;

	// 移動量は毎回リセットする
	g_PlayerData.body.move.x = 0.0f;
	g_PlayerData.body.move.y = 0.0f;

	// 通常弾のインターバル時間を減らす
	if (g_PlayerData.bulletIntervalTimer > 0)
	{
		g_PlayerData.bulletIntervalTimer--;
	}

	//上入力
	if (IsInputKey(KEY_UP))
	{
		g_PlayerData.body.move.y = -PLAYER_MOVE_SPEED;
		g_PlayerData.angle = 270;

		//float sa = 270 - g_PlayerData.angle;
		//float p = sa / 5;
		//p = (p > 30) ? 30 : p;
		//g_PlayerData.angle = g_PlayerData.angle + p;
	}
	//下入力
	if (IsInputKey(KEY_DOWN))
	{
		g_PlayerData.body.move.y = PLAYER_MOVE_SPEED;
		g_PlayerData.angle = 90;
	}
	// 左入力
	if (IsInputKey(KEY_LEFT))
	{
		g_PlayerData.body.move.x = -PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = true;
		g_PlayerData.angle = 180;
	}
	// 右入力
	if (IsInputKey(KEY_RIGHT))
	{
		g_PlayerData.body.move.x = PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = false;
		g_PlayerData.angle = 0;
	}

	//上入力+右入力
	if ((IsInputKey(KEY_UP)) && (IsInputKey(KEY_RIGHT)))
	{
		g_PlayerData.body.move.x = PLAYER_MOVE_SPEED;
		g_PlayerData.body.move.y = -PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = false;
		g_PlayerData.angle = 315;
	}
	//下入力+左入力
	if ((IsInputKey(KEY_DOWN)) && (IsInputKey(KEY_LEFT)))
	{
		g_PlayerData.body.move.x = -PLAYER_MOVE_SPEED;
		g_PlayerData.body.move.y = PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = true;
		g_PlayerData.angle = 135;
	}

	//下入力+右入力
	if ((IsInputKey(KEY_DOWN)) && (IsInputKey(KEY_RIGHT)))
	{
		g_PlayerData.body.move.x = PLAYER_MOVE_SPEED;
		g_PlayerData.body.move.y = PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = false;
		g_PlayerData.angle = 45;
	}

	//上入力+左入力
	if ((IsInputKey(KEY_UP)) && (IsInputKey(KEY_LEFT)))
	{
		g_PlayerData.body.move.x = -PLAYER_MOVE_SPEED;
		g_PlayerData.body.move.y = -PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = true;
		g_PlayerData.angle = 225;
	}


	// Z入力
	if (IsInputKey(KEY_Z))
	{
		FireBullet();
	}

	//X入力
	if (IsTriggerKey(KEY_X))
	{
		GunType pickupGunType = PickupGun();

		//武器を取得できた場合
		if (pickupGunType != GUN_NONE)
		{
			// 所持数が2個未満の場合
			if (g_PlayerData.gunCount < 2)
			{
				// 所持枠に追加
				g_PlayerData.guns[g_PlayerData.gunCount] = pickupGunType;

				// 拾った武器を装備
				g_PlayerData.currentGun = g_PlayerData.gunCount;
				g_PlayerData.gunCount++;
			}
			// すでに2個持っている場合
			else
			{
				// 現在装備中の武器
				GunType dropGun =
					g_PlayerData.guns[g_PlayerData.currentGun];

				// 武器を落とす
				float dropX =
					g_PlayerData.body.pos.x +
					g_PlayerData.body.width / 2 + 80;

				float dropY =
					g_PlayerData.body.pos.y +
					g_PlayerData.body.height / 2;

				SpawnGun(dropX, dropY, dropGun);

				// 武器を交換
				g_PlayerData.guns[g_PlayerData.currentGun] =
					pickupGunType;
			}

			// 取得音
			PlaySE(Wepon_Get);
		}
		// 武器を拾えなかった場合
		else
		{
			// 2個以上持っている場合だけ切り替え
			if (g_PlayerData.gunCount >= 2)
			{
				g_PlayerData.currentGun++;

				if (g_PlayerData.currentGun >= g_PlayerData.gunCount)
				{
					g_PlayerData.currentGun = 0;
				}
			}
		}

		//装備している武器を更新
		g_PlayerData.GUNTYPE = g_PlayerData.guns[g_PlayerData.currentGun];

		SetGun(g_PlayerData.GUNTYPE);



		// 無敵中の処理
		if (g_PlayerData.invisible)
		{
			g_PlayerData.invisibleTimer--;
		}
	}
}

void UpdatePlayer()
{
	// 死んでいたら処理しない
	if (!g_PlayerData.active) return;

	MoveWithMapCollision(&g_PlayerData.body);

	// 無敵解除チェック
	if (g_PlayerData.invisible && g_PlayerData.invisibleTimer <= 0)
	{
		g_PlayerData.invisible = false;
	}
}

void DrawPlayer()
{
	if (!g_PlayerData.active) return;

	//カメラ取得
	CameraData camera = GetCamera();

	VECTOR playerPos = 
	{
		g_PlayerData.body.pos.x + g_PlayerData.body.width / 2,
		g_PlayerData.body.pos.y + g_PlayerData.body.height / 2,
		0.0f
	};

	//Player描画
	DrawRotaGraph((int)(g_PlayerData.body.pos.x+g_PlayerData.body.width/2 - camera.pos.x), (int)(g_PlayerData.body.pos.y+g_PlayerData.body.height/2 - camera.pos.y), 
						1.0,0,g_PlayerData.handle, TRUE,g_PlayerData.isTurn);

	DrawEquipGun(playerPos);
}

void FinPlayer()
{
	DeleteGraph(g_PlayerData.handle);
}

PlayerData GetPlayer()
{
	return g_PlayerData;
}

// リスポーン処理(リスポーンできなかったらfalseを返す)
bool RespawnPlayer()
{
	// 残機チェック
	if (g_PlayerData.life > 0)
	{
		// アクティブ
		g_PlayerData.active = true;

		// 無敵状態
		g_PlayerData.invisible = true;
		g_PlayerData.invisibleTimer = PLAYER_RESPAWN_INVISIBLE_TIME;

		// 初期位置に配置
		g_PlayerData.body.pos.x = PLAYER_DEFAULT_POS_X;
		g_PlayerData.body.pos.y = PLAYER_DEFAULT_POS_Y;

		// リスポーン成功
		return true;
	}

	// リスポーン不可
	return false;
}

void PlayerDead()
{
	// 死亡
	g_PlayerData.active = false;

	// リスポーン待機開始
	g_PlayerData.respawnTimer = PLAYER_RESPAWN_WAIT;
}

// 敵と衝突した
void PlayerHitEnemy()
{
	// 衝突したら死亡
	PlayerDead();
}

// 敵の弾と衝突した
void PlayerHitEnemyBullet()
{
	// 衝突したら死亡
	PlayerDead();
}


void CalcBoxDollision(PlayerData player, float& x, float& y, float& w, float& h)
{
	x = player.isTurn ?
		player.body.pos.x + PLAYER_WIDTH - player.boxCollision.pos.x - player.boxCollision.width :
		player.body.pos.x + player.boxCollision.pos.x;
	y = player.body.pos.y + player.boxCollision.pos.y;
	w = player.boxCollision.width;
	h = player.boxCollision.height;
}

void FireBullet()
{
	// 弾発射位置はちょっと前から
	float fireX = g_PlayerData.body.pos.x + PLAYER_NORMAL_SHOT_OFFSET_X;
	float fireY = g_PlayerData.body.pos.y + PLAYER_NORMAL_SHOT_OFFSET_Y;

	// 現在向いている向きを計算
	VECTOR PlayerVec = { 0 };
	g_PlayerData.dir.x = cosf(AngleToRadian(g_PlayerData.angle));
	g_PlayerData.dir.y = sinf(AngleToRadian(g_PlayerData.angle));

	// 向きのベクトルは常に正規化しておく
	g_PlayerData.dir = VecNormalize(g_PlayerData.dir);

	// インターバル時間がすぎてから撃つ
	if (g_PlayerData.bulletIntervalTimer <= 0)
	{
		VECTOR* pos = new VECTOR;
		VECTOR* move = new VECTOR;
		pos->x = g_PlayerData.body.pos.x + g_PlayerData.dir.x * PLAYER_BOX_COLLISION_OFFSET_X;
		pos->y = g_PlayerData.body.pos.y + g_PlayerData.dir.y * PLAYER_BOX_COLLISION_OFFSET_Y;

		move->x = g_PlayerData.dir.x * PLAYER_BOX_COLLISION_OFFSET_X;
		move->y = g_PlayerData.dir.y * PLAYER_BOX_COLLISION_OFFSET_Y;

		// 弾発射情報を設定
		const FireBulletData FIRE_DATA =
		{
			true,
			0,
			PLAYER_NORMAL_SHOT_LIFE,
			pos->x + g_PlayerData.body.width / 2,
			pos->y + g_PlayerData.body.height / 2 - 10,
			0,
			move->x,
			move->y,
			0.0f,
		};

		FireBullet(BULLET_CATEGORY_STRAIGHT, STRAIGHT_BULLET_TYPE_PLAYER_NORMAL, FIRE_DATA, BULLET_COLLISION_TAG_PLAYER);

		// 向いている方向にバレットを発射する
		VECTOR bulletMove = VecScale(g_PlayerData.dir, PLAYER_NORMAL_SHOT_SPEED);

		// インターバル設定
		g_PlayerData.bulletIntervalTimer = PLAYER_NORMAL_SHOT_INTERVAL;
		if (g_PlayerData.GUNTYPE == NEWREVOLVER)
		{
			g_PlayerData.bulletIntervalTimer = PLAYER_NEW_SHOT_INTERVAL;
		}
		if (g_PlayerData.GUNTYPE == MACHINE_GUN)
		{
			g_PlayerData.bulletIntervalTimer = PLAYER_SHOTGUN_INTERVAL;
		}
	}
}
