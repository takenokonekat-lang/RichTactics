#pragma once

struct AnimationData
{
	bool active;
	bool isLoop;
	int handle;
	int nowFrame;
	int frameNum;
	int frameWidth;
	int frameHeight;
	int timer;
	int interval;
	VECTOR pos;
};

void InitAnimation(AnimationData* animData);
void UpdateAnimation(AnimationData* animData);
void DrawAnimation(AnimationData* animData, bool isTurnX = false, bool isTurnY = false);
void DrawAnimation(AnimationData* animData, VECTOR* pos, bool isTurnX = false, bool isTurnY = false);
void StartAnimation(AnimationData* animData, VECTOR pos, int interval, int frameNum, int width, int height, bool isLoop);
