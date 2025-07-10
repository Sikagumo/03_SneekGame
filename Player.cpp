/* --------------
　プレイヤー処理
 ---------------- */
#include <DxLib.h>
#include "main.h"
#include "Player.h"
#include "Stage.h"


/* --- <<< 変数宣言 >>> --- */

int playerCounter; //プレイヤーのアニメーションループカウンタ
int playersImages[
				  static_cast<int>(PLAYER_TYPE::MAX)][
				  PLAYER_ANI_MAX * static_cast<int>(DIR::MAX)]; //プレイヤー 画像

PLAYER player1;	//プレイヤー１の情報
PLAYER player2;	//プレイヤー２の情報

RESULT result; //ゲーム勝敗

//勝敗結果 取得処理
RESULT GetPlayerResult()
{
	return result;
}


/* --- <<< 処理系 >>> --- */

/* <<< 初回プレイヤー初期化処理 >>> */
bool PlayerInit()
{
	/* << グラフィック登録 >> */

	const char* filename[static_cast<int>(PLAYER_TYPE::MAX)]
	{
		"image/Male 05-1.png",
		"image/Female 05-2.png",
		"image/Enemy 15-3.png",
		"image/Enemy 15-4.png",
		"image/Enemy 09-1.png",
	};

	/*  プレイヤー画像登録処理  */
	for (int pl = 0; pl < static_cast<int>(PLAYER_TYPE::MAX); pl++)
	{
		int player = LoadDivGraph(filename[pl],
								 (PLAYER_ANI_MAX * static_cast<int>(DIR::MAX)),
								  PLAYER_ANI_MAX, static_cast<int>(DIR::MAX),
								  PLAYER_SIZE_X, PLAYER_SIZE_Y,
								 &(playersImages[pl][0]));

		if (player == -1)
		{
			OutputDebugString("プレイヤー画像が読み込まれていません");
			return false;
		}
	}

	PlayerReStart(); //初期化処理

	return true;
}


/* <<< プレイヤー再開時初期化処理 >>> */
void PlayerReStart()
{
	playerCounter = 0;
	result = RESULT::NONE; //勝敗なし


	/*　プレイヤー１初期化　*/
	player1.pos.x = (5 * MAPCHIP_SIZE_X); //プレイヤー１ 横位置
	player1.pos.y = (7 * MAPCHIP_SIZE_Y); //プレイヤー１ 縦位置
	player1.dir   = DIR::RIGHT;			  //プレイヤー１ 向き
	player1.type  = PLAYER_TYPE::FEMALE;  //プレイヤー１ 見た目
	player1.keyCode[static_cast<int>(DIR::LEFT)]  = KEY_INPUT_LEFT;
	player1.keyCode[static_cast<int>(DIR::RIGHT)] = KEY_INPUT_RIGHT;
	player1.keyCode[static_cast<int>(DIR::UP)]	  = KEY_INPUT_UP;
	player1.keyCode[static_cast<int>(DIR::DOWN)]  = KEY_INPUT_DOWN;

	/*　プレイヤー２初期化　*/
	player2.pos.x = (19 * MAPCHIP_SIZE_X); //プレイヤー２ 横位置
	player2.pos.y = (7 * MAPCHIP_SIZE_Y);  //プレイヤー２ 縦位置
	player2.dir   = DIR::LEFT;			   //プレイヤー２ 向き
	player2.type  = PLAYER_TYPE::MALE;	   //プレイヤー２ 見た目
	player2.keyCode[static_cast<int>(DIR::UP)]	  = KEY_INPUT_W;
	player2.keyCode[static_cast<int>(DIR::LEFT)]  = KEY_INPUT_A;
	player2.keyCode[static_cast<int>(DIR::DOWN)]  = KEY_INPUT_S;
	player2.keyCode[static_cast<int>(DIR::RIGHT)] = KEY_INPUT_D;
}


/* <<< プレイヤー更新処理 >>> */
bool PlayerUpdate()
{
	playerCounter++; //プレイヤー画像アニメーション値 増加

	/*  ゲームパッド入力  */

	//複数接続時
	if (GetJoypadNum() >= 2)
	{
		JoyPadUpdate(&(player1)); //プレイヤー１操作
		JoyPadUpdate(&(player2)); //プレイヤー２操作
	}

	//単体接続時
	else if (GetJoypadNum() == 1)
	{
		PlayerControll(&(player1)); //プレイヤー１操作
		JoyPadUpdate(&(player2));	//プレイヤー２操作
	}

	//未接続時
	else if (GetJoypadNum() == 0)
	{
		PlayerControll(&(player1)); //プレイヤー１操作
		PlayerControll(&(player2)); //プレイヤー２操作
	}


	/* << プレイヤー当たり判定 >> */

	bool player1Collision; //プレイヤー１
	bool player2Collision; //プレイヤー２

	//プレイヤー１当たり判定取得
	player1Collision = IsCollision(GetCenterPos(player1.pos));

	//プレイヤー２当たり判定取得
	player2Collision = IsCollision(GetCenterPos(player2.pos));

	/*　通常状態　*/
	if (player1Collision == false && player2Collision == false)
	{
		result = RESULT::NONE;
	}
	/*　プレイヤー１勝利　*/
	else if (player1Collision == false && player2Collision == true)
	{
		result = RESULT::PLAYER1WIN;
		return false;
	}
	/*　プレイヤー２勝利　*/
	else if (player1Collision == true && player2Collision == false)
	{
		result = RESULT::PLAYER2WIN;
		return false;
	}
	/*　引き分け　*/
	else if (player1Collision == true && player2Collision == true)
	{
		result = RESULT::DRAW;
		return false;
	}

	return true;
}

/* <<< 各プレイヤー操作処理 >>> */
void PlayerControll(PLAYER* player)
{
	/* << 向き回転 >> */

	if ((player->pos.x % MAPCHIP_SIZE_X) == 0 &&
		(player->pos.y % MAPCHIP_SIZE_Y) == 0)
	{
		/*  左向き  */
		if (CheckHitKey(player->keyCode[static_cast<int>(DIR::LEFT)]) == 1 &&
			player->dir != DIR::RIGHT)
		{
			player->dir = DIR::LEFT;
		}

		/*  右向き  */
		else if (CheckHitKey(player->keyCode[static_cast<int>(DIR::RIGHT)]) == 1 &&
				 player->dir != DIR::LEFT)
		{
			player->dir = DIR::RIGHT;
		}


		/*  上向き  */
		else if (CheckHitKey(player->keyCode[static_cast<int>(DIR::UP)]) == 1 &&
				 player->dir != DIR::DOWN)
		{
			player->dir = DIR::UP;
		}

		/*  下向き  */
		else if (CheckHitKey(player->keyCode[static_cast<int>(DIR::DOWN)]) == 1 &&
				 player->dir != DIR::UP)
		{
			player->dir = DIR::DOWN;
		}
	}


	/* << プレイヤー移動変更 >> */

	/*  左移動  */
	if (player->dir == DIR::LEFT)
	{
		player->pos.x -= PLAYER_MOVE_X;
	}

	/*  右移動  */
	if (player->dir == DIR::RIGHT)
	{
		player->pos.x += PLAYER_MOVE_X;
	}

	/*  上移動  */
	if (player->dir == DIR::UP)
	{
		player->pos.y -= PLAYER_MOVE_Y;
	}

	/*  下移動  */
	if (player->dir == DIR::DOWN)
	{
		player->pos.y += PLAYER_MOVE_Y;
	}


	//描画状態
	player->type = static_cast<PLAYER_TYPE>(static_cast<int>(player->type));


	SetSeed(GetCenterPos(player->pos)); //ブロック生成
}


/* <<< プレイヤー描画処理 >>> */
void PlayerDraw()
{
	/* << プレイヤー１描画 >> */

	int imageNo; //プレイヤーの画像番号
	int type;

	imageNo = static_cast<int>(player1.dir) * PLAYER_ANI_MAX; //プレイヤー向き番号取得
	imageNo += ((playerCounter / 10) % PLAYER_ANI_MAX);		//移動アニメーションループ処理

	type = static_cast<int>(player1.type); //プレイヤー１見た目変化

	DrawGraph(player1.pos.x, player1.pos.y, playersImages[type][imageNo], true); //プレイヤー画像描画


	/* << プレイヤー２描画 >> */

	imageNo = static_cast<int>(player2.dir) * PLAYER_ANI_MAX; //プレイヤー向き番号取得
	imageNo += ((playerCounter / 10) % PLAYER_ANI_MAX);		//移動アニメーションループ処理

	type = static_cast<int>(player2.type); //プレイヤー２見た目変化

	DrawGraph(player2.pos.x, player2.pos.y, playersImages[type][imageNo], true); //プレイヤー画像描画
}


/* <<< プレイヤー解放処理 >>> */
void PlayerRelease()
{
	for (int p = 0; p < (static_cast<int>(PLAYER_TYPE::MAX)); p++)
	{
		for (int i = 0; i < (PLAYER_ANI_MAX * static_cast<int>(DIR::MAX)); i++)
		{
			DeleteGraph(playersImages[p][i]); //全プレイヤー画像 解放処理
		}
	}
}


/* <<< プレイヤー 中央座標取得 >>> */
POS GetCenterPos(POS playerPos)
{
	POS centerPos;
	centerPos.x = playerPos.x + PLAYER_SIZE_X / 2;
	centerPos.y = playerPos.y + PLAYER_SIZE_Y / 2;

	return centerPos;
}


/* <<<プレイヤー１画像選択処理 >>> */
void SetPlayer1Type(int value)
{
	int typeNum;
	typeNum = value;

	//プレイヤー１の画像遷移
	player1.type = static_cast<PLAYER_TYPE>(typeNum);
}


/* <<<プレイヤー２画像選択処理 >>> */
void SetPlayer2Type(int value)
{
	int typeNum;
	typeNum = value;

	//プレイヤー２の画像遷移
	player2.type = static_cast<PLAYER_TYPE>(typeNum);
}


/* ↓ <<< オリジナリティ >>> ↓ */

void JoyPadUpdate(PLAYER* player)
{
	/* << 向き回転 >> */

	if ((player->pos.y % MAPCHIP_SIZE_Y) == 0 && 
		(player->pos.x % MAPCHIP_SIZE_X) == 0)
	{
		if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_LEFT) != 0 &&
			player->dir != DIR::RIGHT)
		{
			player->dir = DIR::LEFT;
		}
		else if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_RIGHT) != 0 &&
			player->dir != DIR::LEFT)
		{
			player->dir = DIR::RIGHT;
		}


		else if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_UP) != 0 &&
			player->dir != DIR::DOWN)
		{
			player->dir = DIR::UP;
		}
		else if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_DOWN) != 0 &&
			player->dir != DIR::UP)
		{
			player->dir = DIR::DOWN;
		}
	}


	/* << プレイヤー移動変更 >> */

	/*  左移動  */
	if (player->dir == DIR::LEFT)
	{
		player->pos.x -= PLAYER_MOVE_X;
	}

	/*  右移動  */
	if (player->dir == DIR::RIGHT)
	{
		player->pos.x += PLAYER_MOVE_X;
	}

	/*  上移動  */
	if (player->dir == DIR::UP)
	{
		player->pos.y -= PLAYER_MOVE_Y;
	}

	/*  下移動  */
	if (player->dir == DIR::DOWN)
	{
		player->pos.y += PLAYER_MOVE_Y;
	}


	/* << 描画状態遷移 >> */

	int type;
	type = static_cast<int>(player->type);

	if (CheckHitKey(KEY_INPUT_M) == 1)
	{
		type++;

		if (type >= static_cast<int>(PLAYER_TYPE::MAX))
		{
			type = 0;
		}
	}
	if (CheckHitKey(KEY_INPUT_N) == 1)
	{
		type--;

		if (type < 0)
		{
			type = static_cast<int>(PLAYER_TYPE::MAX) - 1;
		}
	}
	player->type = static_cast<PLAYER_TYPE>(type);


	SetSeed(GetCenterPos(player->pos)); //ブロック生成
}
