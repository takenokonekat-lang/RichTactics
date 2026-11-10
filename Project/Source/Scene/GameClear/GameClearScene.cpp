#include "DxLib.h"
#include "GameClearScene.h"
#include "../../BackGround/BackGround.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"
#include "../../Sound/SoundManager.h"
#include "../../UI/UIText.h"
#include <stdio.h>

// GameClear•¶š‰æ‘œ
int g_GameClearHandle = 0;

void InitGameClearScene()
{
	// ”wŒi‰Šú‰»
	InitBackGround();

	// •¶š‰æ‘œƒnƒ“ƒhƒ‹‰Šú‰»
	g_GameClearHandle = 0;
}

void LoadGameClearScene()
{
}

void StartGameClearScene()
{
}

void StepGameClearScene()
{
	// ”wŒiƒXƒeƒbƒv
	StepBackGround();

	// ZƒL[‚Åƒ^ƒCƒgƒ‹‚É–ß‚é
	if (IsTriggerKey(KEY_Z))
	{
		ChangeScene(SCENE_TITLE);
	}
}

void UpdateGameClearScene()
{
	// ”wŒiXV
	UpdateBackGround();
}

void DrawGameClearScene()
{
	// ”wŒi•`‰æ
	DrawBackGround();

	// •¶š‰æ‘œ•`‰æ
	DrawGraph(560, 110, g_GameClearHandle, TRUE);

	// ƒXƒRƒA•`‰æ
	DrawUIText();
}

void FinGameClearScene()
{
	// ”wŒiI—¹
	FinBackGround();

	// •¶š‰æ‘œíœ
	DeleteGraph(g_GameClearHandle);

	// BGM’â~
	StopBGM(BGM_GAME_CLEAR);
}
