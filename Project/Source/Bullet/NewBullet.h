#pragma once
#include "BulletParameter.h"

// 各ロジックごとの関数
void InitNewBullet();
void LoadNewBullet();
void StepNewBullet();
void UpdateNewBullet();
void DrawNewBullert();
void FinNewBullet();

// 発射関数（必ずBulletManagerでしか呼ばないこと）
void FireNewBullet(StraightBulletType type, FireBulletData fireData, BulletCollisionTag tag);

// 弾取得
StraightBulletData* GetNewBullet();

// 当たり判定関連
void NewBulletHitEnemy(int index);
void NewBulletHitPlayer(int index);
