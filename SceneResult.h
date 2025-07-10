/* ----------------------
	リザルトデータ
 ------------------------ */


/* --- <<< 定数定義 >>> --- */

//リザルト演出状態
const enum RESULT_PERFORM_PARAM
{
	NONE, RETURN_TEXT, PROC_ACTIVE
};

//"戻る"テキスト演出状態
const enum RETURN_TEXT_PARAM
{
	INACTIVE, VISIBLE, INVISIBLE
};

const int RESULT_IMAGE_SIZE_X = 452; //リザルト画像 横サイズ
const int RESULT_IMAGE_SIZE_Y = 123; //リザルト画像 縦サイズ

const int RETURN_TEXT_SIZE_X = 200; //"戻る"テキスト 横サイズ
const int RETURN_TEXT_SIZE_Y = 98;  //"戻る"テキスト 縦サイズ

const float RETURN_TEXT_VISIBLE_TIME   = 5.0; //"戻る"テキスト 可視化時間[Default:5.0f]
const float RETURN_TEXT_INVISIBLE_TIME = 2.0; //"戻る"テキスト 不可視化時間[Default:2.0f]

const float RESULT_IMAGE_WAIT_TIME  = 5.0;  //リザルトタイトル 演出待機時間
const float RETURN_TEXT_WAIT_TIME	  = 0.0; //"戻る"テキスト 演出待機時間
const float PROCESS_ACTIVE_WAIT_TIME = 2.5; //遷移処理 有効化待機時間


/* --- <<< プロトタイプ宣言 >>> --- */

bool SceneResultStartInit(void);	//初期化処理[初回]
bool SceneResultUpdate(void);	//メイン処理
void SceneResultDraw(void);	//描画処理
void SceneResultRelease(void);	//解放処理

void SetGameResult(RESULT value); //勝敗結果を取得

void SceneResultInit(void); //初期化処理[共有]
void ResultPerformance(void); //演出処理

//演出待機処理
void PerformWaitTime(RESULT_PERFORM_PARAM nextperform, float nexttime);