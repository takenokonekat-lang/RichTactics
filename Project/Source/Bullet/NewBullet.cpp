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

}

void LoadNewBullet()
{
}

void StepNewBullet()
{
}

void UpdateNewBullet()
{
}

void DrawNewBullert()
{
}

void FinNewBullet()
{
}

void FireNewBullet(StraightBulletType type, FireBulletData fireData, BulletCollisionTag tag)
{
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
