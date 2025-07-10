/* -----------------------
	タイトルシーンデータ
 ------------------------- */

/* --- <<< 定数定義 >>> --- */

const int TITLE_IMAGE_SIZE_X = 700; //タイトル画像 横サイズ[Default:423]
const int TITLE_IMAGE_SIZE_Y = 300; //タイトル画像 縦サイズ[Default:198]

const int START_TEXT_SIZE_X = 175; //開始テキスト 横サイズ[Default:175]
const int START_TEXT_SIZE_Y = 100; //開始テキスト 縦サイズ[Default:100]

const int TEXT_VISIBLE_TIME = 3; //開始テキスト 可視化時間[Default:5]


/* --- <<< プロトタイプ宣言 >>> --- */

bool SceneTitleInit(void);		//タイトル 初期化処理
bool SceneTitleUpdate(void);	//タイトル 処理
void SceneTitleDraw(void);		//タイトル 描画処理
void SceneTitleRelease(void);	//タイトル 画像解放処理

void TitleStartProc(void);	//タイトル 初期状態処理

//#endif