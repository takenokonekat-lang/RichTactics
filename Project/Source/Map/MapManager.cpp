#include "MapManager.h"
#include "MapParameter.h"
#include "MapChip.h"
#include "Block.h"
#include "../Collision/Collision.h"

// キャラクターの周囲何マスまでチェックするか
#define CHECK_ROUND_NUM (2)

void InitMap()
{
}

void LoadMap()
{
	LoadMapChipData();
	LoadBlock();
}

void StartMap()
{
	CreateMap();
}

void DrawMap()
{
	// ブロック描画
	DrawBlock();
}

void FinMap()
{
}

/// <summary>
/// マップとの当たり判定＆移動＆押し出し
/// </summary>
/// <param name="body">対象の当たり判定ボディ</param>
void MoveWithMapCollision(Body* body)
{
	// X軸だけ移動する
	body->pos.x += body->move.x;

	// マップチップと当たり判定＆押し出し
	for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
	{
		for (int x = 0; x < MAP_CHIP_X_NUM; x++)
		{
			// マップチップ取得
			MapChipData mapChipData = GetMapChipData(x, y);
			// マップチップが0の場合は何もない
			if (mapChipData.mapChip == 0) continue;

			// ブロックを取り出して当たり判定
			BlockData* block = mapChipData.data;
			// 当たり判定
			if (CheckSquareSquare(body->pos.x, body->pos.y, body->width, body->height,
				block->pos.x, block->pos.y, block->width, block->height))
			{
				// 左からあたったか
				if (body->move.x > 0.0f)
				{
					// 左に押し出す
					body->pos.x -= (body->pos.x + body->width) - block->pos.x;

				}
				// 右からあたったか
				else if (body->move.x < 0.0f)
				{
					// 右に押し出す
					body->pos.x = block->pos.x + block->width;

				}

				body->move.x = 0.0f;
			}

		}
	}

	// Y軸だけ移動する
	body->pos.y += body->move.y;

	// マップチップと当たり判定＆押し出し
	for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
	{
		for (int x = 0; x < MAP_CHIP_X_NUM; x++)
		{
			// マップチップ取得
			MapChipData mapChipData = GetMapChipData(x, y);
			// マップチップが0の場合は何もない
			if (mapChipData.mapChip == 0) continue;

			// ブロックを取り出して当たり判定
			BlockData* block = mapChipData.data;
			// 当たり判定
			if (CheckSquareSquare(body->pos.x, body->pos.y, body->width, body->height,
				block->pos.x, block->pos.y, block->width, block->height))
			{
				// 上からあたったか
				if (body->move.y > 0.0f)
				{
					// 上に押し出す
					body->pos.y -= (body->pos.y + body->height) - block->pos.y;
				}
				// 下からあたったか
				else if (body->move.y < 0.0f)
				{
					// 下に押し出す
					body->pos.y += (block->pos.y + block->height) - body->pos.y;

				}

				// 移動量は0にする
				body->move.y = 0.0f;
			}

		}
	}
}
