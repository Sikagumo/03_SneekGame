/* ----------------
　システム系データ
 ------------------ */

/* --- <<< 定数系 >>> --- */

//画像サイズ
const int SCREEN_SIZE_X = 800; //縦[Default：800]
const int SCREEN_SIZE_Y = 600; //横[Default：600]

const int PIXEL_SIZE = 30; //1ピクセルサイズ

//設定フレームレート(60分の１)
constexpr float FRAME_RATE(1000 / 60);

const int FONT_SIZE = 40; //フォントサイズ

/* --- <<< 構造体宣言>>> --- */

 /*  座標  */
struct POS
{
	int x;
	int y;
};

//ゲームの状態
const enum class SCENE_ID
{
	TITLE,
	GAME,
	RESULT
};


/*  勝敗  */
enum class RESULT
{
	NONE,		//ゲーム継続時
	PLAYER1WIN, //プレイヤー１勝利
	PLAYER2WIN, //プレイヤー２勝利
	DRAW		//引き分け
};


/* --- <<< 関数プロトタイプ宣言 >>> --- */

void SetResult(RESULT value);

void GameEnd();			//ゲーム終了処理
void FrameRateUpdate(); //フレームレート 計算処理

bool GetFontFile();	 //フォントファイルの読み込み
int GetFontHandle(); //フォントハンドルの取得