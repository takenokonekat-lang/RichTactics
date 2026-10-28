#include "DxLib.h"
#include "AnimationEffect.h"
#include "AnimationEffectParameter.h"
#include "../Animation/Animation.h"

#define ANIMATION_EFFECT_MAX 255

AnimationData g_AnimationEffectData[ANIMATION_EFFECT_MAX] = { 0 };

int g_AnimationEffectHandle[ANIMATION_EFFECT_TYPE_MAX];

void InitAnimationEffect()
{
	AnimationData* effect = g_AnimationEffectData;
	for (int i = 0; i < ANIMATION_EFFECT_MAX; i++, effect++)
	{
		InitAnimation(effect);
	}
}

void LoadAnimationEffect()
{
}

void StepAnimationEffect()
{
	AnimationData* effect = g_AnimationEffectData;
	for (int i = 0; i < ANIMATION_EFFECT_MAX; i++, effect++)
	{
		UpdateAnimation(effect);
	}
}

void UpdateAnimationEffect()
{
}

void DrawAnimationEffect()
{
	AnimationData* effect = g_AnimationEffectData;
	for (int i = 0; i < ANIMATION_EFFECT_MAX; i++, effect++)
	{
		DrawAnimation(effect);
	}
}

void FinAnimationEffect()
{
	for (int i = 0; i < ANIMATION_EFFECT_TYPE_MAX; i++)
	{
		DeleteGraph(g_AnimationEffectHandle[i]);
	}
}

void StartAnimationEffect(AnimationEffectType type, float posX, float posY, int interval, bool isLoop)
{
	// マスターパラム取得
	AnimationEffectMasterParameter param = ANIM_EFFECT_MASTER_PARAM[type];

	AnimationData* effect = g_AnimationEffectData;
	for (int i = 0; i < ANIMATION_EFFECT_MAX; i++, effect++)
	{
		// 未使用のエフェクトを探す
		if (!effect->active)
		{
			StartAnimation(effect, posX, posY, interval, param.frameNum, param.frameWidth, param.frameHeight, isLoop);
			break;
		}
	}
}
