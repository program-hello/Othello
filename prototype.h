

void Handle_load();					// 各種ハンドルの読み込み
void title();						// タイトル画面
void load_standerdMoves();			// 定石ファイルの読み込み
void initialize();					// 各種変数の初期化
void ending();						// 試合結果を表示
void draw_board();					// 盤面の描画
void printBits(long long n);		// 現状の盤面をコンソールに表示する
void put(int x, int y, int color);	// 石を置く  turn : 0 = 黒 , 1 = 白
void remove(int x, int y, int color);	// 石を取り除く　minimax法で使う
void search_selectable(int color);	// 合法手を探す
void select_player();				// 石を置くマスを選択する
void select_com();					// 思考ルーチンで石を置く
void countStone();					// 石の数を数える
void reverse(int color);			// 石をひっくり返す
unsigned long long transfer(unsigned long long  putPlace, int k);	// 石を置いた場所から８方向のいずれか１方向に１マス進める
void loadStanderdMoves();				// 定石の読み込み
int playStanderdMoves();				// 定石を実行

int maxLevel(int alpha, int beta, int depth);	// minimax法
int minLevel(int alpha, int beta, int depth);	// minimax法
int evaluation_function(int index);		// 評価関数 

void maxLevel_openness(int alpha, int beta, int depth, int* cntOpenness);		// 開放度理論用のミニマックス法基盤
void minLevel_openness(int alpha, int beta, int depth, int* cntOpenness);		// 開放度理論用のミニマックス法基盤
int openness(unsigned long long rev);	// 開放度理論
void openness_transfer(unsigned long long  transferRev, int k, int* cntOpenness);	// 開放度理論で開放度をカウントする
int openness_evaluation_function(int index);	// 開放度理論用の評価関数