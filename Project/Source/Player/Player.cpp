#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"
#include "../Effect/AnimationEffect.h"
#include "../Bullet/BulletManager.h"
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

//アニメーション用パラメータ
struct PlayerAnimationParam
{
	int interval;
	int frameNum;
	int width;
	int height;
};
const PlayerAnimationParam PLAYER_ANIM_PARAM[PLAYER_ANIM_MAX] =
{
	10, 4, 80, 80,
	10, 6, 80, 80,
};

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

void StartPlayerAnimation(PlayerAnimationType anim);
void UpdataPlayerAnimation();
void CalcBoxDollision(PlayerData player, float& x, float& y, float& w, float& h);

void InitPlayer()
{
	g_PlayerData.body.pos.x = PLAYER_DEFAULT_POS_X;
	g_PlayerData.body.pos.y = PLAYER_DEFAULT_POS_Y;
	g_PlayerData.body.move.x = 0.0f;
	g_PlayerData.body.move.y = 0.0f;
	g_PlayerData.width = 80.0f;
	g_PlayerData.height = 80.0f;
	g_PlayerData.active = false;
	g_PlayerData.playAnim = PLAYER_ANIM_NONE;
	g_PlayerData.bulletIntervalTimer;

	for (int i = 0; i < PLAYER_ANIM_MAX; i++)
	{
		InitAnimation(&g_PlayerData.animation[i]);
	}

	// 構造体はmemsetで0初期化できる
	memset(&g_PlayerData.boxCollision, 0, sizeof(g_PlayerData.boxCollision));
}

void LoadPlayer()
{
	g_PlayerData.animation[PLAYER_ANIM_IDLE].handle = LoadGraph("Data/Player/PlayerIdle.png");
	g_PlayerData.animation[PLAYER_ANIM_RUN].handle = LoadGraph("Data/Player/PlayerRun.png");
}

void StartPlayer()
{
	// 生存フラグを立てる
	g_PlayerData.active = true;

	g_PlayerData.life = PLAYER_DEFAULT_LIFE;

	// サイズ設定
	g_PlayerData.body.width = PLAYER_WIDTH;
	g_PlayerData.body.height = PLAYER_HEIGHT;

	// 矩形判定設定
	g_PlayerData.boxCollision.pos.x = PLAYER_BOX_COLLISION_OFFSET_X;
	g_PlayerData.boxCollision.pos.y = PLAYER_BOX_COLLISION_OFFSET_Y;
	g_PlayerData.boxCollision.width = PLAYER_BOX_COLLISION_WIDTH;
	g_PlayerData.boxCollision.height = PLAYER_BOX_COLLISION_HEIGHT;

	// 待機アニメーションから開始する
	StartPlayerAnimation(PLAYER_ANIM_IDLE);
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
		if (CheckSquareSquare(g_PlayerData.body.pos.x, g_PlayerData.body.pos.y, g_PlayerData.body.width, g_PlayerData.body.height,
			GetGun().pos.x, GetGun().pos.y, GetGun().width, GetGun().height))
		{
			GunType work = g_PlayerData.GunType;
			g_PlayerData.GunType = GetGun().gunType;
			SetGun(work);

			PlaySE(Wepon_Get);
		}
	}

	// 無敵中の処理
	if (g_PlayerData.invisible)
	{
		g_PlayerData.invisibleTimer--;
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

	PlayerAnimationType animeType = g_PlayerData.playAnim;
	AnimationData* animData = &g_PlayerData.animation[animeType];
	VECTOR* vec = new VECTOR;
	vec->x = g_PlayerData.body.pos.x - camera.pos.x;
	vec->y = g_PlayerData.body.pos.y - camera.pos.y;

	DrawAnimation(animData, vec, g_PlayerData.isTurn);

	VECTOR gunPos;
	if (g_PlayerData.isTurn == false)
	{
		gunPos = VGet(73, 48, 0);
	}
	else
	{
		gunPos = VGet(7, 48, 0);
	}

	int handle = 0;
	if (g_PlayerData.GunType == REVOLVER)
	{
		handle = GetGun().revolverhandle;
	}
	if (g_PlayerData.GunType == NEWREVOLVER)
	{
		handle = GetGun().newrevolverhandle;
	}
	if (g_PlayerData.GunType == MACHINE_GUN)
	{
		handle = GetGun().machinegunhandle;
	}
	DrawRotaGraph(
		(int)(g_PlayerData.body.pos.x - camera.pos.x + gunPos.x),
		(int)(g_PlayerData.body.pos.y - camera.pos.y + gunPos.y),
		1.0, GetGun().angle,
		handle,
		true,
		g_PlayerData.isTurn);
}

void FinPlayer()
{
	for (int i = 0; i < PLAYER_ANIM_MAX; i++)
	{
		DeleteGraph(g_PlayerData.animation[i].handle);
	}
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

void StartPlayerAnimation(PlayerAnimationType anim)
{
	//再生中のアニメーションであれば何もしない
	if (anim == g_PlayerData.playAnim)return;

	//再生中アニメーション設定
	g_PlayerData.playAnim = anim;

	//再生中に必要なデータを取得
	AnimationData* animData = &g_PlayerData.animation[anim];
	PlayerAnimationParam animParam = PLAYER_ANIM_PARAM[anim];

	// アニメーション再生
	StartAnimation(animData, g_PlayerData.body.pos, animParam.interval, animParam.frameNum, animParam.width, animParam.height, true);
}

void UpdataPlayerAnimation()
{
	//横に移動していれば走っている
	if (g_PlayerData.body.move.x < 0.0f || g_PlayerData.body.move.x>0.0f)
	{
		//走るアニメーション
		StartPlayerAnimation(PLAYER_ANIM_RUN);
	}
	else
	{
		//移動していなければ待機
		StartPlayerAnimation(PLAYER_ANIM_IDLE);
	}

	//アニメーション更新
	PlayerAnimationType animType = g_PlayerData.playAnim;
	AnimationData* animData = &g_PlayerData.animation[animType];
	UpdateAnimation(animData);
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
			g_PlayerData.active = true,
			g_PlayerData.handle = true,
			PLAYER_NORMAL_SHOT_LIFE,
			pos->x + g_PlayerData.width / 2,
			pos->y + g_PlayerData.height / 2 - 10,
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
		if (g_PlayerData.GunType == NEWREVOLVER)
		{
			g_PlayerData.bulletIntervalTimer = PLAYER_NEW_SHOT_INTERVAL;
		}
		if (g_PlayerData.GunType == MACHINE_GUN)
		{
			g_PlayerData.bulletIntervalTimer = PLAYER_SHOTGUN_INTERVAL;
		}
	}
}
