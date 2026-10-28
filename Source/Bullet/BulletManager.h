#pragma once
#include "BulletPrameter.h"

// ’e‘S‘Ì‚Ìˆ—
void InitBullet();
void LoadBullet();
void StepBullet();
void UpdateBullet();
void DrawBullet();
void FinBullet();

// ’e”­Ë
void FireBullet(BulletCategory category, int typeID, FireBulletData fireData, BulletCollisionTag tag);
