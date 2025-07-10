/* ------------------
　ゲームシーンデータ
 -------------------- */

/* --- <<< 列挙型宣言 >>> --- */

enum class GAMESCENE_TYPE
{
	PLAYER1SELECT,
	PLAYER2SELECT,
	GAME,
};

 /* --- <<< 定数宣言 >>> --- */

const int GAME_SIZE_X = SCREEN_SIZE_X; //ゲームエリア 横サイズ[最大値：画面横サイズ]
const int GAME_SIZE_Y = SCREEN_SIZE_Y; //ゲームエリア 縦サイズ[最大値：画面縦サイズ]

const int BLOCK_COUNT_X = 25; //ブロックの縦数
const int BLOCK_COUNT_Y = 16; //ブロックの横数

/* --- <<< 関数プロトタイプ宣言 >>> --- */

bool StartSceneGameInit(); //初回初期化処理
void SceneGameInitGame();  //初期化処理
bool SceneGameUpdate();	   //メイン処理
void SceneGameDraw();	   //描画処理
void SceneGameRelease();   //解放処理

void SelectPlayerInputKeyInit(int num, int inputKey);
void SelectPlayerInputKeyUpdate(int num, int inputKey);