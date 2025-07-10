/* ---------------
	タイトルの処理
 ----------------- */

#include <DxLib.h>
#include "main.h"   //システムデータ
#include "SceneTitle.h"  //タイトルシーンデータ　


/* --- <<< 変数定義 >>> --- */

int titleImage;		   //タイトル画像

float curStartTextViewTime; //現在テキスト 描画時間

int preInputKeys[4]; //前フレームの指定した入力キーを取得
int curInputKeys[4]; //現在の入力を取得


/* -- <<< プロトタイプ宣言 >>> --- */

void InputKeyCheckInit(int num, int inputKey);
void InputKeyCheckUpdate(int num, int inputKey);


/* --- <<< 関数系 >>> --- */

/* <<< タイトル初期化処理 >>> */
bool SceneTitleInit(void)
{
	//タイトル画像 グラフィック登録
	titleImage = LoadGraph("image/Title.png");
	if (titleImage == -1)
	{
		OutputDebugString("タイトル画像が読み込まれていません。");
		return false;
	}

	curStartTextViewTime = 0; //描画時間 初期化

	/*  キー入力確認 初期化  */
	InputKeyCheckInit(0, KEY_INPUT_RETURN); //Enterキー  入力確認初期化
	InputKeyCheckInit(1, KEY_INPUT_ESCAPE); //Escapeキー 入力確認初期化
	InputKeyCheckInit(2, KEY_INPUT_LEFT);	//左矢印キー 入力確認初期化
	InputKeyCheckInit(3, KEY_INPUT_RIGHT);	//右矢印キー 入力確認初期化


	return true; //正常終了
}


/* <<< 各入力キー判定 初期化 >>> */
void InputKeyCheckInit(int num, int inputKey)
{
	//指定キーの入力を行っているかの確認
	preInputKeys[num] = CheckHitKey(inputKey);
	curInputKeys[num] = preInputKeys[num];
}


/* <<< タイトルメイン処理 >>> */
bool SceneTitleUpdate(void)
{
	/*  キー入力確認  */
	InputKeyCheckUpdate(0, KEY_INPUT_RETURN); //Enterキー  入力確認
	InputKeyCheckUpdate(1, KEY_INPUT_ESCAPE); //Escapeキー 入力確認
	InputKeyCheckUpdate(2, KEY_INPUT_LEFT);	  //左矢印キー 入力確認
	InputKeyCheckUpdate(3, KEY_INPUT_RIGHT);  //右矢印キー 入力確認

	TitleStartProc();
	if (preInputKeys[0] == 0 && curInputKeys[0] == 1)
	{
		return false;
	}
	return true;
}

/* <<< 各入力キー判定 >>> */
void InputKeyCheckUpdate(int num, int inputKey)
{
	//指定キーの入力を行っているかの確認
	preInputKeys[num] = curInputKeys[num];	   //現在の入力を 前入力に反映する
	curInputKeys[num] = CheckHitKey(inputKey); //現在の入力を取得
}


/* <<< タイトル初期状態処理 >>> */
void TitleStartProc(void)
{
	//ゲーム終了処理
	if (preInputKeys[1] == 0 && curInputKeys[1] == 1)
	{
		GameEnd();
	}

	curStartTextViewTime += 0.1f; //描画時間 増加処理
}


/* <<< タイトル描画処理 >>>> */
void SceneTitleDraw(void)
{
	//タイトル画像 描画処理
	DrawGraph((SCREEN_SIZE_X / 2) - (TITLE_IMAGE_SIZE_X / 2),
			  (SCREEN_SIZE_Y / 2) - TITLE_IMAGE_SIZE_Y,
			  titleImage, true);

	/*  開始テキスト 可視化処理  */
	if ((static_cast<int>(curStartTextViewTime) % TEXT_VISIBLE_TIME) == 0)
	{
		//テキスト描画
		DrawFormatStringFToHandle((SCREEN_SIZE_X / 2) - (GetDrawFormatStringWidthToHandle(GetFontHandle(), "ENTERき～で げ～むすた～と") / 2),
			(SCREEN_SIZE_Y / 2) + (PIXEL_SIZE * 5),
			0xFFFFFF, GetFontHandle(),
			"ENTERキ～で ゲ～ムスタ～ト");
	}
}


/* <<< タイトル解放処理 >>> */
void SceneTitleRelease(void)
{
	DeleteGraph(titleImage);		   //タイトル画像 解放

	//メモリから画像を解放(削除)する
}