#include "DxLib.h"
#include "machinegun.h"
#include "../Camera/Camera.h"

#define HIT_EFFECT_INTERVAL (1)
#define HIT_EFFECT_OFFSET_X (-10.0f)
#define HIT_EFFECT_OFFSET_Y (-10.0f)

struct StraightBulletData g_MachineBulletData[STRAIGHT_BULLET_MAX] = { 0 };

int g_MachineBulletHandle[STRAIGHT_BULLET_TYPE_MAX] = { 0 };

// 各弾丸の半径データ
float g_MachineBulletRadius[STRAIGHT_BULLET_TYPE_MAX] = { 0 };

void InitMacinGunBullet()
{
	StraightBulletData* machineBullet = g_MachineBulletData;
	for (int i = 0; i < STRAIGHT_BULLET_MAX; i++)
	{
		machineBullet->handle = 0;
		machineBullet->life = 0;
		machineBullet->pos.x = 0.0f;
		machineBullet->pos.y = 0.0f;
		machineBullet->move.x = 0.0f;
		machineBullet->move.y = 0.0f;
		machineBullet->radius = 0.0f;
		machineBullet->active = false;
	}

	for (int i = 0; i < STRAIGHT_BULLET_TYPE_MAX; i++)
	{
		g_MachineBulletHandle[i] = 0;
	}
}

void LoadMacinGunBullet()
{
}

void StepMacinGunBullet()
{
	StraightBulletData* machineBullet = g_MachineBulletData;
	for (int i = 0; i < STRAIGHT_BULLET_MAX; i++, machineBullet++)
	{
		// 生きている弾だけ処理をする
		if (!machineBullet->active)continue;

		// 寿命を減らす
		machineBullet->life--;
	}
}

void UpdateMacinGunBullet()
{
	StraightBulletData* machineBullet = g_MachineBulletData;
	for (int i = 0; i < STRAIGHT_BULLET_MAX; i++, machineBullet++)
	{
		if (!machineBullet->active)continue;

		machineBullet->pos.x += machineBullet->move.x;
		machineBullet->pos.y += machineBullet->move.y;

		if (machineBullet->life <= 0)
		{
			machineBullet->active = false;
		}
	}
}

void DrawMacinGunBullet()
{
	// 処理高速化のためポインタを使う
	StraightBulletData* machineBullet = g_MachineBulletData;
	for (int i = 0; i < STRAIGHT_BULLET_MAX; i++, machineBullet++)
	{
		// 生きている弾だけ描画する
		if (!machineBullet->active) continue;

		DrawGraph((int)machineBullet->pos.x, (int)machineBullet->pos.y, machineBullet->handle, TRUE);
	}
}

void FinMacinGunBullet()
{
	for (int i = 0; i < STRAIGHT_BULLET_TYPE_MAX; i++)
	{
		DeleteGraph(g_MachineBulletHandle[i]);
	}
}

void FireMachineGunBullet(StraightBulletType type, FireBulletData fireData, BulletCollisionTag tag)
{
	// 処理高速化のためポインタを使う
	StraightBulletData* machineBullet = g_MachineBulletData;
	for (int i = 0; i < STRAIGHT_BULLET_MAX; i++, machineBullet++)
	{
		// 配列内で使われていないところを探す
		if (!machineBullet->active)
		{
			// 発射処理
			machineBullet->active = true;

			// 寿命設定
			machineBullet->life = fireData.life;

			// 画像設定
			machineBullet->handle = g_MachineBulletHandle[type];

			// 座標設定
			machineBullet->pos.x = fireData.pos.x;
			machineBullet->pos.y = fireData.pos.y;

			// 移動量設定
			machineBullet->move.x = fireData.move.x;
			machineBullet->move.y = fireData.move.y;

			// 画像から幅と高さを取得
			int w, h;
			GetGraphSize(machineBullet->handle, &w, &h);
			machineBullet->width = (float)w;
			machineBullet->height = (float)h;

			// 画像が正方形の前提で半径を設定
			machineBullet->radius = machineBullet->width / 2;

			// 当たり判定タグ設定
			machineBullet->tag = tag;

			// 1発発射したら抜ける
			break;
		}
	}
}

StraightBulletData* GetMachineGunBullet()
{
	return nullptr;
}

void MachineGunBulletHitEnemy(int index)
{
}

void MachineGunBulletHitPlayer(int index)
{
}
