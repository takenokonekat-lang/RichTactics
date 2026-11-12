#pragma once
#include "../Collision/Collision.h"
#include "../Collision/CollisionParameter.h"

enum GunType
{
	REVOLVER,
	NEWREVOLVER,
	MACHINE_GUN,
	SNIPER,
	SHOT_GUN,
	WEPON_NONE = -1
};

struct GunData
{
	VECTOR pos;
	VECTOR vec;
	//int handle;	//画像ハンドル
	int revolverhandle;
	int newrevolverhandle;
	int machinegunhandle;
	int sniperRiflehandle;
	int shotgunhandle;
	float angle;
	bool isTurn;
	float width;
	float height;
	GunType gunType;
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
void SetGun(GunType gunType);
