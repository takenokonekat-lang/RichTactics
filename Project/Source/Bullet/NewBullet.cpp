#include "DxLib.h"
#include "NewBullet.h"
#include "../Camera/Camera.h"

#define HIT_EFFECT_INTERVAL (1)
#define HIT_EFFECT_OFFSET_X (-10.0f)
#define HIT_EFFECT_OFFSET_Y (-10.0f)

struct StraightBulletData g_NewBulletData[STRAIGHT_BULLET_MAX] = { 0 };

int g_NewBulletHandle[STRAIGHT_BULLET_TYPE_MAX] = { 0 };

// 各弾丸の半径データ
float g_NewBulletRadius[STRAIGHT_BULLET_TYPE_MAX] = { 0 };

void InitNewBullet()
{
	StraightBulletData* newBullet = g_NewBulletData;
	for (int i = 0; i < STRAIGHT_BULLET_MAX; i++)
	{
		newBullet->handle = 0;
		newBullet->life = 0;
		newBullet->pos.x = 0.0f;
		newBullet->pos.y = 0.0f;
		newBullet->move.x = 0.0f;
		newBullet->move.y = 0.0f;
		newBullet->radius = 0.0f;
		newBullet->active = false;
	}

	for (int i = 0; i < STRAIGHT_BULLET_TYPE_MAX; i++)
	{
		g_NewBulletHandle[i] = 0;
	}
}

void LoadNewBullet()
{
}

void StepNewBullet()
{
	StraightBulletData* newbullet = g_NewBulletData;
	for (int i = 0; i < STRAIGHT_BULLET_MAX; i++, newbullet++)
	{
		// 生きている弾だけ処理をする
		if (!newbullet->active)continue;

		// 寿命を減らす
		newbullet->life--;
	}
}

void UpdateNewBullet()
{
	StraightBulletData* newBullet = g_NewBulletData;
	for (int i = 0; i < STRAIGHT_BULLET_MAX; i++, newBullet++)
	{
		if (!newBullet->active)continue;

		newBullet->pos.x += newBullet->move.x;
		newBullet->pos.y += newBullet->move.y;

		if (newBullet->life <= 0)
		{
			newBullet->active = false;
		}
	}
}

void DrawNewBullert()
{
	// 処理高速化のためポインタを使う
	StraightBulletData* newBullet = g_NewBulletData;
	for (int i = 0; i < STRAIGHT_BULLET_MAX; i++, newBullet++)
	{
		// 生きている弾だけ描画する
		if (!newBullet->active) continue;

		DrawGraph((int)newBullet->pos.x, (int)newBullet->pos.y, newBullet->handle, TRUE);
	}
}

void FinNewBullet()
{
	for (int i = 0; i < STRAIGHT_BULLET_TYPE_MAX; i++)
	{
		DeleteGraph(g_NewBulletHandle[i]);
	}
}

void FireNewBullet(StraightBulletType type, FireBulletData fireData, BulletCollisionTag tag)
{
	// 処理高速化のためポインタを使う
	StraightBulletData* bullet = g_NewBulletData;
	for (int i = 0; i < STRAIGHT_BULLET_MAX; i++, bullet++)
	{
		// 配列内で使われていないところを探す
		if (!bullet->active)
		{
			// 発射処理
			bullet->active = true;

			// 寿命設定
			bullet->life = fireData.life;

			// 画像設定
			bullet->handle = g_NewBulletHandle[type];

			// 座標設定
			bullet->pos.x = fireData.pos.x;
			bullet->pos.y = fireData.pos.y;

			// 移動量設定
			bullet->move.x = fireData.move.x;
			bullet->move.y = fireData.move.y;

			// 画像から幅と高さを取得
			int w, h;
			GetGraphSize(bullet->handle, &w, &h);
			bullet->width = (float)w;
			bullet->height = (float)h;

			// 画像が正方形の前提で半径を設定
			bullet->radius = bullet->width / 2;

			// 当たり判定タグ設定
			bullet->tag = tag;

			// 1発発射したら抜ける
			break;
		}
	}
}

StraightBulletData* GetNewBullet()
{
	return nullptr;
}

void NewBulletHitEnemy(int index)
{
}

void NewBulletHitPlayer(int index)
{
}
