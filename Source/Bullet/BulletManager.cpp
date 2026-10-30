#include "BulletManager.h"
#include "StraightBullet.h"

void InitBullet()
{
	// 各弾を初期化
	// 直線弾
	InitStraightBullet();
}

void LoadBullet()
{
	// 各弾をロード
	// 直線弾
	LoadStraightBullet();
}

void StepBullet()
{
	// 各弾をステップ
	// 直線弾
	StepStraightBullet();
}

void UpdateBullet()
{
	// 各弾を更新
	// 直線弾
	UpdateStraightBullet();
}

void DrawBullet()
{
	DrawStraightBullet();
}

void FinBullet()
{
	// 各弾を終了
	// 直線弾
	FinStraightBullet();
}

void FireBullet(BulletCategory category, int typeID, FireBulletData fireData, BulletCollisionTag tag)
{
	switch (category)
	{
	case BULLET_CATEGORY_OLD:
		BulletType type = (BulletType)typeID;
		FireStraightBullet(type, fireData, tag);
		break;
	}
}

//FireBulletData* CreateBullet(VECTOR pos)
//{
//	// 未使用のブロックを探す
//	FireBulletData* bullet = g_Bullet;
//	for (int i = 0; i < STRAIGHT_BULLET_MAX; i++, bullet++)
//	{
//		if (!bullet->active)
//		{
//			// ブロック生成
//			bullet->active = true;
//			bullet->handle = g_BulletHandle[type];
//			bullet->pos = pos;
//			return bullet;
//		}
//	}
//
//	return nullptr;
//}


