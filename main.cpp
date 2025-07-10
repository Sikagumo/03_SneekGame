/* -------------------------------
　ゲーム制作：課題３(スネークゲーム)
　　 氏名 　：2416026　中川原 諒
 --------------------------------- */

#include <DxLib.h>
#include "main.h"		//システムデータ
#include "SceneTitle.h"    //タイトルシーンデータ
#include "SceneGame.h"	   //ゲームシーンデータ
#include "SceneResult.h" //リザルトデータ


/* --- <<< 変数定義 >>> --- */

SCENE_ID sceneID; //ゲームの状態
bool gameEndFlag; //ゲーム終了フラグ[true:GameEnd,false:GameActive]

int preInputKey; //前フレームの入力キーを取得
int curInputKey; //現在の入力を取得

/*  フレームレート制御系  */

int curTime		  = 0; //現在の時間
int lateFrameTime = 0; //前回のフレーム実行時間

int frameCount			= 0;	//フレームカウント
int updateFrameRateTime = 0;	//フレームレート 更新時間
float frameRate			= 0.0f; //表示用フレームレート

/*  フォント系  */

int fontHandle;
int GetFontHandle()
{
	return fontHandle;
}

/* --- <<< 関数プロトタイプ宣言 >>> --- */

bool StartInit(); //初期化処理[初回]
void Update();	  //更新処理
void Draw();	  //描画処理	
void Release();	  //解放処理

/*--- <<< WinMain関数 >>> ---*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	//初期化処理
	if (!StartInit())
	{
		return -1; //失敗時に強制終了
	}

	/*--- <<< ゲームループ >>> ---*/

	while (ProcessMessage() == 0)
	{
		/* <<< フレームレート >>> */

		Sleep(1); //システムに処理を返す

		//現在時刻を取得する
		curTime = GetNowCount();

		if (curTime - lateFrameTime >= FRAME_RATE)
		{
			FrameRateUpdate();

			/*　入力キー初期化　*/
			preInputKey = curInputKey;
			curInputKey = CheckHitKey(KEY_INPUT_RETURN);

			Update(); //ゲームメイン処理

			Draw(); //描画処理
			
			ScreenFlip();//裏画面を表画面にコピー
		}


		/*　ゲーム終了処理　*/
		if (gameEndFlag == true) 
		{
			break;
		}
	}

	Release(); //解放処理

	/*--- <<< システム終了処理 >>> ---*/
	DxLib_End();
	return 0;
}


/* --- <<< 初期化処理[初回] >>> --- */
bool StartInit(void)
{
	/*--- <<< システム処理 >>> ---*/
	SetWindowText("課題3：スネークゲーム");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	SetWaitVSyncFlag(0); //垂直同期を切る

	if (DxLib_Init() == -1)
	{
		OutputDebugString("DxLibが初期化されていません\n");
		return false;
	}

	//タイトルシーン 初期化
	if (!SceneTitleInit())
	{
		OutputDebugString("タイトルデータ が初期化されていません\n\n");
		return false;
	}

	//ゲームシーン 初期化
	if (!StartSceneGameInit())
	{
		OutputDebugString("ゲームシーン が初期化されていません\n\n");
		return false;
	}

	//リザルトシーン 初期化
	if (!SceneResultStartInit())
	{
		OutputDebugString("リザルトが初期化されていません\n");
		//return false;
	}

	sceneID = SCENE_ID::TITLE;  //ゲーム状態 初期化
	gameEndFlag = false;			//ゲーム終了フラグ 無効化

	/*　入力キー初期化　*/
	preInputKey = CheckHitKey(KEY_INPUT_RETURN);
	curInputKey = preInputKey;

	/*　フォントファイル取得処理　*/
	if (!GetFontFile())
	{
		OutputDebugString("フォントファイルが読み込まれていません\n\n");
		return false;
	}

	return true; //正常に終了
}


/* --- <<< ゲームメイン処理 >>> --- */
void Update() 
{
	/*  タイトル処理  */
	switch (sceneID) 
	{

		/*  タイトル処理  */
		case SCENE_ID::TITLE:

		if (SceneTitleUpdate() == false)
		{
			sceneID = SCENE_ID::GAME;
			StartSceneGameInit(); //タイトル初期化処理
		}
		break;


		/*  ゲーム処理  */
		case SCENE_ID::GAME:

		if (SceneGameUpdate() == false)
		{
			sceneID = SCENE_ID::RESULT;
		}
		break;


		/*  リザルト処理  */
		case SCENE_ID::RESULT:

		if (SceneResultUpdate() == false)
		{
			sceneID = SCENE_ID::TITLE;
		}
		break;
			
	}


	//ゲーム終了処理
	if (sceneID != SCENE_ID::TITLE &&
		CheckHitKey(KEY_INPUT_ESCAPE) == 1)
	{
		GameEnd();
	}
}


/* --- <<< 描画処理 >>> --- */
void Draw()
{
	SetDrawScreen(DX_SCREEN_BACK); //描画画面の内容を裏画面に設定
	ClearDrawScreen();			   //描画画面の内容削除


	DrawFormatString(SCREEN_SIZE_X - 90, 0, GetColor(255, 255, 255), "FPS[%.1f]", frameRate);

	
	switch (sceneID)
	{
		/*  タイトルシーン  */
	case SCENE_ID::TITLE:

		SceneTitleDraw();
		break;


	case SCENE_ID::GAME:

		SceneGameDraw();
		break;


		/*  リザルトシーン  */
	case SCENE_ID::RESULT:

		SceneResultDraw();
		break;
	}
}


/* --- <<< 解放処理 >>> --- */
void Release()
{
	SceneResultRelease();	//リザルト 解放
	SceneGameRelease();		//ゲーム 解放
	SceneTitleRelease();	//タイトル 解放

	//初期化処理とは逆に解放処理を行う(初期化[親→子→孫で生成]、解放処理[孫→子→親で解放を行うため])
}

/* <<< 勝敗の設定 >>> */
void SetResult(RESULT value)
{
	SetGameResult(value); //リザルトシーンに渡す
}

/* ↓ <<< オリジナリティ >>> ↓ */

/* <<< ゲーム終了処理 >>> */
void GameEnd()
{
	gameEndFlag = true;
}

/* <<< フレームレート更新処理 >>> */
void FrameRateUpdate()
{
	lateFrameTime = curTime;

	frameCount++; //フレーム計算増加処理

	int nDifTime = curTime - updateFrameRateTime;

	if (nDifTime > 1000)
	{
		float fFrameConut = (float)(frameCount * 1000);

		frameRate = fFrameConut / nDifTime;

		frameCount = 0;

		//フレームレート更新時間 更新
		updateFrameRateTime = curTime;
	}
}


/* <<< フォントファイルの取得 >>> */
bool GetFontFile()
{
	int fontFileSize   = FileRead_size("font/叛逆明朝.ttf"); //ファイル容量
	int fontFileHandle = FileRead_open("font/叛逆明朝.ttf"); //ファイルハンドル

	//フォントデータ格納用のメモリ領域確保
	void* Buffer = new void* [fontFileSize];

	//フォントファイルをまとめて読み込み
	FileRead_read(Buffer, fontFileSize, fontFileHandle);

	//AddFontMemeResourceEx引数用変数
	DWORD font_num = 0;
	if (AddFontMemResourceEx(Buffer, fontFileSize, NULL, &font_num) == 0)
	{
		OutputDebugString("フォントファイルが読み込めませんでした。\n");
		return false;
	}


	FileRead_close(fontFileHandle); //フォント読み込み終了
	delete[] Buffer;				//バッファから解放


	//フォントを生成
	fontHandle = CreateFontToHandle("叛逆明朝", FONT_SIZE, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
	return true;


	/* ↓↓ フォントを使用してのテキスト生成 ↓↓ */

	//DrawFormatStringToHandle(0, 16, 0xFFFFFF, GetFontHandle(), "任意の値 : %d", GetFontHandle());
}