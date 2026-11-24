#pragma once
#include "BulletParameter.h"

void InitSniperBullet();
void LoadSniperBullet();
void StepSniperBullet();
void UpdateSniperBullet();
void DrawSniperBullet();
void FinSniperBullet();

// 発射関数（必ずBulletManagerでしか呼ばないこと）
void FireSnaiperBullet(StraightBulletType type, FireBulletData fireData, BulletCollisionTag tag);

// 弾取得
StraightBulletData* GetSniperBullet();

// 当たり判定関連
void SniperBulletHitEnemy(int index);
void SniperBulletHitPlayer(int index);