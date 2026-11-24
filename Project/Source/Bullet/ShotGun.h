#pragma once
#include "BulletParameter.h"

// 各ロジックごとの関数
void InitShotGunBullet();
void LoadShotGunBullet();
void StepShotGunBullet();
void UpdateShotGunBullet();
void DrawShotGunBullet();
void FinShotGunBullet();

// 発射関数（必ずBulletManagerでしか呼ばないこと）
void FireShotGunBullet(StraightBulletType type, FireBulletData fireData, BulletCollisionTag tag);

// 弾取得
StraightBulletData* GetShotGunBullet();

// 当たり判定関連
void ShotGunBulletHitEnemy(int index);
void ShotGunBulletHitPlayer(int index);