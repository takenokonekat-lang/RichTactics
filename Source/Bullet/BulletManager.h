#pragma once
#include "BulletPrameter.h"

// íeëSëÃÇÃèàóù
void InitBullet();
void LoadBullet();
void StepBullet();
void UpdateBullet();
void DrawBullet();
void FinBullet();

// íeî≠éÀ
void FireBullet(BulletCategory category, int typeID, FireBulletData fireData, BulletCollisionTag tag);

//FireBulletData* CreateBullet(VECTOR pos);
