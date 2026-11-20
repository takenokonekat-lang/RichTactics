#include "DxLib.h"
#include "../Input/Input.h"
#include "Gun.h"
#include "../Camera/Camera.h"
#include "../Player/Player.h"
#include "../Collision/Collision.h"
#include "../Common/Common.h"
#include <math.h>

#define WORLD_GUN_MAX 100
#define INVENTORY_MAX 2

GunData g_WorldGun[WORLD_GUN_MAX];

GunResource g_GunRes;

int g_EquipIndex = 0;

PlayerData g_PlayerPos = { 0 };
GunData g_GunData = { 0 };

void InitGun()
{
	for (int i = 0; i < WORLD_GUN_MAX; i++)
	{
		g_WorldGun[i].isUse = false;
	}

	SpawnGun(500, 600,NEWREVOLVER);
	SpawnGun(400, 600, MACHINE_GUN);
	SpawnGun(600, 600, SNIPER);
	SpawnGun(700, 600, SHOT_GUN);
}

void LoadGun()
{
	g_GunRes.revolverhandle = LoadGraph("Data/Wepon/Revolver.png");
	g_GunRes.newrevolverhandle = LoadGraph("Data/Wepon/NewRevolver.png");
	g_GunRes.machinegunhandle = LoadGraph("Data/Wepon/MachineGun.png");
	g_GunRes.sniperRiflehandle = LoadGraph("Data/Wepon/SniperRifle.png");
	g_GunRes.shotgunhandle = LoadGraph("Data/Wepon/ShotGun.png");
}

void StartGun()
{
}

void StepGun()
{
}

void UpdateGun()
{
}

void DrawGun()
{
	//ƒJƒƒ‰Žæ“¾
	CameraData camera = GetCamera();

	for (int i = 0; i < WORLD_GUN_MAX; i++)
	{
		if (!g_WorldGun[i].isUse)
		{
			continue;
		}
		int handle = -1;

		switch (g_WorldGun[i].gunType)
		{
		case REVOLVER:
			handle = g_GunRes.revolverhandle;
			break;
		case NEWREVOLVER:
			handle = g_GunRes.newrevolverhandle;
			break;
		case MACHINE_GUN:
			handle = g_GunRes.machinegunhandle;
			break;
		case SNIPER:
			handle = g_GunRes.sniperRiflehandle;
			break;
		case SHOT_GUN:
			handle = g_GunRes.shotgunhandle;
			break;
		}
		DrawRotaGraph((int)(g_WorldGun[i].pos.x -camera.pos.x),(int)(g_WorldGun[i].pos.y -camera.pos.y),
			1.0f,g_WorldGun[i].angle,handle,TRUE);
	}
}

void FinGun()
{
	DeleteGraph(g_GunRes.revolverhandle);
	DeleteGraph(g_GunRes.newrevolverhandle);
	DeleteGraph(g_GunRes.machinegunhandle);
	DeleteGraph(g_GunRes.sniperRiflehandle);
	DeleteGraph(g_GunRes.shotgunhandle);

}

void SpawnGun(float x, float y, GunType type)
{
	for (int i = 0; i < WORLD_GUN_MAX; i++)
	{
		if (!g_WorldGun[i].isUse)
		{
			g_WorldGun[i].isUse = true;

			g_WorldGun[i].pos.x = x;
			g_WorldGun[i].pos.y = y;

			g_WorldGun[i].angle = 0.0f;
			g_WorldGun[i].gunType = type;

			break;
		}
	}
}

GunType PickupGun()
{
	PlayerData player = GetPlayer();

	for (int i = 0; i < WORLD_GUN_MAX; i++)
	{
		if (!g_WorldGun[i].isUse)
		{
			continue;
		}

		float dx =
			player.body.pos.x -
			g_WorldGun[i].pos.x;

		float dy =
			player.body.pos.y -
			g_WorldGun[i].pos.y;

		float dist =
			sqrtf(dx * dx + dy * dy);

		if (dist < 50.0f)
		{
			g_WorldGun[i].isUse = false;
			return g_WorldGun[i].gunType;
		}
	}
	return GUN_NONE;
}

//ŠŽ‚µ‚Ä‚¢‚ée
GunData GetGun()
{
	return g_GunData;
}

GunResource GunRes()
{
	return g_GunRes;
}

void SetGun(GunType gunType)
{
	g_GunData.gunType = gunType;
}

void DrawEquipGun(VECTOR playerPos)
{
	PlayerData player = GetPlayer();

	if (!player.active)return;

	CameraData camera = GetCamera();

	//‘•”õ’†‚Ì•Ší
	int handle = -1;

	//‘•”õ’†‚Ì•Ší‚É‰ž‚¶‚½‰æ‘œ
	switch (player.GUNTYPE)
	{
	case REVOLVER:
		handle = g_GunRes.revolverhandle;
		break;
	case NEWREVOLVER:
		handle = g_GunRes.newrevolverhandle;
		break;
	case MACHINE_GUN:
		handle = g_GunRes.machinegunhandle;
		break;
	case SNIPER:
		handle = g_GunRes.sniperRiflehandle;
		break;
	case SHOT_GUN:
		handle = g_GunRes.shotgunhandle;
		break;

	default:
		return;
	}

	if (handle < 0)return;

	VECTOR gunPos;

	if (!player.isTurn)
	{
		gunPos = VGet(25, 15, 0);
	}
	else
	{
		gunPos = VGet(-25, 15, 0);
	}

	//ƒvƒŒƒCƒ„[‚ÌŒü‚¢‚Ä‚¢‚é•ûŒü
	float angle = AngleToRadian(player.angle);

	//•Ší‚Ì•`‰æ
	DrawRotaGraph(
		(int)(playerPos.x + gunPos.x - camera.pos.x),
		(int)(playerPos.y + gunPos.y - camera.pos.y),
		1.0f,angle,handle,TRUE,FALSE);
}