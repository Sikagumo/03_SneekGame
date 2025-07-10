/* --------------------
	リザルト処理
 ---------------------- */

#include <DxLib.h>
#include "main.h" //システムデータ
#include "SceneResult.h"//リザルトデータ

/* --- <<< 変数定義 >>> --- */

float curPerformWaitTime;	//現在演出 待機時間
float returnTextPerformTime; //"戻る"テキスト 演出時間

RESULT gameResult;

RETURN_TEXT_PARAM returnTextParam; //"戻る"状態演出
RESULT_PERFORM_PARAM performParam;

int resultPreInputKey;
int resultCurInputKey;


/* <<< リザルト初期化[初回] >>> */
bool SceneResultStartInit(void)
{
	SceneResultInit(); //初期化処理

	return true;
}


/* <<< リザルト初期化[共有] >>> */
void SceneResultInit(void)
{
	//演出初期化処理
	performParam = RESULT_PERFORM_PARAM::NONE;

	curPerformWaitTime	  = RESULT_IMAGE_WAIT_TIME; //リザルト演出時間 初期化
	returnTextPerformTime = RETURN_TEXT_VISIBLE_TIME; //"戻る"テキスト演出時間 初期化

	returnTextParam = RETURN_TEXT_PARAM::INACTIVE; //"戻る"テキスト描画処理 無効化

	resultPreInputKey = CheckHitKey(KEY_INPUT_RETURN);
	resultCurInputKey = resultPreInputKey;
}


/* <<< リザルト処理 >>> */
bool SceneResultUpdate(void)
{
	resultPreInputKey = resultCurInputKey;
	resultCurInputKey = CheckHitKey(KEY_INPUT_RETURN);

	/*  演出スキップ処理 (連続入力制限あり)  */

	if (performParam == RESULT_PERFORM_PARAM::PROC_ACTIVE)
	{
		if (resultPreInputKey == 0 && resultCurInputKey == 1)
		{
			SceneResultInit();
			return false; //シーン遷移
		}
	}

	/*  リザルト処理  */
	else if (resultPreInputKey == 0 && resultCurInputKey == 1)
	{
		returnTextParam = RETURN_TEXT_PARAM::VISIBLE; //"戻る"テキスト 有効化

		performParam = RESULT_PERFORM_PARAM::PROC_ACTIVE; //"処理有効化" 状態遷移
	}
	
	


	/*  戻るボタン演出  */
	if (returnTextParam != RETURN_TEXT_PARAM::INACTIVE)
	{
		if (returnTextPerformTime > 0)
		{
			returnTextPerformTime -= 0.1f; //演出時間 減少処理
		}

		//演出状態 遷移処理
		else
		{
			if (returnTextParam == RETURN_TEXT_PARAM::INVISIBLE)
			{
				returnTextPerformTime = RETURN_TEXT_VISIBLE_TIME;	//可視化時間 取得
				returnTextParam		  = RETURN_TEXT_PARAM::VISIBLE; //"戻る"テキスト 可視化
			}
			else if (returnTextParam == RETURN_TEXT_PARAM::VISIBLE)
			{
				returnTextPerformTime = RETURN_TEXT_INVISIBLE_TIME;	  //不可視化時間 取得
				returnTextParam		  = RETURN_TEXT_PARAM::INVISIBLE; //"戻る"テキスト 不可視化
			}
		}
	}

	ResultPerformance(); //各演出遷移処理
	return true;
}


/* <<< 演出遷移処理 >>> */
void ResultPerformance(void)
{
	/*  初期演出 状態  */
	if (performParam == RESULT_PERFORM_PARAM::NONE)
	{
		//初期状態 → タイトル画像演出 遷移処理
		PerformWaitTime(RESULT_PERFORM_PARAM::RETURN_TEXT, RESULT_IMAGE_WAIT_TIME);
	}

	/*  "戻る"テキスト演出 状態  */
	else if (performParam == RESULT_PERFORM_PARAM::RETURN_TEXT)
	{
		if (returnTextParam == RETURN_TEXT_PARAM::INACTIVE) 
		{
			returnTextParam = RETURN_TEXT_PARAM::VISIBLE;
		}

		//"戻る"テキスト演出 → 処理有効化 遷移処理
		PerformWaitTime(RESULT_PERFORM_PARAM::PROC_ACTIVE, PROCESS_ACTIVE_WAIT_TIME);
	}
}


/* <<< 演出待機処理 >>> */
void PerformWaitTime(RESULT_PERFORM_PARAM nextperform, float nexttime)
{
	//演出待機処理
	if (curPerformWaitTime > 0)
	{
		curPerformWaitTime -= 0.1f;
	}

	//演出状態 遷移処理
	else
	{
		performParam = nextperform; //演出状態遷移
		curPerformWaitTime = nexttime;	 //次の演出待機時間を取得
	}
}



/* <<< リザルト描画処理 >>> */
void SceneResultDraw(void)
{
	switch (gameResult)
	{
	case RESULT::PLAYER1WIN:
		//テキスト描画
		DrawFormatStringFToHandle((SCREEN_SIZE_X / 2) - (GetDrawFormatStringWidthToHandle(GetFontHandle(), "プレイヤ～ 1 の勝利！！") / 2),
			(SCREEN_SIZE_Y / 2),
			0xFFFFFF, GetFontHandle(),
			"プレイヤ～ 1 の勝利！！");
		break;


	case RESULT::PLAYER2WIN:
		//テキスト描画
		DrawFormatStringFToHandle((SCREEN_SIZE_X / 2) - (GetDrawFormatStringWidthToHandle(GetFontHandle(), "プレイヤ～ 2 の勝利！！") / 2),
			(SCREEN_SIZE_Y / 2),
			0xFFFFFF, GetFontHandle(),
			"プレイヤ～ 2 の勝利！！");
		break;


	case RESULT::DRAW:
		//テキスト描画
		DrawFormatStringFToHandle((SCREEN_SIZE_X / 2) - (GetDrawFormatStringWidthToHandle(GetFontHandle(), "両者引き分け！！") / 2),
			(SCREEN_SIZE_Y / 2),
			0xFFFFFF, GetFontHandle(),
			"両者引き分け！！");
		break;
	}


	//"戻る"テキスト 描画
	if (returnTextParam == RETURN_TEXT_PARAM::VISIBLE)
	{
		//テキスト描画
		DrawFormatString((SCREEN_SIZE_X / 2 - RETURN_TEXT_SIZE_X / 2),
			(SCREEN_SIZE_Y / 2 + RETURN_TEXT_SIZE_Y),
			0xFFFFFF, "Enterキーでタイトルに戻る");
	}

}


/* <<< リザルト解放処理 >>> */
void SceneResultRelease(void)
{
	
}


/* <<< 勝敗結果を取得 >>> */
void SetGameResult(RESULT value) 
{
	gameResult = value;
}