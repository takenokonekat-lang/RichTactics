#pragma once
#include "DxLib.h"

#define STRAIGHT_BULLET_MAX (1024)

enum BulletCategory
{
	BULLET_CATEGORY_OLD,
};

// 弾の当たり判定で区別をつけるためのタグ
enum BulletCollisionTag
{
	BULLET_COLLISION_TAG_PLAYER,	// プレイヤーが撃った弾
};

enum BulletType
{
	BULLET_TYPE_PLAYER_OLD,			// プレイヤーの通常弾
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
	int life;
	VECTOR pos;
	VECTOR move;
};
