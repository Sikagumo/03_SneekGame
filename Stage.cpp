/* ------------
　ステージ処理
 -------------- */

#include <DxLib.h>
#include "main.h"
#include "SceneGame.h"
#include "Stage.h"
#include "Player.h"

/* --- <<< 定数宣言 >>> --- */

const int SEED_MAX = 7;	//生成ブロック 画像数a
const int SEED_STEP = (MAPCHIP_SIZE_X * 1); //生成ブロック成長 フレーム数

const int BLOCK_FLOOR = 0; //床
const int BLOCK_SEED  = 1; //生成ブロック
const int BLOCK_WALL  = (SEED_STEP * SEED_MAX); //壁


/* --- <<< 変数宣言 >>> --- */

int wallImage; //壁画像
int seedImages[SEED_MAX]; //生成ブロック画像

int map[BLOCK_COUNT_Y][BLOCK_COUNT_X]; //マップ情報


/* --- <<< 処理系 >>> --- */

/* <<< ステージ初期化処理 >>> */
bool StageInit()
{
	/* << グラフィック登録 >> */

	/*  壁画像 読み込み  */
	wallImage = LoadGraph("image/wall.png");
	if (wallImage == -1)
	{
		OutputDebugString("壁画像が描画されていません。");
		return false;
	}

	/*  レンガ画像 読み込み  */
	int seed = LoadDivGraph("image/block.png",
							SEED_MAX,
							SEED_MAX,1,
							MAPCHIP_SIZE_X, MAPCHIP_SIZE_Y,
							&(seedImages[0]));
	if (seed == -1)
	{
		OutputDebugString("ブロック画像が描画されていません。");
		return false;
	}

	StageReStart(); //初期化処理

	return true;
}


/* <<< ステージ再初期化処理 >>> */
void StageReStart()
{
	for (int y = 0; y < BLOCK_COUNT_Y; y++)
	{
		for (int x = 0; x < BLOCK_COUNT_X; x++)
		{
			map[y][x] = BLOCK_FLOOR; //床情報 取得

			if (x == 0 || x == (BLOCK_COUNT_X - 1) ||
				y == 0 || y == (BLOCK_COUNT_Y - 1))
			{
				map[y][x] = BLOCK_WALL;  //壁情報 取得
			}

		}
	}
}

/* <<< ステージ更新処理 >>> */
void StageUpdate()
{
	for (int y = 0; y < BLOCK_COUNT_Y; y++)
	{
		for (int x = 0; x < BLOCK_COUNT_X; x++)
		{
			if (map[y][x] >= BLOCK_SEED)
			{
				if (map[y][x] < (SEED_STEP * (SEED_MAX - 1)))
				{
					map[y][x]++; //ブロック成長処理
				}
			}
		}
	}
}


/* <<< ステージ描画処理 >>> */
void StageDraw()
{
	for (int y = 0; y < BLOCK_COUNT_Y; y++)
	{
		for (int x = 0; x < BLOCK_COUNT_X; x++)
		{
			/*  床描画  */
			if ((x + y) % 2 == 1)
			{
				//■描画
				DrawBox(x * MAPCHIP_SIZE_X, y * MAPCHIP_SIZE_Y,
					   (x + 1) * MAPCHIP_SIZE_X, (y + 1) * MAPCHIP_SIZE_Y,
						0x444444, true);
			}

			
			if (map[y][x] == BLOCK_FLOOR)
			{

			}
			/*  壁描画  */
			if (map[y][x] == BLOCK_WALL)
			{
				DrawGraph(x * MAPCHIP_SIZE_X, y * MAPCHIP_SIZE_Y, wallImage, true);
			}

			/*　生成ブロック描画　*/
			else if (map[y][x] >= BLOCK_SEED)
			{
				int seedImageNo;
				seedImageNo = (map[y][x] / SEED_STEP);

				//画像位置制限
				if (seedImageNo > (SEED_MAX - 1))
				{
					seedImageNo = (SEED_MAX - 1);
				}
				
				//生成ブロック 描画
				DrawGraph(x * MAPCHIP_SIZE_X, y * MAPCHIP_SIZE_Y, seedImages[seedImageNo], true);
			}
		}
	}
}


/* <<< ステージ画像 解放処理 >>> */
void StageRelease()
{
	for (int sd = 0; sd < SEED_MAX; sd++)
	{
		DeleteGraph(seedImages[sd]);
	}

	DeleteGraph(wallImage);
}


POS PosToIndex(POS playerPos)
{
	POS index;
	index.x = playerPos.x / MAPCHIP_SIZE_X;
	index.y = playerPos.y / MAPCHIP_SIZE_Y;

	return index;
}


/* << 指定された場所に種を蒔く >> */
void SetSeed(POS centerPos)
{
	POS index = PosToIndex(centerPos);


	/*  種蒔き不可化  */
	if (index.x < 0 || index.x > (BLOCK_COUNT_X - 1) ||
		index.y < 0 || index.y > (BLOCK_COUNT_Y - 1))
	{
		return;
	}

	/*  種蒔き可能状態  */
	if (map[index.y][index.x] == BLOCK_FLOOR)
	{
		map[index.y][index.x] = BLOCK_SEED;
	}
}

bool IsCollision(POS centerPos)
{
	POS index;
	index = PosToIndex(centerPos); //中央座標取得

	//衝突時
	if (map[index.y][index.x] >= (SEED_STEP * (SEED_MAX - 1)))
	{
		return true;
	}

	return false;
}