#pragma once
#include "../Collision/Collision.h"
#include "../Collision/CollisionParameter.h"

struct GunData
{
	VECTOR pos;
	VECTOR vec;
	int handle;	//画像ハンドル
	float angle;
	bool isTurn;
	int width;
	int height;
};

enum GunType
{
	OLD_GUN,
	NEW_GUN,
	WEPON_NONE=-1
};

//各シーンの状態別処理
void InitGun();
void LoadGun();
void StartGun();
void StepGun();
void UpdateGun();
void DrawGun();
void FinGun();

GunData GetGun();