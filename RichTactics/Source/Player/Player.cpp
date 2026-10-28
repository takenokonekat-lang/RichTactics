#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"
#include "../Camera/Camera.h"

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
	10, 12, 80, 80,
	10, 24, 80, 80,
};

// プレイヤー設定関連
#define PLAYER_DEFAULT_POS_X (100.0f)
#define PLAYER_DEFAULT_POS_Y (500.0f)
#define PLAYER_MOVE_SPEED (10.0f)

// マップ衝突判定用のプレイヤーサイズ補正
#define PLAYER_MAP_COLLISION_OFFSET (0.05f)
// 矩形判定の位置補正
#define PLAYER_BOX_COLLISION_OFFSET_X (24)
#define PLAYER_BOX_COLLISION_OFFSET_Y (20)
// 矩形判定のサイズ
#define PLAYER_BOX_COLLISION_WIDTH (20)
#define PLAYER_BOX_COLLISION_HEIGHT (44)

PlayerData g_PlayerData = { 0 };
PlayerData g_PrevPlayerData = { 0 };

// このCPPでのみ使用する関数の宣言
void StartPlayerAnimation(PlayerAnimationType anim);	// アニメーション再生
void UpdatePlayerAnimation();							// アニメーション更新
void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h);

void InitPlayer()
{
	g_PlayerData.posX = PLAYER_DEFAULT_POS_X;
	g_PlayerData.posY = PLAYER_DEFAULT_POS_Y;
	g_PlayerData.moveX = 0.0f;
	g_PlayerData.moveY = 0.0f;
	g_PlayerData.active = false;
	g_PlayerData.playAnim = PLAYER_ANIM_NONE;

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
	g_PlayerData.boxCollision.posX = PLAYER_BOX_COLLISION_OFFSET_X;
	g_PlayerData.boxCollision.posY = PLAYER_BOX_COLLISION_OFFSET_Y;
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
	g_PlayerData.moveX = 0.0f;


	//上入力
	if (IsInputKey(KEY_UP))
	{
		g_PlayerData.moveY = -PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = true;
	}

	//下入力
	if (IsInputKey(KEY_UP))
	{
		g_PlayerData.moveY = PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = false;
	}

	// 左入力
	if (IsInputKey(KEY_LEFT))
	{
		g_PlayerData.moveX = -PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = true;
	}
	// 右入力
	else if (IsInputKey(KEY_RIGHT))
	{
		g_PlayerData.moveX = PLAYER_MOVE_SPEED;
		g_PlayerData.isTurn = false;
	}

}

void UpdatePlayer()
{
	// 死んでいたら処理しない
	if (!g_PlayerData.active) return;

	// 移動処理
	g_PlayerData.posX += g_PlayerData.moveX;
	g_PlayerData.posY += g_PlayerData.moveY;

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
	DrawAnimation(animData, g_PlayerData.posX - camera.posX, g_PlayerData.posY - camera.posY, g_PlayerData.isTurn);

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
	player.posX = g_PlayerData.posX;
	player.posY = g_PrevPlayerData.posY;

	// 当たり判定のボックス計算
	float x, y, w, h;
	CalcBoxCollision(player, x, y, w, h);

	if (CheckSquareSquare(x + POS_OFFSET, y + POS_OFFSET, w - SIZE_OFFSET, h - SIZE_OFFSET,
		block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
	{
		// 左からあたったか
		if (player.moveX > 0.0f)
		{
			// 左に押し出す
			g_PlayerData.posX -= (x + w) - block->pos.x;
		}
		// 右からあたったか
		else if (player.moveX < 0.0f)
		{
			// 右に押し出す
			g_PlayerData.posX += (block->pos.x + MAP_CHIP_WIDTH) - x;
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
		g_PlayerData.moveY = 0.0f;

		// 上からあたったか
		if (player.moveY > 0.0f)
		{
			// 上に押し出す
			g_PlayerData.posY -= (y + h) - block->pos.y;
			g_PlayerData.isAir = false;
		}
		// 下からあたったか
		else if (player.moveY < 0.0f)
		{
			// 下に押し出す
			g_PlayerData.posY += (block->pos.y + MAP_CHIP_WIDTH) - y;
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
	StartAnimation(animData, g_PlayerData.posX, g_PlayerData.posY, animParam.interval, animParam.frameNum, animParam.width, animParam.height, true);
}

void UpdatePlayerAnimation()
{
	// 地上にいるか
	if (!g_PlayerData.isAir)
	{
		// 横に移動していれば走っている
		if (g_PlayerData.moveX < 0.0f || g_PlayerData.moveX > 0.0f)
		{
			// 走るアニメーション
			StartPlayerAnimation(PLAYER_ANIM_RUN);
		}
		else
		{
			// 移動していなければ待機
			StartPlayerAnimation(PLAYER_ANIM_IDLE);
		}
	}
	// 空中にいる
	else
	{
		// 上昇していればジャンプアニメーション
		if (g_PlayerData.moveY < 0.0f)
		{
			StartPlayerAnimation(PLAYER_ANIM_JUMP);
		}
		// 下降していれば落下アニメーション
		else if (g_PlayerData.moveY > 0.0f)
		{
			StartPlayerAnimation(PLAYER_ANIM_FALL);
		}
	}

	// アニメーション更新
	PlayerAnimationType animType = g_PlayerData.playAnim;
	AnimationData* animData = &g_PlayerData.animation[animType];
	UpdateAnimation(animData);
}

void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h)
{
	x = player.isTurn ?
		player.posX + PLAYER_WIDTH - player.boxCollision.posX - player.boxCollision.width :
		player.posX + player.boxCollision.posX;
	y = player.posY + player.boxCollision.posY;
	w = player.boxCollision.width;
	h = player.boxCollision.height;
}
