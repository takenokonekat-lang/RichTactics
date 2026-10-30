#include "DxLib.h"
#include "Animation.h"

void InitAnimation(AnimationData* animData)
{
	animData->active = false;
	animData->handle = 0;
	animData->nowFrame = 0;
	animData->frameNum = 0;
	animData->frameWidth = 0;
	animData->frameHeight = 0;
	animData->timer = 0;
	animData->pos.x = 0.0f;
	animData->pos.y = 0.0f;
}

void StartAnimation(AnimationData* animData, VECTOR pos, int interval, int frameNum, int width, int height, bool isLoop)
{
	// アクティブ
	animData->active = true;

	// 座標設定
	animData->pos.x = pos.x;
	animData->pos.y = pos.y;

	// インターバル設定
	animData->interval = interval;

	// タイマー設定
	animData->timer = animData->interval;

	// 0コマ目にする
	animData->nowFrame = 0;

	// コマ数
	animData->frameNum = frameNum;

	// 1コマのサイズ
	animData->frameWidth = width;
	animData->frameHeight = height;

	// ループするか
	animData->isLoop = isLoop;
}

void UpdateAnimation(AnimationData* animData)
{
	if (!animData->active) return;

	// 次のコマへ進めるか
	if (animData->timer <= 0)
	{
		// 次のコマへ
		animData->nowFrame++;
		// タイマーリセット
		animData->timer = animData->interval;

		// 全コマ表示し終わったら非アクティブ
		if (animData->nowFrame >= animData->frameNum)
		{
			// ループするかどうか
			if (animData->isLoop)
			{
				// 0コマ目に戻る
				animData->nowFrame = 0;
			}
			else
			{
				// 終了
				animData->active = false;
			}

			// これ以上処理する必要なし
			return;
		}
	}

	animData->timer--;
}

void DrawAnimation(AnimationData* animData, bool isTurnX, bool isTurnY)
{
	DrawAnimation(animData, animData->pos.x, animData->pos.y);
}


void DrawAnimation(AnimationData* animData, VECTOR* pos, bool isTurnX, bool isTurnY)
{
	if (!animData->active)return;

	// コマの切り取り位置計算（横１行の画像の想定）
	int frameX = animData->frameWidth * animData->nowFrame;

	// 画像から表示するコマの部分だけ描画
	DrawRectGraph((int)pos->x, (int)pos->y, frameX, 0, animData->frameWidth, animData->frameHeight, animData->handle, TRUE, isTurnX, isTurnY);
}
