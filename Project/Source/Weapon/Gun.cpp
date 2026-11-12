#include "DxLib.h"
#include "../Input/Input.h"
#include "Gun.h"
#include "../Camera/Camera.h"
#include "../Player/Player.h"
#include "../Collision/Collision.h"

//設定関連
#define GUN_DEFAULT_POS_X	(400.0f)
#define GUN_DEFAULT_POS_Y	(600.0f)

#define PLAYER_DEFAULT_POS_X	(200.0f)
#define PLAYER_DEFAULT_POS_Y	(250.0f)

PlayerData g_PlayerPos = { 0 };
GunData g_GunData = { 0 };

void InitGun()
{
	g_GunData.pos.x = GUN_DEFAULT_POS_X;
	g_GunData.pos.y = GUN_DEFAULT_POS_Y;
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



	/*
	// リボルバーに切替
	g_GunData.handle = g_GunData.revolverhandle;

	// ニューリボルバーに切替
	g_GunData.handle = g_GunData.newrevolverhandle;
	*/
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
	//カメラ取得
	CameraData camera = GetCamera();

	int handle = 0;
	if (g_GunData.gunType == REVOLVER)
	{
		handle = g_GunData.revolverhandle;
	}
	else if (g_GunData.gunType == NEWREVOLVER)
	{
		handle = g_GunData.newrevolverhandle;
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

GunData GetGun()
{
	return g_GunData;
}

void SetGun(GunType gunType)
{
	g_GunData.gunType = gunType;
}