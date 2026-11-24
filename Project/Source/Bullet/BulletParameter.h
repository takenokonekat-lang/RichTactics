#pragma once
#include "DxLib.h"

#define STRAIGHT_BULLET_MAX (1024)

// 発射する弾丸のカテゴリ
enum BulletCategory
{
	BULLET_CATEGORY_STRAIGHT,
	BULLET_CATEGORY_MACHINE,
	BULLET_CATEGORY_SHOT,
	BULLET_CATEGORY_SNIP,
};

// 弾の当たり判定で区別をつけるためのタグ
enum BulletCollisionTag
{
	BULLET_COLLISION_TAG_PLAYER,	// プレイヤーが撃った弾
	BULLET_COLLISION_TAG_ENEMY,		// エネミーが撃った弾
};

// 直線弾の種類
enum StraightBulletType
{
	STRAIGHT_BULLET_TYPE_PLAYER_NORMAL,		// プレイヤーの通常弾
	STRAIGHT_BULLET_TYPE_PLAYER_NEW,		// 新しい銃の弾
	STRAIGHT_BULLET_TYPE_PLAYER_MACHINE,	// マシンガンの弾
	STRAIGHT_BULLET_TYPE_PLAYER_SNIP,		// スナイパーの弾
	STRAIGHT_BULLET_TYPE_PLAYER_SHOT,		// ショットガンの弾
	STRAIGHT_BULLET_TYPE_KAPU,				// カプが撃つ弾
	STRAIGHT_BULLET_TYPE_MAX
};

// 直線弾のデータ
struct StraightBulletData
{
	int handle;
	int life;
	VECTOR pos;
	VECTOR move;
	float width;
	float height;
	float radius;
	bool active;
	BulletCollisionTag tag;
};



// 発射に必要なデータ
struct FireBulletData
{
	bool active;
	int handle;
	int life;
	VECTOR pos;
	VECTOR move;
};

