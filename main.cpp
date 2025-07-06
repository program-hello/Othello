#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include "define.h"
#include "prototype.h"
#include "global.h"



//*******************************************************
// デバッグ用のコンソールの初期処理(メインループ前に呼ぶ)
//*******************************************************
void initDebugConsole() {
	// デバッグ用コンソール画面表示フラグが表示しない設定の場合、処理終了
	//if (debugInfo.debugConsoleDisplayFlg == FALSE) {
	//	return;
	//}
	//デバッグ用にコンソールを呼び出す
	AllocConsole();
	(void)freopen("CONOUT$", "w", stdout);
	(void)freopen("CONIN$", "r", stdin);
	// デバッグコンソールがアクティブウィンドウになるので
	// ゲーム本体のウィンドウをアクティブにする
	SetForegroundWindow(GetMainWindowHandle());
}
//*******************************************************
// デバッグ用のコンソールのクリア処理
//*******************************************************
void clearDebugConsole() {
	//// デバッグ用コンソール画面表示フラグが表示しない設定の場合、処理終了
	//if (debugInfo.debugConsoleDisplayFlg == FALSE) {
	//	return;
	//}
	system("cls");
}
//*******************************************************
// デバッグ用のコンソールの終了処理(メインループ後に呼ぶ)
//*******************************************************
void endDebugConsole() {
	//// デバッグ用コンソール画面表示フラグが表示しない設定の場合、処理終了
	//if (debugInfo.debugConsoleDisplayFlg == FALSE) {
	//	return;
	//}
	//コンソール解放
	FreeConsole();
}






int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//*******************************************
	// 初期設定
	//*******************************************
	{
		//initDebugConsole();							// コンソールの表示
		ChangeWindowMode(TRUE);						// TRUE : ウィンドウ  FALSE ; フルスクリーン
		SetGraphMode(500, 500, 32);					// ウィンドウサイズ
		SetWindowSizeChangeEnableFlag(TRUE);		// ウィンドウサイズの変更を許可するか否か
		SetMainWindowText("Othello");				// ウィンドウ名
		SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_DESKTOP);		// フルスクリーンモード時の解像度モードを設定する

		SetBackgroundColor(241, 237, 238);			// 画面の背景色を設定
		if (DxLib_Init() == -1) return -1;			// エラーが起きたら直ちに終了
		SetDrawScreen(DX_SCREEN_BACK);				// 裏画面に描画する
		SetMouseDispFlag(TRUE);						// マウスを画面上に表示する
	}






	Handle_load();	// 各種ハンドルの読み込み
	load_standerdMoves();


TITLE:
	initialize();	// 各種変数の初期化
	title();


	// ゲームメインループ
	while (ProcessMessage() != -1) {

		// クリックした状態でほかの要素が反応することを防ぐ
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0) clickFlag = 0;


		search_selectable(playerColor);// 合法手を探す
		countStone();			// 石を数える


		// レベル４の時だけ探索の深さを随時変更する
		if (randomFreq == 0) {
			// 盤面の石の数によって探索を深くする
			if ((cntB + cntW) == 4) depthNow = 10;
			else if ((cntB + cntW) == 55) depthNow = 11;
			else if ((cntB + cntW) == 54) depthNow = 12;
			else if ((cntB + cntW) == 53) depthNow = 13;
			else if ((cntB + cntW) == 52) depthNow = 14;
			else if ((cntB + cntW) == 51) depthNow = 15;
			else if ((cntB + cntW) == 50) depthNow = 16;
			else if ((cntB + cntW) == 49) depthNow = 17;
			else if ((cntB + cntW) == 48) depthNow = 18;
			else if ((cntB + cntW) == 47) depthNow = 19;
			else if (((cntB + cntW) >= 34) && ((cntB + cntW) <= 46)) depthNow = 12;

			// 何手目から完全解析を試みるかの設定
			if (steps >= 46) endThink = 1;
		}


		// モードの切り替え
		if (mode == 1) {
			if (turn == 0) select_player();		// マウスで石を置く
			else select_player();				// マウスで石を置く
		}
		else if (mode == 0) {
			if (turn == 0) select_player();		// マウスで石を置く
			else select_com();
		}


		// 埋まっている角に対応したフラグを立てる
		if (((bB | bW) >> 63) & 1) edgeFlag[0] = 1;		// 左上角
		if (((bB | bW) >> 56) & 1) edgeFlag[1] = 1;		// 右上角
		if (((bB | bW) >> 7) & 1) edgeFlag[2] = 1;		// 左下角
		if (((bB | bW) >> 0) & 1) edgeFlag[3] = 1;		// 右下角



		draw_board();			// 盤面の描画

		
		// メニューボタン
		int mx, my;		// マウスの座標
		GetMousePoint(&mx, &my);
		// メニューボタンクリック
		if ((mx > 382 && mx < 490) && (my > 10 && my < 40)) {
			DrawBox(380, 0, 492, 50, 0x767B87, TRUE);
			DrawFormatStringToHandle(388, 14, 0x000000, FHandle[4], "メニュー");
			DrawFormatStringToHandle(385, 12, 0xFFFFFF, FHandle[4], "メニュー");
			if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
				goto TITLE;
			}
		}


		ScreenFlip();


		// 終了判定
		countStone();			// 石を数える
		if (cntB + cntW == 64 || cntPass == 2 || cntB == 0 || cntW == 0) {
			WaitTimer(2000);
			ending();
			goto TITLE;
		}


		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			DrawString(400, 30, "終了", 0x000000);
			WaitKey();				// キー入力待ち
			endDebugConsole();		// コンソールの開放
			DxLib_End();			// ＤＸライブラリ使用の終了処理
			return 0;				// ソフトの終了 
		}
	}


	return 0;
}

void Handle_load() {
	GHandle[0] = LoadGraph("image/stone-black.png");
	GHandle[1] = LoadGraph("image/stone-white.png");
	GHandle[2] = LoadGraph("image/stone-gray.png");
	GHandle[3] = LoadGraph("image/board.png");
	GHandle[4] = LoadGraph("image/board-resultB.png");
	GHandle[5] = LoadGraph("image/board-resultW.png");
	GHandle[6] = LoadGraph("image/player.png");
	GHandle[7] = LoadGraph("image/com.png");
	GHandle[8] = LoadGraph("image/turnB.png");
	GHandle[9] = LoadGraph("image/turnW.png");
	GHandle[10] = LoadGraph("image/pass.png");
	GHandle[11] = LoadGraph("image/board-draw.png");
	GHandle[12] = LoadGraph("image/board-title.png");
	GHandle[13] = LoadGraph("image/stone-red.png");
	GHandle[14] = LoadGraph("image/stone-blue.png");

	FHandle[0] = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING);
	FHandle[1] = CreateFontToHandle("UD デジタル 教科書体 NK-B", 40, -1, DX_FONTTYPE_ANTIALIASING);
	FHandle[2] = CreateFontToHandle("UD デジタル 教科書体 NK-B", 35, -1, DX_FONTTYPE_ANTIALIASING);
	FHandle[3] = CreateFontToHandle("UD デジタル 教科書体 NK-B", 60, -1, DX_FONTTYPE_ANTIALIASING);
	FHandle[4] = CreateFontToHandle("UD デジタル 教科書体 NK-B", 30, -1, DX_FONTTYPE_ANTIALIASING);
	FHandle[5] = CreateFontToHandle("UD デジタル 教科書体 NK-B", 20, -1, DX_FONTTYPE_ANTIALIASING);
	FHandle[6] = CreateFontToHandle("UD デジタル 教科書体 NK-B", 42, -1, DX_FONTTYPE_ANTIALIASING);
}

void title() {



	while (ProcessMessage() != -1) {
		// クリックした状態でほかの要素が反応することを防ぐ
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0) clickFlag = 0;
		int mx, my;		// マウスの座標
		GetMousePoint(&mx, &my);
		
		DrawGraph(0, 0, GHandle[12], TRUE);		// 背景

		// 状態の描画
		{
			// 先後の下線
			if (turn == 0) DrawBox(325, 238, 404, 247, 0xB03307, TRUE);
			if (turn == 1) DrawBox(325, 308, 404, 317, 0xB03307, TRUE);

			// ★の背景
			if (randomFreq == 3) DrawBox(90, 190, 265, 228, 0xB03307, TRUE);
			if (randomFreq == 5) DrawBox(90, 231, 265, 268, 0xB03307, TRUE);
			if (randomFreq == 8) DrawBox(90, 271, 265, 308, 0xB03307, TRUE);
			if (randomFreq == 0) DrawBox(90, 311, 265, 348, 0xB03307, TRUE);

			// mode の背景
			if (mode == 0) DrawBox(90, 350, 265, 388, 0x00A2AB, TRUE);
			if (mode == 1) DrawBox(90, 390, 265, 428, 0x00A2AB, TRUE);

		}

		DrawFormatStringToHandle(99, 94, 0x000000, FHandle[3], "～ オセロ ～");
		DrawFormatStringToHandle(95, 90, 0xFFFFFF, FHandle[3], "～ オセロ ～");
		DrawFormatStringToHandle(331, 210, 0xEDEDED, FHandle[2], "先手");
		DrawFormatStringToHandle(330, 210, 0x000000, FHandle[2], "先手");
		DrawFormatStringToHandle(333, 280, 0x000000, FHandle[2], "後手");
		DrawFormatStringToHandle(330, 280, 0xFFFFFF, FHandle[2], "後手");

		DrawBox(90, 228, 265, 232, 0x000000, TRUE);		// 横線
		DrawBox(90, 268, 265, 272, 0x000000, TRUE);		// 横線
		DrawBox(90, 308, 265, 312, 0x000000, TRUE);		// 横線
		DrawBox(90, 186, 265, 190, 0x000000, TRUE);		// 横線
		DrawBox(90, 347, 265, 351, 0x000000, TRUE);		// 横線

		DrawFormatStringToHandle(160, 195, 0xE6E607, FHandle[2], "★");
		DrawFormatStringToHandle(142, 235, 0xE6E607, FHandle[2], "★★");
		DrawFormatStringToHandle(125, 275, 0xE6E607, FHandle[2], "★★★");
		DrawFormatStringToHandle(108, 315, 0xE6E607, FHandle[2], "★★★★");

		DrawFormatStringToHandle(303, 393, 0x000000, FHandle[2], "ＳＴＡＲＴ");
		DrawFormatStringToHandle(300, 390, 0xFFFFFF, FHandle[2], "ＳＴＡＲＴ");

		DrawFormatStringToHandle(94, 360, 0xFFFFFF, FHandle[5], "player vs com");
		DrawFormatStringToHandle(94, 400, 0xFFFFFF, FHandle[5], "player vs player");
		DrawBox(90, 388, 265, 392, 0x000000, TRUE);		// 横線
		DrawBox(90, 428, 265, 432, 0x000000, TRUE);		// 横線



		// クリック判定
		{
			// 先手
			if ((mx > 325 && mx < 400) && (my > 205 && my < 240)) {
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
					clickFlag = 1;	// クリックフラグを立てる
					turn = 0;
				}
			}
			// 後手
			if ((mx > 325 && mx < 400) && (my > 275 && my < 310) && (mode != 1)) {
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
					clickFlag = 1;	// クリックフラグを立てる
					turn = 1;
				}
			}
			// ★
			if ((mx > 90 && mx < 265) && (my > 140 && my < 226) && (mode != 1)) {
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
					clickFlag = 1;
					depthNow = 4;
					randomFreq = 3;
					playStanderdMovesFlag = 0;
				}
			}
			// ★★
			if ((mx > 90 && mx < 265) && (my > 233 && my < 266) && (mode != 1)) {
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
					clickFlag = 1;
					depthNow = 6;
					randomFreq = 5;
					playStanderdMovesFlag = 0;
				}
			}
			// ★★★
			if ((mx > 90 && mx < 265) && (my > 274 && my < 306) && (mode != 1)) {
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
					clickFlag = 1;
					depthNow = 8;
					randomFreq = 8;
					playStanderdMovesFlag = 0;
				}
			}
			// ★★★★
			if ((mx > 90 && mx < 265) && (my > 316 && my < 346) && (mode != 1)) {
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
					clickFlag = 1;
					depthNow = 10;
					randomFreq = 0; 
					playStanderdMovesFlag = 1;
				}
			}
			// player vs com
			if ((mx > 90 && mx < 265) && (my > 350 && my < 388)) {
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
					clickFlag = 1;
					mode = 0;
					randomFreq = 3;
				}
			}
			// player vs player
			if ((mx > 90 && mx < 265) && (my > 390 && my < 428)) {
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
					clickFlag = 1;
					mode = 1;
					turn = 0;
					randomFreq = -1;
				}
			}
			// ＳＴＡＲＴ
			if ((mx > 300 && mx < 425) && (my > 390 && my < 422)) {
				DrawBox(296, 385, 429, 426, 0xB03307, TRUE);
				DrawFormatStringToHandle(303, 393, 0x000000, FHandle[2], "ＳＴＡＲＴ");
				DrawFormatStringToHandle(300, 390, 0xFFFFFF, FHandle[2], "ＳＴＡＲＴ");
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {

					// 先後の設定
					if (turn == 0) {
						playerColor = 0;
						comColor = 1;
					}
					if (turn == 1) {
						playerColor = 1;
						comColor = 0;
						//turn = 0;
					}
					break;
				}
			}
		}


		ScreenFlip();




		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			DrawString(400, 30, "終了", 0x000000);
			WaitKey();				// キー入力待ち
			endDebugConsole();		// コンソールの開放
			DxLib_End();			// ＤＸライブラリ使用の終了処理
			return ;				// ソフトの終了 
		}
	}
}

void load_standerdMoves() {

	FILE* fp;
	int lineCount = 0;

	// ファイルを開く
	fp = fopen("standerdmoves3.txt", "r");
	if (fp == NULL) {
		printf("ファイルを開けませんでした");
		return;
	}

	// ファイルから一行ずつ読み取る
	while (fgets(standerdMoves[lineCount], 128, fp) != NULL) {
		// 改行文字を削除
		standerdMoves[lineCount][strcspn(standerdMoves[lineCount], "\n")] = '\0';

		for (int i = 0; standerdMoves[lineCount][i] != '\0'; i++) {
			standerdMoves[lineCount][i] = tolower((unsigned char)standerdMoves[lineCount][i]); // 1文字ずつ小文字に変換
		}

		char* tmp = standerdMoves[lineCount];
		int len = (int)strlen(tmp);	// 定石の長さを取得
		lineCount++;


		// f5 始まりの定石を4方向に対応させる
		for (int i = 1; i <= 3; i++) {
			// 元の定石をコピー
			strcpy(standerdMoves[lineCount], tmp);

			// 定石を変換
			for (int j = 0; j < len; j += 2) {
				int x = standerdMoves[lineCount][j] - 'a';   // アルファベットを座標数字に変換
				int y = standerdMoves[lineCount][j + 1] - '1'; // 数値変換


				switch (i) {
				case 1:
					// 90度回転してx軸を反転
					standerdMoves[lineCount][j + 1] = x + '1';   // 数値を文字に戻す
					standerdMoves[lineCount][j] = (7 - (8 - y - 1)) + 'a'; // 数値を文字に戻す
					break;
				case 2:
					// 90度回転してy軸を反転
					standerdMoves[lineCount][j + 1] = 7 - x + '1';   // 数値を文字に戻す
					standerdMoves[lineCount][j] = (8 - y - 1) + 'a'; // 数値を文字に戻す
					break;
				case 3:
					// x軸とy軸を反転
					standerdMoves[lineCount][j + 1] = (8 - y - 1) + '1';   // 数値を文字に戻す
					standerdMoves[lineCount][j] = 7 - x + 'a'; // 数値を文字に戻す
					break;
				}
			}
			lineCount++;
		}
	}

	for (int i = 0; i < lineCount; i++) {
		printf("%s\n", standerdMoves[i]);
	}


}

void initialize() {

	// ビットボードの初期化
	bB = 0b0000000000000000000000000000100000010000000000000000000000000000;	// bitboard Black
	bW = 0b0000000000000000000000000001000000001000000000000000000000000000;	// bitboard White
	bN = 0b0000000000000000000000000000000000000000000000000000000000000000;	// bitboard Now  現在の盤面

	mode = 0;
	cntPass = 0;					// パスのカウントを初期化
	turn = 0;						// メニュー画面を作成したら削除する
	steps = 1;						// 手数カウントをリセット
	endThink = 0;					// ４４手以降かを判定用
	playStanderdMovesFlag = 1;		// 定石を抜けたかのフラグ
	randomFreq = 3;					// ランダムに指す確率
	putPlace = 0;

	// 埋まっている角を表すフラグを初期化
	for (int i = 0; i < 4; i++) edgeFlag[i] = 0;


	countStone();	// 石の数を初期化　ＣＯＭが先手の時先に初期化していないと maxLevel() がうまく動かない


	cntValid = 0;
	load_standerdMoves();		// 定石ファイルを読み込みなおす
}

void ending() {

	if (cntB > cntW) DrawGraph(0, 0, GHandle[4], TRUE);			// 盤の描画  黒勝ち
	else if (cntB < cntW) DrawGraph(0, 0, GHandle[5], TRUE);		// 盤の描画　白勝ち
	else if (cntB == cntW) DrawGraph(0, 0, GHandle[11], TRUE);	// 盤の描画　引き分け

	DrawFormatStringToHandle(208, 165, 0x000000, FHandle[1], "%2d", cntB);
	DrawFormatStringToHandle(208, 252, 0xFFFFFF, FHandle[1], "%2d", cntW);

	DrawBox(100,300,400 , 400, 0x009040, TRUE);
	DrawBox(100,70,400 , 150, 0x009040, TRUE);


	DrawFormatStringToHandle(209, 86, 0x000000, FHandle[6], "結果");
	DrawFormatStringToHandle(206, 84, 0xDFE42E, FHandle[6], "結果");
	DrawFormatStringToHandle(130, 352, 0x000000, FHandle[6], "Ｐｒｅｓｓ Ｅｎｔｅｒ");
	DrawFormatStringToHandle(127, 350, 0xFFFFFF, FHandle[6], "Ｐｒｅｓｓ Ｅｎｔｅｒ");

	ScreenFlip();

	// エンターもしくはエスケープキーでメニューに戻る
	while (ProcessMessage() != -1) {
		if (CheckHitKey(KEY_INPUT_ESCAPE) || CheckHitKey(KEY_INPUT_RETURN)) {
			break;
		}
	}
}

void draw_board() {

	DrawGraph(0, 0, GHandle[3], TRUE);	// 盤の描画


	// 合法手の描画
	for (int i = 63; i >= 0; i--) {
		if (((bSelectable >> i) & 1) == 1) {
			int xx = abs((63 - i) % 8);
			int yy = abs((63 - i) / 8);
			DrawExtendGraph((baseX + selectableSize) + xx * masuSize, (baseY + selectableSize) + yy * masuSize, ((baseX - selectableSize) + xx * masuSize) + masuSize, ((baseY - selectableSize) + yy * masuSize) + masuSize, GHandle[2], TRUE);
		}
	}

	// 黒石の描画
	for (int i = 63; i >= 0; i--) {
		if (((bB >> i) & 1) == 1) {
			int xx = abs((63 - i) % 8);
			int yy = abs((63 - i) / 8);
			DrawExtendGraph((baseX + stoneSize) + xx * masuSize, (baseY + stoneSize) + yy * masuSize, ((baseX - stoneSize) + xx * masuSize) + masuSize, ((baseY - stoneSize) + yy * masuSize) + masuSize, GHandle[0], TRUE);
		}
	}
	// 白石の描画
	for (int i = 63; i >= 0; i--) {
		if (((bW >> i) & 1) == 1) {
			int xx = abs((63 - i) % 8);
			int yy = abs((63 - i) / 8);
			DrawExtendGraph((baseX + stoneSize) + xx * masuSize, (baseY + stoneSize) + yy * masuSize, ((baseX - stoneSize) + xx * masuSize) + masuSize, ((baseY - stoneSize) + yy * masuSize) + masuSize, GHandle[1], TRUE);
		}
	}
	// 直前手の印
	for (int i = 63; i >= 0; i--) {
		if (((putPlace >> i) & 1) == 1) {
			int xx = abs((63 - i) % 8);
			int yy = abs((63 - i) / 8);
			DrawExtendGraph((baseX + justBeforeSize) + xx * masuSize, (baseY + justBeforeSize) + yy * masuSize, ((baseX - justBeforeSize) + xx * masuSize) + masuSize, ((baseY - justBeforeSize) + yy * masuSize) + masuSize, GHandle[13], TRUE);
		}
	}


	// パスボタンの描画（合法手がない場合）
	if (bSelectable == 0) DrawGraph(0, 0, GHandle[10], TRUE);


	// 左下に各色の石の数を描画
	// １桁と２桁で別の調整をする	
	if (cntB < 10) DrawFormatStringToHandle(71, 460, 0xFFFFFF, FHandle[0], "%d", cntB);
	else if (cntB >= 10) DrawFormatStringToHandle(63, 460, 0xFFFFFF, FHandle[0], "%2d", cntB);
	if (cntW < 10) DrawFormatStringToHandle(133, 460, 0x000000, FHandle[0], "%d", cntW);
	else if (cntW >= 10) DrawFormatStringToHandle(125, 460, 0x000000, FHandle[0], "%2d", cntW);

	// 手番を表示
	if (playerColor == 0 && turn == 0) DrawGraph(0, 0, GHandle[8], TRUE);
	else if (playerColor == 1 && turn == 0) DrawGraph(0, 0, GHandle[9], TRUE);
	// 手番を表示
	if (playerColor == 0 && turn == 1) DrawGraph(0, 0, GHandle[9], TRUE);
	else if (playerColor == 1 && turn == 1) DrawGraph(0, 0, GHandle[8], TRUE);

	// 現在の手数を表示
	DrawFormatStringToHandle(327, 460, 0xDCDCDC, FHandle[2], "%2d手目", (cntB + cntW) - 3);


	// 選択しているレベルを描画
	if(randomFreq == 3) DrawFormatStringToHandle(22, 13, 0xE6E607, FHandle[4], "★");
	if(randomFreq == 5) DrawFormatStringToHandle(22, 13, 0xE6E607, FHandle[4], "★★");
	if(randomFreq == 8) DrawFormatStringToHandle(22, 13, 0xE6E607, FHandle[4], "★★★");
	if(randomFreq == 0) DrawFormatStringToHandle(22, 13, 0xE6E607, FHandle[4], "★★★★");


	// メニューボタン
	DrawFormatStringToHandle(388, 14, 0x000000, FHandle[4], "メニュー");
	DrawFormatStringToHandle(385, 12, 0xFFFFFF, FHandle[4], "メニュー");
}

void printBits(long long n) {
	for (int i = 63; i >= 0; i--) {
		printf("%lld ", (n >> i) & 1);
		if (i % 8 == 0) printf("\n");  // 8ビットごとに改行を入れる
	}
	printf("\n");
}

void put(int x, int y, int color) {
	// turn :: 0 = 黒 , 1 = 白

	unsigned long long d = 0b1000000000000000000000000000000000000000000000000000000000000000;	// 特定の位置に石を置くためのビット列　論理シフトして使う

	unsigned long long n = y * 8 + x;	// 該当箇所にビットを立てる

	putPlace = d >> n;	// 石を置いた場所にビットが立ったビット列


	if (color == 0) {
		bB |= putPlace;
		bN = bB | bW;
	}
	else if (color == 1) {
		bW |= putPlace;
		bN = bB | bW;
	}
}

void remove(int x, int y, int color) {
	// turn :: 0 = 黒 , 1 = 白

	unsigned long long d = 0b1000000000000000000000000000000000000000000000000000000000000000;	// 特定の位置に石を置くためのビット列　論理シフトして使う

	unsigned long long n = y * 8 + x;	// 該当箇所にビットを立てる

	putPlace = d >> n;	// 石を置いた場所にビットが立ったビット列


	if (color == 0) {
		bB ^= putPlace;
		bN = bB | bW;
	}
	else if (color == 1) {
		bW ^= putPlace;
		bN = bB | bW;
	}
}

void search_selectable(int color) {

	unsigned long long bC1 = 0;		// Bitboard Color1	手番の石の色
	unsigned long long bC2 = 0;		// Bitboard Color2
	unsigned long long bTC1 = 0;		// Bitboard Color2
	unsigned long long bTC2 = 0;		// Bitboard Color2
	unsigned long long bTmp = 0;	// 一時変数
	bSelectable = 0;	// 合法手をリセット

	if (color == 0) {
		bC1 = bB;
		bC2 = bW;
	}
	else if (color == 1) {
		bC1 = bW;
		bC2 = bB;
	}


	// 各方向の合法手を探す
	{
		// 右
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			// horizontalMask の０の部分で bC1 のビットを消したいけど、直接 bC1 を変更すると後のコードで使えなくなるので、bTC1 という一時変数に退避させる
			bTC1 = (horizontalMask & bC2) & (bTC1 >> 1);
			bTmp |= (horizontalMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp >> 1) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;

		// 左
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			bTC1 = (horizontalMask & bC2) & (bTC1 << 1);
			bTmp |= (horizontalMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp << 1) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;

		// 上
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			bTC1 = (verticalMask & bC2) & (bTC1 << 8);
			bTmp |= (verticalMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp << 8) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;

		// 下
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			bTC1 = (verticalMask & bC2) & (bTC1 >> 8);
			bTmp |= (verticalMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp >> 8) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;

		// 右上
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			bTC1 = (allsideMask & bC2) & (bTC1 << 7);
			bTmp |= (allsideMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp << 7) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;

		// 左上
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			bTC1 = (allsideMask & bC2) & (bTC1 << 9);
			bTmp |= (allsideMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp << 9) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;

		// 右下
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			bTC1 = (allsideMask & bC2) & (bTC1 >> 9);
			bTmp |= (allsideMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp >> 9) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;

		// 左下
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			bTC1 = (allsideMask & bC2) & (bTC1 >> 7);
			bTmp |= (allsideMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp >> 7) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;
	}
}

void select_player() {

	int mx, my;		// マウスの座標

	GetMousePoint(&mx, &my);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {


			if ((mx > baseX + (masuSize * j) && mx < baseX + (masuSize * j) + masuSize) &&
				(my > baseY + (masuSize * i) && my < baseY + (masuSize * i) + masuSize)) {

				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {

					clickFlag = 1;	// クリックフラグを立てる

					// クリックしたマスが合法手か判定する。合法手の場合のみ石を置く
					for (int k = 63; k >= 0; k--) {

						if ((bSelectable >> k) & 1) {

							// x座標とy座標
							int xx = abs((63 - k) % 8);
							int yy = abs((63 - k) / 8);

							if (xx == j && yy == i) {
								put(j, i, playerColor);		// 石を置く
								reverse(playerColor);			// 石をひっくり返す
								turn = (turn + 1) % 2;	// 手番を入れ替える
								cntPass = 0;	// パスのカウントを初期化
								steps++;		// 手数をカウント
								if (mode == 1) playerColor = (playerColor + 1) % 2;		// player vs player で手番を入れ替える
							}
						}
					}
				}
			}
		}
	}

	// パスボタンをクリック
	if (bSelectable == 0 && ((mx > 200 && mx < 300) && (my > 450 && my < 500))) {
		if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			clickFlag = 1;
			cntPass++;
			turn = (turn + 1) % 2;
		}
	}
}

void select_com() {

	search_selectable(comColor);
	if (bSelectable == 0) {		// 置くますがない場合パスを選択
		turn = (turn + 1) % 2;
		cntPass++;
		WaitTimer(1000);
		return;
	}
	draw_board();
	ScreenFlip();
	
	if(randomFreq > 0) WaitTimer(500);		// com のレベルが低いときに設定する（画面がちらつくのを防ぐため）



	// ランダムに指すか思考ルーチンを通すか乱数で判定
	int random = GetRand(randomFreq - 1);


	if (random == 0 && randomFreq != 0) {
		// ランダムに指す
		printf("ランダム\n");

		int ranX = 0;			// ランダムに指すマスＸ
		int ranY = 0;			// ランダムに指すマスＹ
		int ranPutPlace = 0;	// ランダムに石を置いたマスを示す
		unsigned long long d = 0b1000000000000000000000000000000000000000000000000000000000000000;	// 特定の位置に石を置くためのビット列　論理シフトして使う
		unsigned long long n = 0;	// 該当箇所にビットを立てる

		while (1) {
			ranX = GetRand(7);
			ranY = GetRand(7);
			n = ranY * 8 + ranX;
			putPlace = d >> n;


			if ((bSelectable | putPlace) == bSelectable) {
				bestX = ranX;
				bestY = ranY;
				break;
			}
		}
	}
	else {
		// ミニマックス法
		printf("ミニマックス\n");

		depthNum = 0;	// 現在の探索数をリセット

		// レベル４の時だけ定石を指す
		if (playStanderdMovesFlag == 1) playStanderdMovesFlag = playStanderdMoves();		// 一度定石を抜けたらその先定石に合流しても定石を指さない（誤動作防止）

		if (playStanderdMovesFlag == 0) {
			if (steps <= 20) {		// ３０手目までは開放度理論のミニマックス法を使う
				int dumy = 0;	// 最初だけダミーでアドレスを渡す
				if (comColor == 0) maxLevel_openness(-INFINITY, INFINITY, 1, &dumy);
				else if (comColor == 1) minLevel_openness(-INFINITY, INFINITY, 1, &dumy);
			}
			else {
				if (comColor == 0) maxLevel(-INFINITY, INFINITY, 1);
				else if (comColor == 1) minLevel(-INFINITY, INFINITY, 1);
			}
		}
	}


	put(bestX, bestY, comColor);
	reverse(comColor);
	printf("%8d  depthNow = %2d  %2d手目\n", depthNum, depthNow, steps);
	turn = (turn + 1) % 2;	// 手番を入れ替える
	steps++;
	cntPass = 0;	// パスのカウントを初期化
}

void countStone() {

	// 石のカウントを初期化
	cntB = 0;
	cntW = 0;

	// 黒と白の石の数を数える
	for (int i = 0; i < 64; i++) {
		if ((bB >> i) & 1) cntB++;
		if ((bW >> i) & 1) cntW++;
	}
}

void reverse(int color) {

	unsigned long long bC1 = 0;  // 手番の石の色
	unsigned long long bC2 = 0;  // 相手の石の色

	// 手番に応じたボードを設定
	if (color == 0) {
		bC1 = bB;
		bC2 = bW;
	}
	else if (color == 1) {
		bC1 = bW;
		bC2 = bB;
	}

	rev = 0;  // 反転する位置を格納するビットボード

	// ８方向をチェックする
	for (int i = 0; i < 8; i++) {

		unsigned long long rev_ = 0;  // 各方向で反転する場所
		unsigned long long mask = transfer(putPlace, i);	  // mask : ひっくり返せる石の場所
		unsigned long long maskPrev = 0;  // 一回動かす前の mask を保存

		// 石を置いた場所から任意の方向に動かし、相手の石が続いているかを確認
		while ((mask != 0) && ((mask & bC2) != 0)) {
			rev_ |= mask;			   // 相手の石を反転候補に追加
			maskPrev = mask;
			mask = transfer(mask, i);  // 次の場所を調べる
		}

		// 端まで行った mask は番兵によって値が０になってしまうので、
		// その一つ前の値を maskPrev に退避させて任意の方向に１マス動かし bC1 との論理積をとる
		switch (i) {
		case 0: //右
			maskPrev = (maskPrev >> 1);
			break;
		case 1: //左
			maskPrev = (maskPrev << 1);
			break;
		case 2: //上
			maskPrev = (maskPrev << 8);
			break;
		case 3: //下
			maskPrev = (maskPrev >> 8);
			break;
		case 4: //右上
			maskPrev = (maskPrev << 7);
			break;
		case 5: //左上
			maskPrev = (maskPrev << 9);
			break;
		case 6: //右下
			maskPrev = (maskPrev >> 9);
			break;
		case 7: //左下
			maskPrev = (maskPrev >> 7);
			break;
		}

		// 最後に自分の石があれば、その方向で反転できる
		if ((maskPrev & bC1) != 0) {
			rev |= rev_;  // 反転する場所を確定
		}
	}



	// 反転処理
	bC1 |= (putPlace | rev);  // 自分の石と反転する場所を反転
	bC2 ^= rev;				  // 相手の石を反転

	// 反転後、グローバルボードに更新を反映させる
	if (color == 0) {
		bB = bC1;  // 黒のボードに反映（手番が黒の場合）
		bW = bC2;  // 白のボードに反映（手番が白の場合）
	}
	else if (color == 1) {
		bB = bC2;  // 黒のボードに反映（手番が黒の場合）
		bW = bC1;  // 白のボードに反映（手番が白の場合）
	}
}

unsigned long long transfer(unsigned long long  putPlace, int k) {

	switch (k) {
	case 0: //右
		return (putPlace >> 1) & horizontalMask;
	case 1: //左
		return (putPlace << 1) & horizontalMask;
	case 2: //上
		return (putPlace << 8) & verticalMask;
	case 3: //下
		return (putPlace >> 8) & verticalMask;
	case 4: //右上
		return (putPlace << 7) & allsideMask;
	case 5: //左上
		return (putPlace << 9) & allsideMask;
	case 6: //右下
		return (putPlace >> 9) & allsideMask;
	case 7: //左下
		return (putPlace >> 7) & allsideMask;
	default:
		return 0;
	}
}

int playStanderdMoves() {

	char nowPut[10] = { "" };
	int xx = 0;
	int yy = 0;
	char cx[5];			// 定石のアルファベット部分を格納
	char dumy[128];		// 定石の最初の二文字以降の文字列を便宜上格納
	int cntNewValid = 0;	// 有効な定石の数をカウント
	int validFlag = 0;	// 有効な定石の数をカウント


	// com が先手の時の初手を決める
	{
		if (steps == 1 && comColor == 0) {
			int ranNum = GetRand(3);

			switch (ranNum) {
			case 0:
				bestX = 5;
				bestY = 4;
				break;
			case 1:
				bestX = 4;
				bestY = 5;
				break;
			case 2:
				bestX = 2;
				bestY = 3;
				break;
			case 3:
				bestX = 3;
				bestY = 2;
				break;
			}

			sprintf(nowPut, "%c%d", 'a' + bestX, bestY + 1);		// 指し手を定石の形式に変換


			// 指し手に対応する定石を valid[] に格納する（先頭から２文字削除する）
			for (int i = 0; i < MAXSTANDERDMOVES; i++) {
				if (strncmp(nowPut, standerdMoves[i], 2) == 0) {
					strcpy(valid[cntValid++], standerdMoves[i] + 2);
					printf("valid[%d] = %s\n", cntValid - 1, valid[cntValid - 1]);
				}
			}

			return 1;
		}
	}



	// プレイヤーが置いた石の座標を 'a3' のような型式に変換
	for (int i = 63; i >= 0; i--) {
		if ((putPlace >> i) & 1) {
			xx = abs((63 - i) % 8);		// プレイヤーが置いた石の座標 X
			yy = abs((63 - i) / 8);		// プレイヤーが置いた石の座標 Y
			sprintf(nowPut, "%c%d", 'a' + xx, yy + 1);		// 指し手を定石の形式に変換
		}
	}

	// 初手だけ全定石から検索する
	if (steps == 1 || steps == 2) {
		cntValid = 0;
		// 指し手に対応する定石を valid[] に格納する（先頭から２文字削除する）
		for (int i = 0;i < MAXSTANDERDMOVES;i++) {
			if (strncmp(nowPut, standerdMoves[i], 2) == 0) {
				strcpy(valid[cntValid++], standerdMoves[i] + 2);
				validFlag = 1;
			}
		}
	}
	else {		// ２手目以降は有効な定石の配列から検索する
		cntNewValid = 0;
		for (int i = 0; i < cntValid; i++) {
			if (strncmp(nowPut, valid[i], 2) == 0) {
				strcpy(valid[cntNewValid++], valid[i] + 2);

				if (valid[cntNewValid - 1][0] == '\0') return 0;
				validFlag = 1;
			}
		}
		cntValid = cntNewValid;		// 有効な定石の数を更新
	}


	// 現在有効な定石の中から次の指し手を選択する
	if (validFlag == 1) {
		validFlag = 0;

		int ranNum = GetRand(cntValid - 1);

		// 有効な定石の中からからランダムに選択　
		(void)sscanf(valid[ranNum], "%c%d%s", cx, &yy, dumy);
		xx = cx[0] - 97;	// ASCIIコード表で abcdefgh を数字に変換する
		yy -= 1;

		bestX = xx;		// 最善手を更新
		bestY = yy;


		sprintf(nowPut, "%c%d", 'a' + xx, yy + 1);		// 指し手を定石の形式に変換


		// 有効な定石を更新
		cntNewValid = 0;
		for (int i = 0; i < cntValid; i++) {
			if (strncmp(nowPut, valid[i], 2) == 0) {
				strcpy(valid[cntNewValid++], valid[i] + 2);
				printf("valid[%d] = %s\n", cntNewValid - 1, valid[cntNewValid - 1]);
			}
		}
		cntValid = cntNewValid;	// 有効な定石の数を更新

		return 1;
	}

	return 0;
}


int maxLevel(int alpha, int beta, int depth) {
	if (depth >= depthNow) {
		//printf("koko");
		return evaluation_function(0); // 評価関数の値を返す
	}
	countStone();
	if (cntB + cntW == 64) return evaluation_function(0);		// 盤面が全部埋まったら評価関数の値を返す
	if (cntB == 0 || cntW == 0) return evaluation_function(0);	// どちらか一方の石がゼロになったら評価関数を返す

	int arrScore[30] = { 0 };	// depth = 1 の時の合法手のスコアを格納
	int cntArrScore = 0;		// arrScore[] のインデックス用

	int score_max = (int)(-INFINITY);  // 最大スコア（負の無限大で初期化)

	int xx = 0;		// 仮置きの座標 X	最も評価の高い手を最善手にするために用意する
	int yy = 0;		// 仮置きの座標 Y



	for (int i = 63; i >= 0; i--) {
		search_selectable(0);		// 黒が打てる場所を探す
		if ((bSelectable >> i) & 1) {

			if (++depthNum > DEPTHNUM) {
				//printf("koko");
				return evaluation_function(0);		//  あらかじめ決めた局面数に達したら評価関数の値を返す
			}

			int score = 0;
			xx = abs((63 - i) % 8);		// 仮置きの座標 X
			yy = abs((63 - i) / 8);		// 仮置きの座標 Y

			put(xx, yy, 0);		// 黒の石を暫定で置く
			steps++;			// 手数をカウント
			int tmpCntPass = cntPass;
			cntPass = 0;		// パスのカウントをリセット
			reverse(0);			// 石をひっくり返す
			unsigned long long tmpPutPlace = putPlace;	 // 置いた石のマスを保存　局面ごとに保存しとかないと値が更新されてしまうから
			unsigned long long tmpRev = rev;			 // ひっくり返した石のマスを保存　理由は同上


			// 開放度理論 ***********



			// 暫定で黒石を置いたことで白石が角に置けるようになっていないかチェックする
			// 白石の合法手の中で角が含まれていないかを判定（角が含まれている場合その手は合法手ではなかったと判断する）
			search_selectable(1);
			countStone();
			if ((((bSelectable >> 63) & 1) || ((bSelectable >> 56) & 1) ||
				((bSelectable >> 7) & 1) || ((bSelectable >> 0) & 1)) && (endThink != 1)) {

				//// reverse の反対の操作	
				score = -500000 - cntB;						// プラスマイナス変えてみる
				depthNum--;		// 手数をひとつ戻す
				goto SKIP;		// 次の候補手を探索する
			}

			// X のマスに置かないようにする
			if ((((tmpPutPlace >> 54) & 1) || ((tmpPutPlace >> 49) & 1) ||
				((tmpPutPlace >> 14) & 1) || ((tmpPutPlace >> 9) & 1)) && (endThink != 1)) {

				score = -99999 - cntB;
				goto SKIP;		// 次の候補手を探索する
			}


			score = minLevel(alpha, beta, depth + 1);  // 次は相手のターン


		SKIP:


			// reverse の反対の操作
			bB = bB ^ tmpRev;		// ひっくり返した石のマスを空にする
			bW = bW | tmpRev;		// ひっくり返されていた白のマスにビットを立てる
			bB = bB ^ tmpPutPlace;	// 暫定で置いた黒の石を取り除く
			steps--;				// 手数をひとつ戻す




			// β値を超えると枝刈り
			if (score >= beta) {
				return score;
			}
			// α値を更新
			if (score > score_max) {
				score_max = score;
				alpha = max(alpha, score_max);

				if (depth == 1) {
					// 最善手を記録
					bestX = xx;
					bestY = yy;
				}
			}

			// depth = 1 の時のスコアを格納
			if (depth == 1) {
				arrScore[cntArrScore++] = score;
			}
		}
	}

	for (int i = 0; i < cntArrScore; i++) printf("%6d  ", arrScore[i]);		// debug 用合法手のスコア表示

	// 置く場所がない = パス
	if (bSelectable == 0) {
		cntPass++;

		// お互いがパスの場合それ以降石が置かれることがないからその盤面を評価する
		if (cntPass == 2) {
			cntPass = 0;
			return evaluation_function(3);
		}

		int score = minLevel(alpha, beta, depth + 1);  // 次は相手のターン;

		// β値を超えると枝刈り
		if (score >= beta) {
			return score;
		}

		// α値を更新
		if (score > score_max) {
			score_max = score;
			alpha = max(alpha, score_max);

			if (depth == 1) {
				// 最善手を記録
				bestX = xx;
				bestY = yy;
			}
		}
	}


	//// 置ける場所がなかったらの記述をする
	//if (score_max == (int)(-INFINITY)) return evaluation_function(0); // 評価関数の値を返す


	return score_max;
}

int minLevel(int alpha, int beta, int depth) {
	if (depth >= depthNow) {
		//printf("koko");
		return evaluation_function(1); // 評価関数の値を返す
	}
	countStone();
	if (cntB + cntW == 64) return evaluation_function(1);	 // 盤面が全部埋まったら評価関数の値を返す
	if (cntB == 0 || cntW == 0) return evaluation_function(1);	// どちらか一方の石がゼロになったら評価関数を返す

	int arrScore[30] = { 0 };	// depth = 1 の時の合法手のスコアを格納
	int cntArrScore = 0;		// arrScore[] のインデックス用

	int score_min = (int)(INFINITY);  // 最大スコア（正の無限大で初期化)

	int xx = 0;		// 仮置きの座標 X	最も評価の高い手を最善手にするために用意する
	int yy = 0;		// 仮置きの座標 Y


	// 指すことが可能な座標全てに対して探索を行う
	for (int i = 63; i >= 0; i--) {

		search_selectable(1);

		if ((bSelectable >> i) & 1) {

			if (++depthNum > DEPTHNUM) {
				return evaluation_function(1);		//  あらかじめ決めた局面数に達したら評価関数の値を返す
			}

			int score = 0;
			xx = abs((63 - i) % 8);		// 仮置きの座標 X	
			yy = abs((63 - i) / 8);		// 仮置きの座標 Y

			put(xx, yy, 1);		// 白の石を暫定で置く
			steps++;			// 手数をカウント
			int tmpCntPass = cntPass;
			cntPass = 0;		// パスのカウントをリセット
			reverse(1);		// 石をひっくり返す
			unsigned long long tmpPutPlace = putPlace;	 // 置いた石のマスを保存　局面ごとに保存しとかないと値が更新されてしまうから
			unsigned long long tmpRev = rev;			 // ひっくり返した石のマスを保存　理由は同上


			// 暫定で白石を置いたことで黒石が角に置けるようになっていないかチェックする
			// 白石の合法手の中で角が含まれていないかを判定（角が含まれている場合その手は合法手ではないと判断する）
			search_selectable(0);
			countStone();
			if ((((bSelectable >> 63) & 1) || ((bSelectable >> 56) & 1) ||
				((bSelectable >> 7) & 1) || ((bSelectable >> 0) & 1)) && (endThink != 1)) {	// 左上角

				//// reverse の反対
				score = 500000 + cntW;		// ( + cntW ) が正しいのか要検討 **************************************************
				depthNum--;
				goto SKIP;		// 次の候補手を探索する

			}

			// X のマスに置かないようにする
			if ((((tmpPutPlace >> 54) & 1) || ((tmpPutPlace >> 49) & 1) ||
				((tmpPutPlace >> 14) & 1) || ((tmpPutPlace >> 9) & 1)) && (endThink != 1)) {

				score = 99999 + cntW;		// ( + cntW ) が正しいのか要検討 **************************************************
				goto SKIP;		// 次の候補手を探索する
			}


			score = maxLevel(alpha, beta, depth + 1);  // 次は相手のターン


		SKIP:


			// reverse の反対
			bW = bW ^ tmpRev;		// ひっくり返した石のマスを空にする
			bB = bB | tmpRev;		// ひっくり返されていた黒のマスにビットを立てる
			bW = bW ^ tmpPutPlace;	// 暫定で置いた白の石を取り除く	
			steps--;				// 手数をひとつ戻す


			// α値を超えると枝刈り
			if (score <= alpha) {
				return score;
			}
			// β値を更新
			if (score < score_min) {
				score_min = score;
				beta = min(beta, score_min);

				if (depth == 1) {
					// 最適な手を記録
					bestX = xx;
					bestY = yy;
				}
			}

			// depth = 1 の時のスコアを格納
			if (depth == 1) {
				arrScore[cntArrScore++] = score;
			}
		}
	}

	for (int i = 0; i < cntArrScore; i++) printf("%6d  ", arrScore[i]);		// debug 用合法手のスコア表示

	// 置く場所がない = パス
	if (bSelectable == 0) {
		cntPass++;

		// お互いがパスの場合それ以降石が置かれることがないからその盤面を評価する
		if (cntPass == 2) {
			cntPass = 0;
			return evaluation_function(3);
		}

		int score = maxLevel(alpha, beta, depth + 1);  // 次は相手のターン;


		// α値を超えると枝刈り
		if (score <= alpha) {
			return score;
		}
		// β値を更新
		if (score < score_min) {
			score_min = score;
			beta = min(beta, score_min);

			if (depth == 1) {
				// 最適な手を記録
				bestX = xx;
				bestY = yy;
			}
		}

	}


	//// 置ける場所がなかったらの記述をする
	//if (score_min == (int)(INFINITY)) return evaluation_function(1); // 評価関数の値を返す


	return score_min;
}

int evaluation_function(int index) {

	int end = 0;
	int scoreXB = 0, scoreXW = 0;	// ３つの評価関数の値を格納する変数
	int scoreYB = 0;
	int scoreYW = 0;
	int scoreZB = 0;
	int scoreZW = 0;
	int scoreAB = 0;
	int scoreAW = 0;
	int result = 0;
	int cnt = 0;


	countStone();



	{
		// 石が０枚になった時の記述
		if (cntB == 0) {
			end = -99999;
			goto SKIP;
		}
		else if (cntW == 0) {
			end = 99999;
			goto SKIP;
		}


		// 黒か白の石の数が０枚の場合、スコアを 99999 にする

		if (endThink == 1) {

			if (cntB > 32) {
				end = 99999 + (cntB - cntW);
				goto SKIP;
			}
			else if (cntB < 32) {
				end = -99999 + (cntB - cntW);
				goto SKIP;
			}
			else if (cntB == cntW) {
				end = 0;
				goto SKIP;
			}
		}
	}


	{
		// 角に相手の石がある場合評価を下げる
		{
			if (endThink == 0) {
				// 黒
				if (((bB >> 63) & 1) && (edgeFlag[0] == 0)) {
					//printf("B");
					end += 150000;
				}
				if (((bB >> 56) & 1) && (edgeFlag[1] == 0)) {
					//printf("B");
					end += 150000;
				}
				if (((bB >> 7) & 1) && (edgeFlag[2] == 0)) {
					//printf("B");
					end += 150000;
				}
				if (((bB >> 0) & 1) && (edgeFlag[3] == 0)) {
					//printf("B");
					end += 150000;
				}
				// 白
				if (((bW >> 63) & 1) && (edgeFlag[0] == 0)) {
					//printf("W");
					end -= 150000;
				}
				if (((bW >> 56) & 1) && (edgeFlag[1] == 0)) {
					//printf("W");
					end -= 150000;
				}

				if (((bW >> 7) & 1) && (edgeFlag[2] == 0)) {
					//printf("W");
					end -= 150000;
				}
				if (((bW >> 0) & 1) && (edgeFlag[3] == 0)) {
					//printf("W");
					end -= 150000;
				}
			}
		}
	}



	// 石がある場所による評価（ X ）**********************************
	for (int i = 0; i < 64; i++) {
		if ((bB >> i) & 1) scoreXB += point[i];		// 黒の得点
		if ((bW >> i) & 1) scoreXW += point[i];		// 白の得点
	}


	// 着手可能箇所による評価（ Y ）**********************************
	search_selectable(0);
	for (int i = 0; i < 64; i++) {
		if ((bSelectable >> i) & 1) scoreYB += (20 + point[i]);
	}
	search_selectable(1);
	for (int i = 0; i < 64; i++) {
		if ((bSelectable >> i) & 1) scoreYW += (20 + point[i]);
	}





	// 確定石による評価（ Z ）****************************************
	// 角に石があるかを探索してそこから上下左右に連続する石の数を数える
	// 黒
	if ((bB >> 63) & 1) {	// 左上角から右に探索
		for (int i = 1; i < 8; i++) {
			if ((bB >> (63 - i)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	if ((bB >> 56) & 1) {	// 右上角から左に探索
		for (int i = 1; i < 8; i++) {
			if ((bB >> (56 + i)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	scoreZB += cnt;
	cnt = 0;		// 各辺の探索ごとにリセットする（各辺の最大石数が８だから）

	if ((bB >> 63) & 1) {	// 左上角から下に探索
		for (int i = 1; i < 8; i++) {
			if ((bB >> (63 - i * 8)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	if ((bB >> 7) & 1) {	// 左下角から上に探索
		for (int i = 1; i < 8; i++) {
			if ((bB >> (7 + i * 8)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	scoreZB += cnt;
	cnt = 0;		// 各辺の探索ごとにリセットする（各辺の最大石数が８だから）

	if ((bB >> 56) & 1) {	// 右上角から下に探索
		for (int i = 1; i < 8; i++) {
			if ((bB >> (56 - i * 8)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	if ((bB >> 0) & 1) {	// 右下角から上に探索
		for (int i = 1; i < 8; i++) {
			if ((bB >> (0 + i * 8)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	scoreZB += cnt;
	cnt = 0;		// 各辺の探索ごとにリセットする（各辺の最大石数が８だから）

	if ((bB >> 7) & 1) {	// 左下角から右に探索
		for (int i = 1; i < 8; i++) {
			if ((bB >> (7 - i)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	if ((bB >> 0) & 1) {	// 右下角から左に探索
		for (int i = 1; i < 8; i++) {
			if ((bB >> (0 + i)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	scoreZB += cnt;
	cnt = 0;		// 各辺の探索ごとにリセットする（各辺の最大石数が８だから）]



	//// 白		// 白の確定石の判定はいらなかったかも
	{
		if ((bW >> 63) & 1) {	// 左上角から右に探索
			for (int i = 1; i < 8; i++) {
				if ((bW >> (63 - i)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		if ((bW >> 56) & 1) {	// 右上角から左に探索
			for (int i = 1; i < 8; i++) {
				if ((bW >> (56 + i)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		scoreZW += cnt;
		cnt = 0;		// 各辺の探索ごとにリセットする（各辺の最大石数が８だから）

		if ((bW >> 63) & 1) {	// 左上角から下に探索
			for (int i = 1; i < 8; i++) {
				if ((bW >> (63 - i * 8)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		if ((bW >> 7) & 1) {	// 左下角から上に探索
			for (int i = 1; i < 8; i++) {
				if ((bW >> (7 + i * 8)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		scoreZW += cnt;
		cnt = 0;		// 各辺の探索ごとにリセットする（各辺の最大石数が８だから）

		if ((bW >> 56) & 1) {	// 右上角から下に探索
			for (int i = 1; i < 8; i++) {
				if ((bW >> (56 - i * 8)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		if ((bW >> 0) & 1) {	// 右下角から上に探索
			for (int i = 1; i < 8; i++) {
				if ((bW >> (0 + i * 8)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		scoreZW += cnt;
		cnt = 0;		// 各辺の探索ごとにリセットする（各辺の最大石数が８だから）

		if ((bW >> 7) & 1) {	// 左下角から右に探索
			for (int i = 1; i < 8; i++) {
				if ((bW >> (7 - i)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		if ((bW >> 0) & 1) {	// 右下角から左に探索
			for (int i = 1; i < 8; i++) {
				if ((bW >> (0 + i)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		scoreZW += cnt;
		cnt = 0;		// 各辺の探索ごとにリセットする（各辺の最大石数が８だから）
	}



	// 盤面の枚数による評価（A）
	if (endThink != 1) {		// ４４手目の局面までは自分の石の枚数が少ない盤面を高く評価する
		scoreAB = cntB;
		scoreAW = cntW;
	}




SKIP:



	if (index == 0) {
		//printf("X");
		result = end + ((scoreXB * 10) + (scoreXW * 15) + ((scoreYB - scoreYW) * -30) + (scoreZB - scoreZW) * 150) * -1;
	}
	else if (index == 1) {
		//printf("Y");
		result = end + ((scoreXB * 10) + (scoreXW * 15) + ((scoreYB - scoreYW) * -30) + (scoreZB - scoreZW) * 150) * -1;
	}


	return result;
}


void maxLevel_openness(int alpha, int beta, int depth, int* cntOpenness) {
	if (depth > depthNowOpenness) {
		return; //openness(tmpRevOut);
	}
	countStone();


	//if (cntB == 0 || cntW == 0) return evaluation_function(0);	// どちらか一方の石がゼロになったら評価関数を返す

	int cntOpennessBW2 = 0;
	int arrScore[30] = { 0 };	// depth = 1 の時の合法手のスコアを格納
	int cntArrScore = 0;		// arrScore[] のインデックス用

	int score_max = (int)(-INFINITY);  // 最大スコア（負の無限大で初期化)

	int xx = 0;		// 仮置きの座標 X	最も評価の高い手を最善手にするために用意する
	int yy = 0;		// 仮置きの座標 Y
	unsigned long long tmpRev = 0;


	for (int i = 63; i >= 0; i--) {

		search_selectable(0);		// 黒が打てる場所を探す
		if ((bSelectable >> i) & 1) {

			int score = 0;
			int numOpenness = 0;

			xx = abs((63 - i) % 8);		// 仮置きの座標 X
			yy = abs((63 - i) / 8);		// 仮置きの座標 Y

			put(xx, yy, 0);		// 黒の石を暫定で置く
			steps++;			// 手数をカウント
			int tmpCntPass = cntPass;
			cntPass = 0;		// パスのカウントをリセット
			reverse(0);			// 石をひっくり返す
			unsigned long long tmpPutPlace = putPlace;	 // 置いた石のマスを保存　局面ごとに保存しとかないと値が更新されてしまうから
			tmpRev = rev;			 // ひっくり返した石のマスを保存　理由は同上

			numOpenness = openness(tmpRev);
			*cntOpenness += numOpenness;	// 黒を置いたことによる開放度を足し合わせる


			// 暫定で黒石を置いたことで白石が角に置けるようになっていないかチェックする
			// 白石の合法手の中で角が含まれていないかを判定（角が含まれている場合その手は合法手ではなかったと判断する）
			search_selectable(1);
			countStone();
			if ((((bSelectable >> 63) & 1) || ((bSelectable >> 56) & 1) ||
				((bSelectable >> 7) & 1) || ((bSelectable >> 0) & 1)) && (endThink != 1)) {
				score = -500000 - cntB;
				//depthNum--;		// 手数をひとつ戻す
				goto SKIP;		// 次の候補手を探索する
			}

			// X のマスに置かないようにする
			if ((((tmpPutPlace >> 54) & 1) || ((tmpPutPlace >> 49) & 1) ||
				((tmpPutPlace >> 14) & 1) || ((tmpPutPlace >> 9) & 1)) && (endThink != 1)) {

				score = -99999 - cntB;
				goto SKIP;		// 次の候補手を探索する
			}

			// 開放度理論 ***********


			minLevel_openness(alpha, beta, depth + 1, cntOpenness);  // 次は相手のターン

			score = -1 * (*cntOpenness + openness_evaluation_function(0));


		SKIP:


			// reverse の反対の操作
			bB = bB ^ tmpRev;		// ひっくり返した石のマスを空にする
			bW = bW | tmpRev;		// ひっくり返されていた白のマスにビットを立てる
			bB = bB ^ tmpPutPlace;	// 暫定で置いた黒の石を取り除く
			steps--;				// 手数をひとつ戻す

			*cntOpenness -= numOpenness;	// 黒を置いたことによる開放度を元に戻す



			//// β値を超えると枝刈り		// カットはいらないかも
			//if (score >= beta) {
			//	return score;
			//}

				// α値を更新
			if (score > score_max) {
				score_max = score;
				alpha = max(alpha, score_max);

				if (depth == 1) {
					// 最善手を記録
					bestX = xx;
					bestY = yy;
				}

			}

			arrScore[cntArrScore++] = score;
		}
	}

	if (depth == 1) {
		for (int i = 0; i < cntArrScore; i++) printf("%6d  ", arrScore[i]);		// debug 用合法手のスコア表示
	}

	// 置く場所がない = パス
	if (bSelectable == 0) {
		cntPass++;

		//// お互いがパスの場合それ以降石が置かれることがないからその盤面を評価する
		//if (cntPass == 2) {
		//	cntPass = 0;
		//	return ;
		//}


		minLevel_openness(alpha, beta, depth + 1, cntOpenness);  // 次は相手のターン;

		int score = -1 * (*cntOpenness + openness_evaluation_function(0));


		//// β値を超えると枝刈り
		//if (score >= beta) {
		//	return score;
		//}
		// α値を更新
		if (score > score_max) {
			score_max = score;
			alpha = max(alpha, score_max);

			if (depth == 1) {
				// 最善手を記録
				bestX = xx;
				bestY = yy;
			}
		}
	}


	//// 置ける場所がなかったらの記述をする
	//if (score_max == (int)(-INFINITY)) return evaluation_function(0); // 評価関数の値を返す


	return;// score_max;
}

void minLevel_openness(int alpha, int beta, int depth, int* cntOpenness) {
	if (depth > depthNowOpenness) {
		return; // openness(tmpRevOut);
	}
	countStone();

	//if (cntB == 0 || cntW == 0) return evaluation_function(1);	// どちらか一方の石がゼロになったら評価関数を返す

	int arrScore[30] = { 0 };	// depth = 1 の時の合法手のスコアを格納
	int cntArrScore = 0;		// arrScore[] のインデックス用

	int score_min = (int)(INFINITY);  // 最大スコア（正の無限大で初期化)

	int xx = 0;		// 仮置きの座標 X	最も評価の高い手を最善手にするために用意する
	int yy = 0;		// 仮置きの座標 Y
	unsigned long long tmpRev = 0;


	// 指すことが可能な座標全てに対して探索を行う
	for (int i = 63; i >= 0; i--) {

		search_selectable(1);

		if ((bSelectable >> i) & 1) {


			int score = 0;
			int numOpenness = 0;

			xx = abs((63 - i) % 8);		// 仮置きの座標 X
			yy = abs((63 - i) / 8);		// 仮置きの座標 Y

			put(xx, yy, 1);		// 白の石を暫定で置く
			steps++;			// 手数をカウント
			int tmpCntPass = cntPass;
			cntPass = 0;		// パスのカウントをリセット
			reverse(1);		// 石をひっくり返す
			unsigned long long tmpPutPlace = putPlace;	 // 置いた石のマスを保存　局面ごとに保存しとかないと値が更新されてしまうから
			tmpRev = rev;			 // ひっくり返した石のマスを保存　理由は同上

			numOpenness = openness(tmpRev);
			*cntOpenness -= numOpenness;	// 白石を置いたことによる開放度を引く


			// 暫定で白石を置いたことで黒石が角に置けるようになっていないかチェックする
			// 黒石の合法手の中で角が含まれていないかを判定（角が含まれている場合その手は合法手ではないと判断する）
			search_selectable(0);
			countStone();
			if ((((bSelectable >> 63) & 1) || ((bSelectable >> 56) & 1) ||
				((bSelectable >> 7) & 1) || ((bSelectable >> 0) & 1)) && (endThink != 1)) {	// 左上角

				score = 500000 + cntW;		// ( + cntW ) が正しいのか要検討 **************************************************
				//depthNum--;
				goto SKIP;		// 次の候補手を探索する

			}

			// X のマスに置かないようにする
			if ((((tmpPutPlace >> 54) & 1) || ((tmpPutPlace >> 49) & 1) ||
				((tmpPutPlace >> 14) & 1) || ((tmpPutPlace >> 9) & 1)) && (endThink != 1)) {

				score = 99999 + cntW;		// ( + cntW ) が正しいのか要検討 **************************************************
				goto SKIP;		// 次の候補手を探索する
			}



			// 開放度が大きい方を選択したいので score に -1 をかける（minLevel_openness は score の小さい方を選ぶため）
			maxLevel_openness(alpha, beta, depth + 1, cntOpenness);  // 次は相手のターン

			score = -1 * (*cntOpenness + openness_evaluation_function(1));

		SKIP:


			// reverse の反対
			bW = bW ^ tmpRev;		// ひっくり返した石のマスを空にする
			bB = bB | tmpRev;		// ひっくり返されていた黒のマスにビットを立てる
			bW = bW ^ tmpPutPlace;	// 暫定で置いた白の石を取り除く	
			steps--;				// 手数をひとつ戻す
			*cntOpenness += numOpenness;	// 黒を置いたことによる開放度を元に戻す


			//// α値を超えると枝刈り	 カットはいらないかも
			//if (score <= alpha) {
			//	return score;
			//}

				// β値を更新
			if (score < score_min) {
				score_min = score;
				beta = min(beta, score_min);

				if (depth == 1) {
					// 最適な手を記録
					bestX = xx;
					bestY = yy;
				}

			}

			arrScore[cntArrScore++] = score;
		}
	}

	if (depth == 1) {
		for (int i = 0; i < cntArrScore; i++) printf("%6d  ", arrScore[i]);		// debug 用合法手のスコア表示
	}

	// 置く場所がない = パス
	if (bSelectable == 0) {
		cntPass++;

		//// お互いがパスの場合それ以降石が置かれることがないからその盤面を評価する
		//if (cntPass == 2) {
		//	cntPass = 0;
		//	return ;
		//}


		maxLevel_openness(alpha, beta, depth + 1, cntOpenness);  // 次は相手のターン;

		int score = -1 * (*cntOpenness + openness_evaluation_function(1));


		//// α値を超えると枝刈り
		//if (score <= alpha) {
		//	return score;
		//}
		// β値を更新
		if (score < score_min) {
			score_min = score;
			beta = min(beta, score_min);

			if (depth == 1) {
				// 最適な手を記録
				bestX = xx;
				bestY = yy;
			}
		}

	}


	//// 置ける場所がなかったらの記述をする
	//if (score_min == (int)(INFINITY)) return evaluation_function(1); // 評価関数の値を返す


	return;// score_min;
}

int openness(unsigned long long rev) {


	int cntOpenness = 0;
	unsigned long long revStone = 0b0;


	// ひっくり返された石の周囲の空きマスの数を数える
	for (int i = 63; i >= 0; i--) {
		if ((rev >> i) & 1) {		// ひっくり返された石の特定

			unsigned long long transferRev = 0b1000000000000000000000000000000000000000000000000000000000000000;	// 特定の位置に石を置くためのビット列　論理シフトして使う
			revStone = 0b0;

			int xx = abs((63 - i) % 8);		// ひっくり返された石の座標 X	
			int yy = abs((63 - i) / 8);		// ひっくり返された石の座標 Y


			transferRev = transferRev >> (yy * 8 + xx);		// ひっくり返された石の座標の位置にビットを立てる

			for (int k = 0; k < 8; k++) {
				openness_transfer(transferRev, k, &cntOpenness);	// ひっくり返された石の周囲の空きマスの数を数える
			}
		}
	}

	//printf("%d\n", cntOpenness);
	return cntOpenness;
}

void openness_transfer(unsigned long long  transferRev, int k, int* cntOpenness) {

	switch (k) {
	case 0: //右
		if ((((transferRev >> 1) > 0) && ((transferRev >> 1) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	case 1: //左
		if ((((transferRev << 1) > 0) && ((transferRev << 1) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	case 2: //上
		if ((((transferRev << 8) > 0) && ((transferRev << 8) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	case 3: //下
		if ((((transferRev >> 8) > 0) && ((transferRev >> 8) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	case 4: //右上
		if ((((transferRev << 7) > 0) && ((transferRev << 7) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	case 5: //左上
		if ((((transferRev << 9) > 0) && ((transferRev << 9) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	case 6: //右下
		if ((((transferRev >> 9) > 0) && ((transferRev >> 9) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	case 7: //左下
		if ((((transferRev >> 7) > 0) && ((transferRev >> 7) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	default:
		return;
	}
}

int openness_evaluation_function(int index) {

	int end = 0;

	if (endThink != 1) {
		// 黒
		if (((bB >> 63) & 1) && (edgeFlag[0] == 0)) {
			//printf("B");
			end += 150000;
		}
		if (((bB >> 56) & 1) && (edgeFlag[1] == 0)) {
			//printf("B");
			end += 150000;
		}
		if (((bB >> 7) & 1) && (edgeFlag[2] == 0)) {
			//printf("B");
			end += 150000;
		}
		if (((bB >> 0) & 1) && (edgeFlag[3] == 0)) {
			//printf("B");
			end += 150000;
		}
		// 白
		if (((bW >> 63) & 1) && (edgeFlag[0] == 0)) {
			//printf("W");
			end -= 150000;
		}
		if (((bW >> 56) & 1) && (edgeFlag[1] == 0)) {
			//printf("W");
			end -= 150000;
		}

		if (((bW >> 7) & 1) && (edgeFlag[2] == 0)) {
			//printf("W");
			end -= 150000;
		}
		if (((bW >> 0) & 1) && (edgeFlag[3] == 0)) {
			//printf("W");
			end -= 150000;
		}

		return -end;
	}
	return 0;
}


// 今までの評価関数に、周囲を石で囲われたマスにある自分の石を高く評価するコードを書く ***********************