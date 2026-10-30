#pragma once
#include "../Map/MapParameter.h"
#include "../Animation/Animation.h"
#include "../Collision/Collision.h"

// プレイヤーサイズ
#define PLAYER_WIDTH	(80.0f)
#define PLAYER_HEIGHT	(80.0f)
#define PLAYER_RADIUS	(40.0f)

// アニメーションタイプ
enum PlayerAnimationType
{
	PLAYER_ANIM_IDLE,
	PLAYER_ANIM_RUN,
	PLAYER_ANIM_JUMP,
	PLAYER_ANIM_FALL,
	PLAYER_ANIM_LANDING,
	PLAYER_ANIM_MAX,
	PLAYER_ANIM_NONE = -1
};

// プレイヤーデータ
struct PlayerData
{
	bool active;								// 生存フラグ
	bool isTurn;								// 左を向いているか
	int handle;
	float angle;
	int invisibleTimer;
	float bulletIntervalTimer;

	AnimationData animation[PLAYER_ANIM_MAX];	// アニメーション
	PlayerAnimationType playAnim;				// 再生中のアニメーション
	BoxCollision boxCollision;					// 矩形当たり判定

	VECTOR dir;
	VECTOR pos;
	VECTOR move;
};

// 関数のプロトタイプ宣言
void InitPlayer();
void LoadPlayer();
void StartPlayer();
void StepPlayer();
void UpdatePlayer();
void DrawPlayer();
void FinPlayer();

// 取得関数
PlayerData GetPlayer();

// 当たり判定
void PlayerHitNormalBlockX(MapChipData mapChipData);
void PlayerHitNormalBlockY(MapChipData mapChipData);