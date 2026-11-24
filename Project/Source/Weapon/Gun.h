#pragma once
#include "../Collision/Collision.h"
#include "../Collision/CollisionParameter.h"

#define GUN_PICKUP_RANGE 65.0f

enum GunType
{
	REVOLVER,
	NEWREVOLVER,
	MACHINE_GUN,
	SNIPER,
	SHOT_GUN,
	GUN_NONE,
	GUN_MAX
};

struct GunData
{
	bool isUse;
	VECTOR pos;
	VECTOR vec;
	float angle;
	bool isTurn;
	float width;
	float height;
	GunType gunType;
};

struct GunResource
{
	int revolverhandle;
	int newrevolverhandle;
	int machinegunhandle;
	int sniperRiflehandle;
	int shotgunhandle;
};

void SpawnGun(float x, float y, GunType type);
GunType PickupGun();

//äeÉVÅ[ÉìÇÃèÛë‘ï èàóù
void InitGun();
void LoadGun();
void StartGun();
void StepGun();
void UpdateGun();
void DrawGun();
void FinGun();

GunData GetGun();
GunResource GunRes();
void SetGun(GunType gunType);

void DrawEquipGun(VECTOR playerPos);