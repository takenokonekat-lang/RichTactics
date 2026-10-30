#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"
#include "../Camera/Camera.h"
#include "../Bullet/BulletManager.h"
#include "../Math/MyMath.h"
#include <math.h>

// アニメーション用パラメータ
struct PlayerAnimationParam
{
	int interval;
	int frameNum;
	int width;
	int height;
};
const PlayerAnimationParam PLAYER_ANIM_PARAM[PLAYER_ANIM_MAX] =
{
	10, 24, 80, 80,
	10, 16, 80, 80,
	
};

// プレイヤー設定関連	
#define PLAYER_DEFAULT_POS_X		(100.0f)
#define PLAYER_DEFAULT_POS_Y		(300.0f)
#define PLAYER_MOVE_SPEED			(7.0f)
#define PLAYER_VIEWING_ANGLE		(2.5f)
#define PLAYER_FOV_RANGE			(200.0f)

// マップ衝突判定用のプレイヤーサイズ補正
#define PLAYER_MAP_COLLISION_OFFSET		(0.10f)
// 矩形判定の位置補正
#define PLAYER_BOX_COLLISION_OFFSET_X	(24)
#define PLAYER_BOX_COLLISION_OFFSET_Y	(20)
// 矩形判定のサイズ
#define PLAYER_BOX_COLLISION_WIDTH		(20)
#define PLAYER_BOX_COLLISION_HEIGHT		(44)

//弾
#define PLAYER_NORMAL_SHOT_OFFSET_X	(55.0f)
#define PLAYER_NORMAL_SHOT_OFFSET_Y	(30.0f)
#define PLAYER_NORMAL_SHOT_INTERVAL (0.00f)

// 弾丸データ
#define BULLET_LIFE					(300)
#define BULLET_SPEED				(23.0f)
#define PLAYER_NORMAL_SHOT_LIFE		(400)
#define PLAYER_NORMAL_SHOT_SPEED	(20.0f)
#define BULLET_RADIUS				(15.0f)

PlayerData g_PlayerData = { 0 };
PlayerData g_PrevPlayerData = { 0 };

// このCPPでのみ使用する関数の宣言
void StartPlayerAnimation(PlayerAnimationType anim);	// アニメーション再生
void UpdatePlayerAnimation();							// アニメーション更新
void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h);

void InitPlayer()
{
	g_PlayerData.pos.x = PLAYER_DEFAULT_POS_X;
	g_PlayerData.pos.y = PLAYER_DEFAULT_POS_Y;
	g_PlayerData.move.x = 0.0f;
	g_PlayerData.move.y = 0.0f;
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
	g_PlayerData.animation[PLAYER_ANIM_RUN].handle = LoadGraph("Data/Player/PlayerRun.png");
	g_PlayerData.animation[PLAYER_ANIM_IDLE].handle = LoadGraph("Data/Player/PlayerIdle.png");
}

void StartPlayer()
{
	// 生存フラグを立てる
	g_PlayerData.active = true;

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

	// X移動量は毎回リセットする
	g_PlayerData.move.x = 0.0f;
	g_PlayerData.move.y = 0.0f;

	//上入力
	if (IsInputKey(KEY_UP))
	{
		g_PlayerData.move.y = -PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = true;
	}

	//下入力
	if (IsInputKey(KEY_DOWN))
	{
		g_PlayerData.move.y = PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = false;
	}

	// 左入力
	if (IsInputKey(KEY_LEFT))
	{
		g_PlayerData.move.x = -PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = true;
	}
	// 右入力
	else if (IsInputKey(KEY_RIGHT))
	{
		g_PlayerData.move.x = PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = false;
	}

	// Z入力
	if (IsTriggerKey(KEY_Z))
	{
		// 弾発射位置はちょっと前から
		float fireX = g_PlayerData.pos.x + PLAYER_NORMAL_SHOT_OFFSET_X;
		float fireY = g_PlayerData.pos.y + PLAYER_NORMAL_SHOT_OFFSET_Y;

		// 現在向いている向きを計算
		g_PlayerData.dir.x = cosf(g_PlayerData.angle);
		g_PlayerData.dir.y = sinf(g_PlayerData.angle);

		// 向きのベクトルは常に正規化しておく
		g_PlayerData.dir = VecNormalize(g_PlayerData.dir);

		// インターバル時間がすぎてから撃つ
		if (g_PlayerData.bulletIntervalTimer <= 0)
		{
			VECTOR* pos = new VECTOR;
			VECTOR* move = new VECTOR;
			pos->x = g_PlayerData.pos.x + g_PlayerData.dir.x * PLAYER_BOX_COLLISION_OFFSET_X;
			pos->y = g_PlayerData.pos.y + g_PlayerData.dir.y * PLAYER_BOX_COLLISION_OFFSET_Y;

			move->x = g_PlayerData.move.x + g_PlayerData.dir.x * PLAYER_BOX_COLLISION_OFFSET_X;
			move->y = g_PlayerData.move.y + g_PlayerData.dir.y * PLAYER_BOX_COLLISION_OFFSET_Y;

			// 弾発射情報を設定
			const FireBulletData FIRE_DATA =
			{
				g_PlayerData.active = true,
				g_PlayerData.handle = true,
				PLAYER_NORMAL_SHOT_LIFE,
				pos->x,
				pos->y,
				0,
				move->x,
				move->y,
				0.0f,
			};

			FireBullet(BULLET_CATEGORY_OLD, BULLET_TYPE_PLAYER_OLD, FIRE_DATA,BULLET_COLLISION_TAG_PLAYER);

			// 向いている方向にバレットを発射する
			VECTOR bulletMove = VecScale(g_PlayerData.dir, PLAYER_NORMAL_SHOT_SPEED);

			// インターバル設定
			g_PlayerData.bulletIntervalTimer = PLAYER_NORMAL_SHOT_INTERVAL;
		}
	}
}

void UpdatePlayer()
{
	// 死んでいたら処理しない
	if (!g_PlayerData.active) return;

	// 移動処理
	g_PlayerData.pos.x += g_PlayerData.move.x;
	g_PlayerData.pos.y += g_PlayerData.move.y;

	UpdatePlayerAnimation();

}

void DrawPlayer()
{
	// 死んでいたら処理しない
	if (!g_PlayerData.active) return;

	// カメラ取得
	CameraData camera = GetCamera();

	// アニメーション描画でプレイヤーを描画する
	PlayerAnimationType animType = g_PlayerData.playAnim;
	AnimationData* animData = &g_PlayerData.animation[animType];
	VECTOR* vec = new VECTOR;
	vec->x = g_PlayerData.pos.x - camera.pos.x;
	vec->y = g_PlayerData.pos.y - camera.pos.y;

	DrawAnimation(animData, vec, g_PlayerData.isTurn);

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

void PlayerHitNormalBlockX(MapChipData mapChipData)
{
	PlayerData player = g_PlayerData;
	BlockData* block = mapChipData.data;
	const float POS_OFFSET = PLAYER_MAP_COLLISION_OFFSET;
	const float SIZE_OFFSET = PLAYER_MAP_COLLISION_OFFSET * 2;

	// ターンフラグは前回のものにしないと反転した分ずれる
	player.isTurn = g_PrevPlayerData.isTurn;

	// Y移動を戻し、横に当たっているかチェック
	player.pos.x = g_PlayerData.pos.x;
	player.pos.y = g_PrevPlayerData.pos.y;

	// 当たり判定のボックス計算
	float x, y, w, h;
	CalcBoxCollision(player, x, y, w, h);

	if (CheckSquareSquare(x + POS_OFFSET, y + POS_OFFSET, w - SIZE_OFFSET, h - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		// 左からあたったか
		if (player.move.x > 0.0f)
		{
			// 左に押し出す
			g_PlayerData.pos.x -= (x + w) - block->pos.x;
		}
		// 右からあたったか
		else if (player.move.x < 0.0f)
		{
			// 右に押し出す
			g_PlayerData.pos.x += (block->pos.x + MAP_CHIP_WIDTH) - x;
		}
	}
}

void PlayerHitNormalBlockY(MapChipData mapChipData)
{
	PlayerData player = g_PlayerData;
	BlockData* block = mapChipData.data;
	const float POS_OFFSET = PLAYER_MAP_COLLISION_OFFSET;
	const float SIZE_OFFSET = PLAYER_MAP_COLLISION_OFFSET * 2;

	// ターンフラグは前回のものにしないと反転した分ずれる
	player.isTurn = g_PrevPlayerData.isTurn;

	// 当たり判定のボックス計算
	float x, y, w, h;
	CalcBoxCollision(player, x, y, w, h);

	// まだ当たっているなら縦に当たっている
	if (CheckSquareSquare(x + POS_OFFSET, y + POS_OFFSET, w - SIZE_OFFSET, h - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		// Y移動量を0にする
		g_PlayerData.move.y = 0.0f;

		// 上からあたったか
		if (player.move.y > 0.0f)
		{
			// 上に押し出す
			g_PlayerData.pos.y -= (y + h) - block->pos.y;
		}
		// 下からあたったか
		else if (player.move.y < 0.0f)
		{
			// 下に押し出す
			g_PlayerData.pos.y += (block->pos.y + MAP_CHIP_WIDTH) - y;
		}
	}
}


void StartPlayerAnimation(PlayerAnimationType anim)
{
	// 再生中のアニメーションであれば何もしない
	if (anim == g_PlayerData.playAnim) return;

	// 再生中アニメーション設定
	g_PlayerData.playAnim = anim;

	// 再生に必要なデータを取得
	AnimationData* animData = &g_PlayerData.animation[anim];
	PlayerAnimationParam animParam = PLAYER_ANIM_PARAM[anim];

	// アニメーション再生
	StartAnimation(animData, g_PlayerData.pos, animParam.interval, animParam.frameNum, animParam.width, animParam.height, true);
}

void UpdatePlayerAnimation()
{
	// 横に移動していれば走っている
	if (g_PlayerData.move.x < 0.0f || g_PlayerData.move.x > 0.0f)
	{
		// 走るアニメーション
		StartPlayerAnimation(PLAYER_ANIM_RUN);
	}
	else
	{
		// 移動していなければ待機
		StartPlayerAnimation(PLAYER_ANIM_IDLE);
	}

	// アニメーション更新
	PlayerAnimationType animType = g_PlayerData.playAnim;
	AnimationData* animData = &g_PlayerData.animation[animType];
	UpdateAnimation(animData);
}

void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h)
{
	x = player.isTurn ?
		player.pos.x + PLAYER_WIDTH - player.boxCollision.pos.x - player.boxCollision.width :
		player.pos.x + player.boxCollision.pos.x;
	y = player.pos.y + player.boxCollision.pos.y;
	w = player.boxCollision.width;
	h = player.boxCollision.height;
}

bool CheckFieldOfView(VECTOR targetPos)
{
	// ターゲットまでのベクトルを作る
	VECTOR targetVec = VecCreate(g_PlayerData.pos, targetPos);

	// ターゲットまでの距離を求める
	float targetDistance = VecLong(targetVec);

	// ターゲットまでの距離が視野範囲よりも近いか判定
	if (targetDistance <= PLAYER_FOV_RANGE)
	{
		// ベクトルを正規化する
		VECTOR targetNormal = VecNormalize(targetVec);

		// ターゲットまでのベクトルと向きのベクトルの内積を計算する
		float dottarget = VecDot(targetNormal, g_PlayerData.dir);

		// 視野の淵に沿ったベクトルと向きのベクトルの内積を計算する
		//    淵に沿ったベクトルは計算できないので
		//    ||A|| * ||B|| * cosθ の方法で内積を計算する
		float dotFov = cosf(PLAYER_VIEWING_ANGLE / 2.0f);

		// 視野範囲にターゲットがいる
		if (dottarget >= dotFov)
		{
			return true;
		}
	}

	return false;
}