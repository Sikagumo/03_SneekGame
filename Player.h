/* ---------------
　プレイヤーデータ
 ----------------- */

 /* --- <<< 定数宣言 >>> --- */

const int PLAYER_VALUE = 2; //プレイヤー数

const int PLAYER_ANI_MAX = 3; // プレイヤーアニメーション数

const int PLAYER_SIZE_X = 32; //プレイヤー画像 横サイズ
const int PLAYER_SIZE_Y = 32; //プレイヤー画像 縦サイズ

const int PLAYER_MOVE_X = 1;
const int PLAYER_MOVE_Y = 1;


 /* --- <<< 列挙型宣言 >>> --- */

 /*  プレイヤー画像状態  */
enum class PLAYER_TYPE
{
	MALE,	//メールくん
	FEMALE, //フィーメイルちゃん
	GREEN,	//緑お化け
	RED,	//赤お化け
	GHOST,	//お化け
	MAX		//最大値
};

/*  向き  */
enum class DIR
{
	DOWN,  //下向き
	LEFT,  //左向き
	RIGHT, //右向き
	UP,	   //上向き
	MAX	   //最大値
};


 /* --- <<< 構造体宣言 >>> --- */

struct PLAYER
{
	POS pos;		  //各プレイヤー 座標状態
	DIR dir;		  //各プレイヤー 方向状態
	PLAYER_TYPE type; //各プレイヤー 画像状態

	//各プレイヤー 入力キー
	int keyCode[static_cast<int>(DIR::MAX)];
};

struct JOYINPUT
{
	int left;
	int right;
	int up;
	int down;
};

/* --- <<< 関数プロトタイプ宣言 >>> --- */

bool PlayerInit();					//プレイヤー 初回初期化処理
void PlayerReStart();				//プレイヤー 再開時初期化処理
bool PlayerUpdate();				//プレイヤー 更新処理
void PlayerControll(PLAYER* player); //各プレイヤー 操作処理
void PlayerDraw();					//プレイヤー 描画処理
void PlayerRelease();				//プレイヤー 解放処理
POS GetCenterPos(POS iplayerPos);	//プレイヤー 中央座標取得
RESULT GetPlayerResult();	//勝敗結果を取得

void SetPlayer1Type(int value); //プレイヤー１画像選択処理
void SetPlayer2Type(int value); //プレイヤー２画像選択処理

void JoyPadUpdate(PLAYER* player);