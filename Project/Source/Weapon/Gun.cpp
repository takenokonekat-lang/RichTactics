#include "DxLib.h"
#include "Gun.h"
#include "../Camera/Camera.h"
#include "../Player/Player.h"
#include "../Collision/Collision.h"

//ê›íËä÷òA
#define GUN_DEFAULT_POS_X	(200.0f)
#define GUN_DEFAULT_POS_Y	(400.0f)

#define PLAYER_DEFAULT_POS_X	(200.0f)
#define PLAYER_DEFAULT_POS_Y	(250.0f)

PlayerData g_PlayerPos = { 0 };
GunData g_GunData = { 0 };

void InitGun()
{
	g_GunData.pos.x = PLAYER_DEFAULT_POS_X;
	g_GunData.pos.y = PLAYER_DEFAULT_POS_Y;
	g_GunData.handle = 0;
	g_GunData.angle = 0.0f;
	g_GunData.width = 30.0f;
	g_GunData.height = 20.0f;
}

void LoadGun()
{
	g_GunData.handle = LoadGraph("Data/Wepon/NewRevolver.png");
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

	DrawRotaGraph((int)(g_GunData.pos.x - camera.pos.x), (int)(g_GunData.pos.y - camera.pos.y), 1.0, g_GunData.angle, g_GunData.handle, TRUE);

	VECTOR* vec = new VECTOR;
}

void FinGun()
{
	DeleteGraph(g_GunData.handle);
}

GunData GetGun()
{
	return g_GunData;
}