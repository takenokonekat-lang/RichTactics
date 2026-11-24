#pragma once
#include "BulletParameter.h"

// 各ロジックごとの関数
void InitMacinGunBullet();
void LoadMacinGunBullet();
void StepMacinGunBullet();
void UpdateMacinGunBullet();
void DrawMacinGunBullet();
void FinMacinGunBullet();

// 発射関数（必ずBulletManagerでしか呼ばないこと）
void FireMachineGunBullet(StraightBulletType type, FireBulletData fireData, BulletCollisionTag tag);

// 弾取得
StraightBulletData* GetMachineGunBullet();

// 当たり判定関連
void MachineGunBulletHitEnemy(int index);
void MachineGunBulletHitPlayer(int index);
