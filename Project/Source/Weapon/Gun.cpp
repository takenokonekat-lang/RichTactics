#include "DxLib.h"
#include "../Input/Input.h"
#include "Gun.h"
#include "../Camera/Camera.h"
#include "../Player/Player.h"
#include "../Collision/Collision.h"

//ê›íËä÷òA
#define NEW_REVOIVER_POS_X	(400.0f)
#define NEW_REVOIVER_POS_Y	(600.0f)

#define MACHINE_GUN_POS_X	(600.0f)
#define MACHINE_GUN_POS_Y	(600.0f)

PlayerData g_PlayerPos = { 0 };
GunData g_GunData = { 0 };

void InitGun()
{
	g_GunData.pos.x = NEW_REVOIVER_POS_X;
	g_GunData.pos.y = NEW_REVOIVER_POS_Y;
	g_GunData.angle = 0.0f;
	g_GunData.width = 30.0f;
	g_GunData.height = 20.0f;
	g_GunData.gunType = NEWREVOLVER;
}

void LoadGun()
{
	g_GunData.revolverhandle = LoadGraph("Data/Wepon/Revolver.png");
	g_GunData.newrevolverhandle = LoadGraph("Data/Wepon/NewRevolver.png");
	g_GunData.machinegunhandle = LoadGraph("Data/Wepon/machinegun.png");
	g_GunData.shotgunhandle = LoadGraph("Data/Wepon/shotgun.png");
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
	//ÉJÉÅÉâéÊìæ
	CameraData camera = GetCamera();

	int handle = 0;
	if (g_GunData.gunType == REVOLVER)
	{
		handle = g_GunData.revolverhandle;
	}
	if (g_GunData.gunType == NEWREVOLVER)
	{
		handle = g_GunData.newrevolverhandle;
	}
	if (g_GunData.gunType == MACHINE_GUN)
	{
		handle = g_GunData.machinegunhandle;
	}

	DrawRotaGraph((int)(g_GunData.pos.x - camera.pos.x), (int)(g_GunData.pos.y - camera.pos.y), 1.0, g_GunData.angle, handle, TRUE);

	VECTOR* vec = new VECTOR;
}

void FinGun()
{
	DeleteGraph(g_GunData.revolverhandle);
	DeleteGraph(g_GunData.newrevolverhandle);
	DeleteGraph(g_GunData.machinegunhandle);
	DeleteGraph(g_GunData.shotgunhandle);

}

void SpawnGun(float x, float y, GunType type)
{
}

void PickupGun()
{
}

GunData GetGun()
{
	return g_GunData;
}

void SetGun(GunType gunType)
{
	g_GunData.gunType = gunType;
}