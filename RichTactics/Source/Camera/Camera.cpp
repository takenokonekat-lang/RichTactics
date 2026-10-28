#include "DxLib.h"
#include "Camera.h"
#include "../Input/Input.h"
#include "../Player/Player.h"

#define DEBUG_CAMERA_SPEED (4.0f)
#define CAMERA_SCROLL_START_X (200.0f)

CameraData g_CameraData = { 0 };

void InitCamera()
{
}

void StepCamera()
{
	// プレイヤー取得
	PlayerData player = GetPlayer();

	// プレイヤーがスクロールラインを超えたらカメラ移動する
	if (player.posX >= CAMERA_SCROLL_START_X)
	{
		// ラインを超えている分だけ移動する
		g_CameraData.posX = player.posX - CAMERA_SCROLL_START_X;
	}
	else
	{
		// 超えていなければ定位置
		g_CameraData.posX = 0.0f;
	}
}

void DrawCamera()
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "カメラの座標＝[%f, %f]", g_CameraData.posX, g_CameraData.posY);
}

CameraData GetCamera()
{
	return g_CameraData;
}

