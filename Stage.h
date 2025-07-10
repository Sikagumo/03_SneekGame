/* -------------
　ステージデータ
 --------------- */

/* --- <<< 定数定義 >>> --- */

const int MAPCHIP_SIZE_X = 32; //マップチップ 横サイズ
const int MAPCHIP_SIZE_Y = 32; //マップチップ 縦サイズ


/* --- <<< 関数プロトタイプ宣言 >>> --- */

bool StageInit();	 //ステージ 初期化
void StageReStart(); //ステージ 再初期化
void StageUpdate();  //ステージ 更新処理
void StageDraw();	 //ステージ 描画処理
void StageRelease(); //ステージ画像 解放処理

//座標系から配列の添え時に変換
POS PosToIndex(POS playerPos);

//指定された場所に種を蒔く
void SetSeed(POS centerPos);

bool IsCollision(POS centerPos);