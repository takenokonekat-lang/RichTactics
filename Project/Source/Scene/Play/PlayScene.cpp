#include "DxLib.h"
#include "../../Player/Player.h"
#include "../../Scene/SceneManager.h"
#include "../../Bullet/BulletManager.h"
#include "../../Enemy/EnemyManager.h"
#include "../../Sound/SoundManager.h"
#include "../../Collision/Collision.h"
#include "../../Effect/AnimationEffect.h"
#include "../../UI/UIImage.h"
#include "../../UI/UIText.h"
#include "../../Score/Score.h"
#include "../../Camera/Camera.h"
#include "../../Map/MapManager.h"
#include "../../Weapon/Gun.h"

// 背景のスピード
#define BACK_GROUND_SPEED (-1.5f)

void InitPlayScene()
{
	// プレイヤー初期化
	InitPlayer();

	// 弾を初期化
	InitBullet();

	// エフェクトを初期化
	InitAnimationEffect();

	InitCamera();

	InitMap();

	InitGun();
}

void LoadPlayScene()
{
	// プレイヤーロード
	LoadPlayer();


	// 弾ロード
	LoadBullet();

	// エフェクトロード
	LoadAnimationEffect();

	LoadMap();

	LoadGun();
}

void StartPlayScene()
{
	// プレイヤー開始
	StartPlayer();

	// BGM再生
	PlayBGM(BGM_PLAY);

	StartMap();

	StartGun();
}

void StepPlayScene()
{
	// プレイヤーステップ
	StepPlayer();

	// 弾ステップ
	StepBullet();

	// エフェクトをステップ
	StepAnimationEffect();

	// UIステップ
	StepUIImage();
	StepUIText();

	StepCamera();
}

void UpdatePlayScene()
{
	// プレイヤー更新
	UpdatePlayer();

	// 弾更新
	UpdateBullet();

	// エフェクト更新
	UpdateAnimationEffect();

	// UI更新
	UpdateUIImage();
	UpdateUIText();
	
	// 当たり判定
	CheckCollision();
}

void DrawPlayScene()
{
	// 弾描画
	DrawBullet();

	// プレイヤー描画
	DrawPlayer();

	// エフェクト描画
	DrawAnimationEffect();

	// UI描画
	DrawUIImage();
	DrawUIText();

	DrawCamera();

	DrawMap();

	DrawGun();
}

void FinPlayScene()
{
	// プレイヤー終了
	FinPlayer();

	// 弾終了
	FinBullet();

	// エフェクト終了
	FinAnimationEffect();

	// UIリセット
	ResetUIImage();
	ResetUIText();

	// BGM停止
	StopBGM(BGM_PLAY);

	FinMap();

	FinGun();
}
