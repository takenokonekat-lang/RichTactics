#include "DxLib.h"
#include "SoundManager.h"

int g_BGMHandle[BGM_TYPE_MAX] = { 0 };
int g_SEHandle[SE_TYPE_MAX] = { 0 };

// BGMロード
void LoadBGM()
{
	g_SEHandle[Wepon_Get] = LoadSoundMem("Data/Sound/SE/WeponGet.ogg");
}

// BGM再生
void PlayBGM(BGMType type)
{
	PlaySoundMem(g_BGMHandle[type], DX_PLAYTYPE_LOOP);
}

// BGM停止
void StopBGM(BGMType type)
{
	StopSoundMem(g_BGMHandle[type]);
}

// BGM終了
void FinBGM()
{
	for (int i = 0; i < BGM_TYPE_MAX; i++)
	{
		DeleteSoundMem(g_BGMHandle[i]);
	}
}


// SEロード
void LoadSE()
{
	g_SEHandle[Wepon_Get] = LoadSoundMem("Data/Sound/SE/WeponGet.ogg");
}

// SE再生
void PlaySE(SEType type)
{
	PlaySoundMem(g_SEHandle[type], DX_PLAYTYPE_BACK);
}

// SE終了
void FinSE()
{
	for (int i = 0; i < SE_TYPE_MAX; i++)
	{
		DeleteSoundMem(g_SEHandle[i]);
	}
}
