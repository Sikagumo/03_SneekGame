/* ----------------
　ゲームシーン処理
 ------------------ */

#include<DxLib.h>
#include "main.h"
#include "Stage.h"
#include "Player.h"
#include "SceneGame.h"


/* --- <<< 変数定義 >>> --- */

int gameScreen; // ゲーム領域
GAMESCENE_TYPE gameSceneType;

int selectNum;	//キャラ選択変数
int preSelectKey[6]; //前フレームの指定した入力キーを取得
int curSelectKey[6]; //現在の入力を取得


/* --- <<< 処理系 >>> --- */

/* <<< ゲームシーン初期化[初回] >>> */
bool StartSceneGameInit()
{
	//ゲーム領域を作成
	gameScreen = MakeScreen(GAME_SIZE_X, GAME_SIZE_Y, false);

	/*  ステージ初期化処理  */
	if (StageInit() == false)
	{
		return false;
	}

	/*  プレイヤー初期化処理  */
	if (PlayerInit() == false)
	{
		return false;
	}

	SceneGameInitGame();


	return true;
}

/* <<< ゲームシーン初期化[初回以降] >>> */
void SceneGameInitGame()
{
	gameSceneType = GAMESCENE_TYPE::PLAYER1SELECT;

	selectNum = 0; //キャラ選択変数 初期化

	/*　選択キー初期化　*/
	SelectPlayerInputKeyInit(0, KEY_INPUT_LEFT);
	SelectPlayerInputKeyInit(1, KEY_INPUT_RIGHT);
	SelectPlayerInputKeyInit(2, KEY_INPUT_A);
	SelectPlayerInputKeyInit(3, KEY_INPUT_D);
	SelectPlayerInputKeyInit(4, KEY_INPUT_RETURN);
	SelectPlayerInputKeyInit(5, KEY_INPUT_SPACE);

}
/* <<< 各入力キー判定 初期化 >>> */
void SelectPlayerInputKeyInit(int num, int inputKey)
{
	//指定キーの入力を行っているかの確認
	preSelectKey[num] = CheckHitKey(inputKey);
	curSelectKey[num] = preSelectKey[num];
}


/* <<< ゲームシーンメイン処理 >>> */
bool SceneGameUpdate()
{
	/*　選択キー入力確認　*/
	SelectPlayerInputKeyUpdate(0, KEY_INPUT_LEFT);
	SelectPlayerInputKeyUpdate(1, KEY_INPUT_RIGHT);
	SelectPlayerInputKeyUpdate(2, KEY_INPUT_A);
	SelectPlayerInputKeyUpdate(3, KEY_INPUT_D);
	SelectPlayerInputKeyUpdate(4, KEY_INPUT_RETURN);
	SelectPlayerInputKeyUpdate(5, KEY_INPUT_SPACE);

	switch (gameSceneType)
	{
	case GAMESCENE_TYPE::PLAYER1SELECT:

		if (preSelectKey[0] == 0 && curSelectKey[0] == 1 ||
			preSelectKey[2] == 0 && curSelectKey[2] == 1)
		{
			selectNum++; //キャラ選択変数 増加

			/*　増加制限　*/
			if (selectNum > static_cast<int>(PLAYER_TYPE::MAX) - 1)
			{
				selectNum = 0;
			}

			SetPlayer1Type(selectNum); //プレイヤー１画像 反映
		}
		else if (preSelectKey[1] == 0 && curSelectKey[1] == 1 ||
				 preSelectKey[3] == 0 && curSelectKey[3] == 1)
		{
			selectNum--; //キャラ選択変数 減少

			/*　減少制限　*/
			if (selectNum < 0)
			{
				selectNum = static_cast<int>(PLAYER_TYPE::MAX) - 1;
			}

			SetPlayer1Type(selectNum); //プレイヤー１画像 反映
		}

		if (preSelectKey[4] == 0 && curSelectKey[4] == 1 ||
			preSelectKey[5] == 0 && curSelectKey[5] == 1)
		{
			SetPlayer1Type(selectNum); //プレイヤー１画像 反映

			selectNum = 0; //キャラ選択変数 初期化

			gameSceneType = GAMESCENE_TYPE::PLAYER2SELECT; //プレイヤー２選択状態に遷移
		}
		break;


	case GAMESCENE_TYPE::PLAYER2SELECT:

		if (preSelectKey[0] == 0 && curSelectKey[0] == 1 ||
			preSelectKey[2] == 0 && curSelectKey[2] == 1)
		{
			selectNum++;  //キャラ選択変数 増加

			/*　増加制限　*/
			if (selectNum > static_cast<int>(PLAYER_TYPE::MAX) - 1)
			{
				selectNum = 0;
			}

			SetPlayer2Type(selectNum); //プレイヤー２画像 反映
		}

		else if (preSelectKey[1] == 0 && curSelectKey[1] == 1 ||
				 preSelectKey[3] == 0 && curSelectKey[3] == 1)
		{
			selectNum--; //キャラ選択変数 減少

			/*　減少制限　*/
			if (selectNum < 0)
			{
				selectNum = static_cast<int>(PLAYER_TYPE::MAX) - 1;
			}

			SetPlayer2Type(selectNum); //プレイヤー２画像 反映
		}

		if (preSelectKey[4] == 0 && curSelectKey[4] == 1 ||
			preSelectKey[5] == 0 && curSelectKey[5] == 1)
		{
			SetPlayer2Type(selectNum); //プレイヤー２画像 反映

			selectNum = 0; //キャラ選択変数 初期化

			gameSceneType = GAMESCENE_TYPE::GAME; //ゲーム状態に遷移
		}
		break;

		
	case GAMESCENE_TYPE::GAME:

		StageUpdate();  //ステージ処理

		//プレイヤー処理
		if (PlayerUpdate() == false)
		{
			//勝敗結果をリザルトに渡す
			SetResult(GetPlayerResult());

			return false; //リザルト画面に遷移
		}
		break;
	}

	return true;
}

/* <<< 各入力キー判定 >>> */
void SelectPlayerInputKeyUpdate(int num, int inputKey)
{
	//指定キーの入力を行っているかの確認
	preSelectKey[num] = curSelectKey[num];	   //現在の入力を 前入力に反映する
	curSelectKey[num] = CheckHitKey(inputKey); //現在の入力を取得
}


/* <<< ゲームシーン 描画処理 >>> */
void SceneGameDraw()
{
	SetDrawScreen(gameScreen); //描画場所をゲーム領域にする
	ClearDrawScreen();		   //ゲーム領域をクリアする

	StageDraw(); //ステージ描画処理
	PlayerDraw(); //プレイヤー描画処理

	if (gameSceneType == GAMESCENE_TYPE::PLAYER1SELECT)
	{
		//テキスト描画
		DrawFormatStringFToHandle((SCREEN_SIZE_X / 2) - (GetDrawFormatStringWidthToHandle(GetFontHandle(), "プレイヤ～ 1 のキャラ選択中・・・") / 2),
			(SCREEN_SIZE_Y / 2),
			0xFFFF00, GetFontHandle(),
			"プレイヤ～ 1 のキャラを左右入力で選択");
	}
	else if (gameSceneType == GAMESCENE_TYPE::PLAYER2SELECT)
	{
		//テキスト描画
		DrawFormatStringFToHandle((SCREEN_SIZE_X / 2) - (GetDrawFormatStringWidthToHandle(GetFontHandle(), "プレイヤ～ 2 のキャラ選択中・・・") / 2),
			(SCREEN_SIZE_Y / 2),
			0xFFFF00, GetFontHandle(),
			"プレイヤ～ 2 のキャラを左右入力で選択");
	}


	SetDrawScreen(DX_SCREEN_BACK); //描画位置をスクリーンバックに設定
	DrawGraph(0, 64, gameScreen, true); //ゲームスクリーンを描画
}


/* <<< ゲームシーン 解放処理 >>> */
void SceneGameRelease()
{
	PlayerRelease(); //プレイヤー解放処理

	StageRelease(); // ステージ解放処理

	DeleteGraph(gameScreen); //ゲーム領域を解放
}