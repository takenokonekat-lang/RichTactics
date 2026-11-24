#include "BulletManager.h"
#include "StraightBullet.h"
#include "machinegun.h"
#include "ShotGun.h"
#include "Sniper.h"

void InitBullet()
{
	// Še’e‚ğ‰Šú‰»
	// ’¼ü’e
	InitStraightBullet();
	InitMacinGunBullet();
	InitShotGunBullet();
	InitSniperBullet();
}

void LoadBullet()
{
	// Še’e‚ğƒ[ƒh
	// ’¼ü’e
	LoadStraightBullet();
}

void StepBullet()
{
	// Še’e‚ğƒXƒeƒbƒv
	// ’¼ü’e
	StepStraightBullet();
	StepMacinGunBullet();
	StepShotGunBullet();
	StepSniperBullet();
}

void UpdateBullet()
{
	// Še’e‚ğXV
	// ’¼ü’e
	UpdateStraightBullet();
	UpdateMacinGunBullet();
	UpdateShotGunBullet();
	UpdateSniperBullet();
}

void DrawBullet()
{
	// Še’e‚ğ•`‰æ
	// ’¼ü’e
	DrawStraightBullet();
	DrawMacinGunBullet();
	DrawShotGunBullet();
	DrawSniperBullet();
}

void FinBullet()
{
	// Še’e‚ğI—¹
	// ’¼ü’e
	FinStraightBullet();
	FinMacinGunBullet();
	FinShotGunBullet();
	FinSniperBullet();
}

void FireBullet(BulletCategory category, int typeID, FireBulletData fireData, BulletCollisionTag tag)
{
	switch (category)
	{
	case BULLET_CATEGORY_STRAIGHT:
		StraightBulletType type = (StraightBulletType)typeID;
		FireStraightBullet(type, fireData, tag);
		break;
	}
}
