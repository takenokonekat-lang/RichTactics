#include "DxLib.h"
#include "../../Scene/SceneManager.h"
#include "../../Map/MapManager.h"
#include "../../Player/Player.h"
#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"
#include "../../Bullet/BulletManager.h"


// 背景のスピード
#define BACK_GROUND_SPEED (-1.5f)

void InitPlayScene()
{
	// プレイヤー初期化
	InitPlayer();

	// マップ初期化
	InitMap();

	// カメラ初期化
	InitCamera();

	InitBullet();

	
}

void LoadPlayScene()
{
	// プレイヤーロード
	LoadPlayer();

	// マップをロード
	LoadMap();

	LoadBullet();
}

void StartPlayScene()
{
	// プレイヤー開始
	StartPlayer();

	// マップを開始
	StartMap();

}

void StepPlayScene()
{
	// プレイヤーステップ
	StepPlayer();

	// カメラステップ
	StepCamera();

	StepBullet();
}

void UpdatePlayScene()
{
	// マップ更新
	// プレイヤー更新
	UpdatePlayer();

	// 当たり判定
	CheckCollision();

	UpdateBullet();
}

void DrawPlayScene()
{
	// マップ描画
	DrawMap();

	// プレイヤー描画
	DrawPlayer();

	// カメラ描画
	DrawCamera();

	DrawBullet();
}

void FinPlayScene()
{
	// マップ終了
	FinMap();

	// プレイヤー終了
	FinPlayer();

	FinBullet();
}
