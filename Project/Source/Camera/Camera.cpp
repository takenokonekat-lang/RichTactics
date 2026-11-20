#include "DxLib.h"
#include "Camera.h"
#include "../Input/Input.h"
#include "../Player/Player.h"
#include "../Weapon/Gun.h"
#include "../GameSetting/GameSetting.h"

#define DEBUG_CAMERA_SPEED (4.0f)
#define CAMERA_SCROLL_START_X (SCREEN_WIDTH/2-PLAYER_WIDTH)
#define CAMERA_SCROLL_START_Y (SCREEN_HEIGHT/2-PLAYER_HEIGHT)

CameraData g_CameraData = { 0 };

void InitCamera()
{
}

void StepCamera()
{
	// プレイヤー取得
	PlayerData player = GetPlayer();

	// プレイヤーがスクロールラインを超えたらカメラ移動する
	if (player.body.pos.x >= CAMERA_SCROLL_START_X)
	{
		// ラインを超えている分だけ移動する
		g_CameraData.pos.x = player.body.pos.x - CAMERA_SCROLL_START_X;
	}
	else
	{
		// 超えていなければ定位置
		g_CameraData.pos.x = 0.0f;
	}


	// プレイヤーがスクロールラインを超えたらカメラ移動する
	if (player.body.pos.y >= CAMERA_SCROLL_START_Y)
	{
		// ラインを超えている分だけ移動する
		g_CameraData.pos.y = player.body.pos.y - CAMERA_SCROLL_START_Y;
	}
	else
	{
		// 超えていなければ定位置
		g_CameraData.pos.y = 0.0f;
	}

}

void DrawCamera()
{
	
}

CameraData GetCamera()
{
	return g_CameraData;
}

