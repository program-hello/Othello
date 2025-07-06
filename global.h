

int GHandle[15];		// 画像ハンドル
int FHandle[7];		// フォントハンドル
int clickFlag = 0;	// クリックフラグ　クリックしている間、連続して動作するのを防ぐ

int baseX = 58, baseY = 58;		// 一番左上の座標
int mode = 0;	// 0 : player 対 com , 1 : player 対 player
int turn = 0;	// 0 : PLAYER , 1 : COM
int comColor = 0;	// 0 : 黒 , 1 : 白
int playerColor = 0;	// 0 : 黒 , 1 : 白
int cntB = 0, cntW = 0;		// 黒と白の数
int cntPass = 0;	// パスのカウント　連続で２回パスが押されたらそれはどちらも石を打てないので試合終了
int steps = 1;		// 現在の手数
int endThink = 0;	// ４6手以降かを判定用
int playStanderdMovesFlag = 1;	// 定石を抜けたかのフラグ	１：定石中　２：定石を抜けている
int edgeFlag[4] = { 0 };		// 埋まっている角を表すフラグ


unsigned long long bB = 0b0000000000000000000000000000100000010000000000000000000000000000;	// bitboard Black
unsigned long long bW = 0b0000000000000000000000000001000000001000000000000000000000000000;	// bitboard White
unsigned long long bN = 0b0000000000000000000000000000000000000000000000000000000000000000;	// bitboard Now  現在の盤面
unsigned long long bSelectable = 0b0;	// 合法手を追加していく
unsigned long long putPlace = 0b0;		// 石を置いた場所
unsigned long long rev = 0;		   // 反転する位置を格納するビットボード
unsigned long long bC1 = 0;        // Bitboard Color1 (current player's stones)
unsigned long long bC2 = 0;        // Bitboard Color2 (opponent's stones)

unsigned long long horizontalMask = 0x7e7e7e7e7e7e7e7e;		// 横用マスク
unsigned long long verticalMask = 0xffffffffffff00;		// 横用マスク
unsigned long long allsideMask = 0x7e7e7e7e7e7e00;		// ななめ用マスク

int depthNow = 10;	// 探索の深さ
int depthNum = 0;	// 現在の探索数
int depthNowOpenness = 6;	// 開放度理論の深さ設定
int randomFreq = 0;	// ランダムに打つ確率
int bestX = 0, bestY = 0;	// 最善手の座標

int ngs = 0;	// ４４手目以降負けている場合、最終局面での枚数が最大になるような合法手のスコアを格納  neber giveup score


// 評価関数で使うマスごとの得点
int point[64] = { 30,-10, 2, 1, 1, 2,-10,30,
				 -10,-100,-3,-3,-3,-3,-100,-10,
				   2, -3, 2, 0, 0, 2, -3, 2,
				   1, -3, 0, 0, 0, 0, -3, 1,
				   1, -3, 0, 0, 0, 0, -3, 1,
				   2, -3, 2, 0, 0, 2, -3, 2,
				 -10,-100,-3,-3,-3,-3,-100,-10,
				  30,-10, 2, 1, 1, 2,-10,30 };

int point2[64] = { 100,-40, 20, 5, 5, 20,-40,100,
				 -40,-80,-1,-1,-1,-1,-80,-40,
				   20, -1, 5, 1, 1, 5, -1, 20,
				   5, -1, 1, 0, 0, 1, -1, 5,
				   5, -1, 1, 0, 0, 1, -1, 5,
				   20, -1, 5, 1, 1, 5, -1, 20,
				 -40,-80,-1,-1,-1,-1,-80,-40,
				  100,-40, 20, 5, 5, 20,-40,100 };


int cntValid = 0;	// 有効な定石の数をカウント
char valid[MAXSTANDERDMOVES][128] = { "" };
char standerdMoves[MAXSTANDERDMOVES][128];