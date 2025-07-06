#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include "define.h"
#include "prototype.h"
#include "global.h"



//*******************************************************
// �f�o�b�O�p�̃R���\�[���̏�������(���C�����[�v�O�ɌĂ�)
//*******************************************************
void initDebugConsole() {
	// �f�o�b�O�p�R���\�[����ʕ\���t���O���\�����Ȃ��ݒ�̏ꍇ�A�����I��
	//if (debugInfo.debugConsoleDisplayFlg == FALSE) {
	//	return;
	//}
	//�f�o�b�O�p�ɃR���\�[�����Ăяo��
	AllocConsole();
	(void)freopen("CONOUT$", "w", stdout);
	(void)freopen("CONIN$", "r", stdin);
	// �f�o�b�O�R���\�[�����A�N�e�B�u�E�B���h�E�ɂȂ�̂�
	// �Q�[���{�̂̃E�B���h�E���A�N�e�B�u�ɂ���
	SetForegroundWindow(GetMainWindowHandle());
}
//*******************************************************
// �f�o�b�O�p�̃R���\�[���̃N���A����
//*******************************************************
void clearDebugConsole() {
	//// �f�o�b�O�p�R���\�[����ʕ\���t���O���\�����Ȃ��ݒ�̏ꍇ�A�����I��
	//if (debugInfo.debugConsoleDisplayFlg == FALSE) {
	//	return;
	//}
	system("cls");
}
//*******************************************************
// �f�o�b�O�p�̃R���\�[���̏I������(���C�����[�v��ɌĂ�)
//*******************************************************
void endDebugConsole() {
	//// �f�o�b�O�p�R���\�[����ʕ\���t���O���\�����Ȃ��ݒ�̏ꍇ�A�����I��
	//if (debugInfo.debugConsoleDisplayFlg == FALSE) {
	//	return;
	//}
	//�R���\�[�����
	FreeConsole();
}






int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//*******************************************
	// �����ݒ�
	//*******************************************
	{
		//initDebugConsole();							// �R���\�[���̕\��
		ChangeWindowMode(TRUE);						// TRUE : �E�B���h�E  FALSE ; �t���X�N���[��
		SetGraphMode(500, 500, 32);					// �E�B���h�E�T�C�Y
		SetWindowSizeChangeEnableFlag(TRUE);		// �E�B���h�E�T�C�Y�̕ύX�������邩�ۂ�
		SetMainWindowText("Othello");				// �E�B���h�E��
		SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_DESKTOP);		// �t���X�N���[�����[�h���̉𑜓x���[�h��ݒ肷��

		SetBackgroundColor(241, 237, 238);			// ��ʂ̔w�i�F��ݒ�
		if (DxLib_Init() == -1) return -1;			// �G���[���N�����璼���ɏI��
		SetDrawScreen(DX_SCREEN_BACK);				// ����ʂɕ`�悷��
		SetMouseDispFlag(TRUE);						// �}�E�X����ʏ�ɕ\������
	}






	Handle_load();	// �e��n���h���̓ǂݍ���
	load_standerdMoves();


TITLE:
	initialize();	// �e��ϐ��̏�����
	title();


	// �Q�[�����C�����[�v
	while (ProcessMessage() != -1) {

		// �N���b�N������Ԃłق��̗v�f���������邱�Ƃ�h��
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0) clickFlag = 0;


		search_selectable(playerColor);// ���@���T��
		countStone();			// �΂𐔂���


		// ���x���S�̎������T���̐[���𐏎��ύX����
		if (randomFreq == 0) {
			// �Ֆʂ̐΂̐��ɂ���ĒT����[������
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

			// ����ڂ��犮�S��͂����݂邩�̐ݒ�
			if (steps >= 46) endThink = 1;
		}


		// ���[�h�̐؂�ւ�
		if (mode == 1) {
			if (turn == 0) select_player();		// �}�E�X�Ő΂�u��
			else select_player();				// �}�E�X�Ő΂�u��
		}
		else if (mode == 0) {
			if (turn == 0) select_player();		// �}�E�X�Ő΂�u��
			else select_com();
		}


		// ���܂��Ă���p�ɑΉ������t���O�𗧂Ă�
		if (((bB | bW) >> 63) & 1) edgeFlag[0] = 1;		// ����p
		if (((bB | bW) >> 56) & 1) edgeFlag[1] = 1;		// �E��p
		if (((bB | bW) >> 7) & 1) edgeFlag[2] = 1;		// �����p
		if (((bB | bW) >> 0) & 1) edgeFlag[3] = 1;		// �E���p



		draw_board();			// �Ֆʂ̕`��

		
		// ���j���[�{�^��
		int mx, my;		// �}�E�X�̍��W
		GetMousePoint(&mx, &my);
		// ���j���[�{�^���N���b�N
		if ((mx > 382 && mx < 490) && (my > 10 && my < 40)) {
			DrawBox(380, 0, 492, 50, 0x767B87, TRUE);
			DrawFormatStringToHandle(388, 14, 0x000000, FHandle[4], "���j���[");
			DrawFormatStringToHandle(385, 12, 0xFFFFFF, FHandle[4], "���j���[");
			if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
				goto TITLE;
			}
		}


		ScreenFlip();


		// �I������
		countStone();			// �΂𐔂���
		if (cntB + cntW == 64 || cntPass == 2 || cntB == 0 || cntW == 0) {
			WaitTimer(2000);
			ending();
			goto TITLE;
		}


		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			DrawString(400, 30, "�I��", 0x000000);
			WaitKey();				// �L�[���͑҂�
			endDebugConsole();		// �R���\�[���̊J��
			DxLib_End();			// �c�w���C�u�����g�p�̏I������
			return 0;				// �\�t�g�̏I�� 
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
	FHandle[1] = CreateFontToHandle("UD �f�W�^�� ���ȏ��� NK-B", 40, -1, DX_FONTTYPE_ANTIALIASING);
	FHandle[2] = CreateFontToHandle("UD �f�W�^�� ���ȏ��� NK-B", 35, -1, DX_FONTTYPE_ANTIALIASING);
	FHandle[3] = CreateFontToHandle("UD �f�W�^�� ���ȏ��� NK-B", 60, -1, DX_FONTTYPE_ANTIALIASING);
	FHandle[4] = CreateFontToHandle("UD �f�W�^�� ���ȏ��� NK-B", 30, -1, DX_FONTTYPE_ANTIALIASING);
	FHandle[5] = CreateFontToHandle("UD �f�W�^�� ���ȏ��� NK-B", 20, -1, DX_FONTTYPE_ANTIALIASING);
	FHandle[6] = CreateFontToHandle("UD �f�W�^�� ���ȏ��� NK-B", 42, -1, DX_FONTTYPE_ANTIALIASING);
}

void title() {



	while (ProcessMessage() != -1) {
		// �N���b�N������Ԃłق��̗v�f���������邱�Ƃ�h��
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0) clickFlag = 0;
		int mx, my;		// �}�E�X�̍��W
		GetMousePoint(&mx, &my);
		
		DrawGraph(0, 0, GHandle[12], TRUE);		// �w�i

		// ��Ԃ̕`��
		{
			// ���̉���
			if (turn == 0) DrawBox(325, 238, 404, 247, 0xB03307, TRUE);
			if (turn == 1) DrawBox(325, 308, 404, 317, 0xB03307, TRUE);

			// ���̔w�i
			if (randomFreq == 3) DrawBox(90, 190, 265, 228, 0xB03307, TRUE);
			if (randomFreq == 5) DrawBox(90, 231, 265, 268, 0xB03307, TRUE);
			if (randomFreq == 8) DrawBox(90, 271, 265, 308, 0xB03307, TRUE);
			if (randomFreq == 0) DrawBox(90, 311, 265, 348, 0xB03307, TRUE);

			// mode �̔w�i
			if (mode == 0) DrawBox(90, 350, 265, 388, 0x00A2AB, TRUE);
			if (mode == 1) DrawBox(90, 390, 265, 428, 0x00A2AB, TRUE);

		}

		DrawFormatStringToHandle(99, 94, 0x000000, FHandle[3], "�` �I�Z�� �`");
		DrawFormatStringToHandle(95, 90, 0xFFFFFF, FHandle[3], "�` �I�Z�� �`");
		DrawFormatStringToHandle(331, 210, 0xEDEDED, FHandle[2], "���");
		DrawFormatStringToHandle(330, 210, 0x000000, FHandle[2], "���");
		DrawFormatStringToHandle(333, 280, 0x000000, FHandle[2], "���");
		DrawFormatStringToHandle(330, 280, 0xFFFFFF, FHandle[2], "���");

		DrawBox(90, 228, 265, 232, 0x000000, TRUE);		// ����
		DrawBox(90, 268, 265, 272, 0x000000, TRUE);		// ����
		DrawBox(90, 308, 265, 312, 0x000000, TRUE);		// ����
		DrawBox(90, 186, 265, 190, 0x000000, TRUE);		// ����
		DrawBox(90, 347, 265, 351, 0x000000, TRUE);		// ����

		DrawFormatStringToHandle(160, 195, 0xE6E607, FHandle[2], "��");
		DrawFormatStringToHandle(142, 235, 0xE6E607, FHandle[2], "����");
		DrawFormatStringToHandle(125, 275, 0xE6E607, FHandle[2], "������");
		DrawFormatStringToHandle(108, 315, 0xE6E607, FHandle[2], "��������");

		DrawFormatStringToHandle(303, 393, 0x000000, FHandle[2], "�r�s�`�q�s");
		DrawFormatStringToHandle(300, 390, 0xFFFFFF, FHandle[2], "�r�s�`�q�s");

		DrawFormatStringToHandle(94, 360, 0xFFFFFF, FHandle[5], "player vs com");
		DrawFormatStringToHandle(94, 400, 0xFFFFFF, FHandle[5], "player vs player");
		DrawBox(90, 388, 265, 392, 0x000000, TRUE);		// ����
		DrawBox(90, 428, 265, 432, 0x000000, TRUE);		// ����



		// �N���b�N����
		{
			// ���
			if ((mx > 325 && mx < 400) && (my > 205 && my < 240)) {
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
					clickFlag = 1;	// �N���b�N�t���O�𗧂Ă�
					turn = 0;
				}
			}
			// ���
			if ((mx > 325 && mx < 400) && (my > 275 && my < 310) && (mode != 1)) {
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
					clickFlag = 1;	// �N���b�N�t���O�𗧂Ă�
					turn = 1;
				}
			}
			// ��
			if ((mx > 90 && mx < 265) && (my > 140 && my < 226) && (mode != 1)) {
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
					clickFlag = 1;
					depthNow = 4;
					randomFreq = 3;
					playStanderdMovesFlag = 0;
				}
			}
			// ����
			if ((mx > 90 && mx < 265) && (my > 233 && my < 266) && (mode != 1)) {
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
					clickFlag = 1;
					depthNow = 6;
					randomFreq = 5;
					playStanderdMovesFlag = 0;
				}
			}
			// ������
			if ((mx > 90 && mx < 265) && (my > 274 && my < 306) && (mode != 1)) {
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
					clickFlag = 1;
					depthNow = 8;
					randomFreq = 8;
					playStanderdMovesFlag = 0;
				}
			}
			// ��������
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
			// �r�s�`�q�s
			if ((mx > 300 && mx < 425) && (my > 390 && my < 422)) {
				DrawBox(296, 385, 429, 426, 0xB03307, TRUE);
				DrawFormatStringToHandle(303, 393, 0x000000, FHandle[2], "�r�s�`�q�s");
				DrawFormatStringToHandle(300, 390, 0xFFFFFF, FHandle[2], "�r�s�`�q�s");
				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {

					// ���̐ݒ�
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
			DrawString(400, 30, "�I��", 0x000000);
			WaitKey();				// �L�[���͑҂�
			endDebugConsole();		// �R���\�[���̊J��
			DxLib_End();			// �c�w���C�u�����g�p�̏I������
			return ;				// �\�t�g�̏I�� 
		}
	}
}

void load_standerdMoves() {

	FILE* fp;
	int lineCount = 0;

	// �t�@�C�����J��
	fp = fopen("standerdmoves3.txt", "r");
	if (fp == NULL) {
		printf("�t�@�C�����J���܂���ł���");
		return;
	}

	// �t�@�C�������s���ǂݎ��
	while (fgets(standerdMoves[lineCount], 128, fp) != NULL) {
		// ���s�������폜
		standerdMoves[lineCount][strcspn(standerdMoves[lineCount], "\n")] = '\0';

		for (int i = 0; standerdMoves[lineCount][i] != '\0'; i++) {
			standerdMoves[lineCount][i] = tolower((unsigned char)standerdMoves[lineCount][i]); // 1�������������ɕϊ�
		}

		char* tmp = standerdMoves[lineCount];
		int len = (int)strlen(tmp);	// ��΂̒������擾
		lineCount++;


		// f5 �n�܂�̒�΂�4�����ɑΉ�������
		for (int i = 1; i <= 3; i++) {
			// ���̒�΂��R�s�[
			strcpy(standerdMoves[lineCount], tmp);

			// ��΂�ϊ�
			for (int j = 0; j < len; j += 2) {
				int x = standerdMoves[lineCount][j] - 'a';   // �A���t�@�x�b�g�����W�����ɕϊ�
				int y = standerdMoves[lineCount][j + 1] - '1'; // ���l�ϊ�


				switch (i) {
				case 1:
					// 90�x��]����x���𔽓]
					standerdMoves[lineCount][j + 1] = x + '1';   // ���l�𕶎��ɖ߂�
					standerdMoves[lineCount][j] = (7 - (8 - y - 1)) + 'a'; // ���l�𕶎��ɖ߂�
					break;
				case 2:
					// 90�x��]����y���𔽓]
					standerdMoves[lineCount][j + 1] = 7 - x + '1';   // ���l�𕶎��ɖ߂�
					standerdMoves[lineCount][j] = (8 - y - 1) + 'a'; // ���l�𕶎��ɖ߂�
					break;
				case 3:
					// x����y���𔽓]
					standerdMoves[lineCount][j + 1] = (8 - y - 1) + '1';   // ���l�𕶎��ɖ߂�
					standerdMoves[lineCount][j] = 7 - x + 'a'; // ���l�𕶎��ɖ߂�
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

	// �r�b�g�{�[�h�̏�����
	bB = 0b0000000000000000000000000000100000010000000000000000000000000000;	// bitboard Black
	bW = 0b0000000000000000000000000001000000001000000000000000000000000000;	// bitboard White
	bN = 0b0000000000000000000000000000000000000000000000000000000000000000;	// bitboard Now  ���݂̔Ֆ�

	mode = 0;
	cntPass = 0;					// �p�X�̃J�E���g��������
	turn = 0;						// ���j���[��ʂ��쐬������폜����
	steps = 1;						// �萔�J�E���g�����Z�b�g
	endThink = 0;					// �S�S��ȍ~���𔻒�p
	playStanderdMovesFlag = 1;		// ��΂𔲂������̃t���O
	randomFreq = 3;					// �����_���Ɏw���m��
	putPlace = 0;

	// ���܂��Ă���p��\���t���O��������
	for (int i = 0; i < 4; i++) edgeFlag[i] = 0;


	countStone();	// �΂̐����������@�b�n�l�����̎���ɏ��������Ă��Ȃ��� maxLevel() �����܂������Ȃ�


	cntValid = 0;
	load_standerdMoves();		// ��΃t�@�C����ǂݍ��݂Ȃ���
}

void ending() {

	if (cntB > cntW) DrawGraph(0, 0, GHandle[4], TRUE);			// �Ղ̕`��  ������
	else if (cntB < cntW) DrawGraph(0, 0, GHandle[5], TRUE);		// �Ղ̕`��@������
	else if (cntB == cntW) DrawGraph(0, 0, GHandle[11], TRUE);	// �Ղ̕`��@��������

	DrawFormatStringToHandle(208, 165, 0x000000, FHandle[1], "%2d", cntB);
	DrawFormatStringToHandle(208, 252, 0xFFFFFF, FHandle[1], "%2d", cntW);

	DrawBox(100,300,400 , 400, 0x009040, TRUE);
	DrawBox(100,70,400 , 150, 0x009040, TRUE);


	DrawFormatStringToHandle(209, 86, 0x000000, FHandle[6], "����");
	DrawFormatStringToHandle(206, 84, 0xDFE42E, FHandle[6], "����");
	DrawFormatStringToHandle(130, 352, 0x000000, FHandle[6], "�o�������� �d��������");
	DrawFormatStringToHandle(127, 350, 0xFFFFFF, FHandle[6], "�o�������� �d��������");

	ScreenFlip();

	// �G���^�[�������̓G�X�P�[�v�L�[�Ń��j���[�ɖ߂�
	while (ProcessMessage() != -1) {
		if (CheckHitKey(KEY_INPUT_ESCAPE) || CheckHitKey(KEY_INPUT_RETURN)) {
			break;
		}
	}
}

void draw_board() {

	DrawGraph(0, 0, GHandle[3], TRUE);	// �Ղ̕`��


	// ���@��̕`��
	for (int i = 63; i >= 0; i--) {
		if (((bSelectable >> i) & 1) == 1) {
			int xx = abs((63 - i) % 8);
			int yy = abs((63 - i) / 8);
			DrawExtendGraph((baseX + selectableSize) + xx * masuSize, (baseY + selectableSize) + yy * masuSize, ((baseX - selectableSize) + xx * masuSize) + masuSize, ((baseY - selectableSize) + yy * masuSize) + masuSize, GHandle[2], TRUE);
		}
	}

	// ���΂̕`��
	for (int i = 63; i >= 0; i--) {
		if (((bB >> i) & 1) == 1) {
			int xx = abs((63 - i) % 8);
			int yy = abs((63 - i) / 8);
			DrawExtendGraph((baseX + stoneSize) + xx * masuSize, (baseY + stoneSize) + yy * masuSize, ((baseX - stoneSize) + xx * masuSize) + masuSize, ((baseY - stoneSize) + yy * masuSize) + masuSize, GHandle[0], TRUE);
		}
	}
	// ���΂̕`��
	for (int i = 63; i >= 0; i--) {
		if (((bW >> i) & 1) == 1) {
			int xx = abs((63 - i) % 8);
			int yy = abs((63 - i) / 8);
			DrawExtendGraph((baseX + stoneSize) + xx * masuSize, (baseY + stoneSize) + yy * masuSize, ((baseX - stoneSize) + xx * masuSize) + masuSize, ((baseY - stoneSize) + yy * masuSize) + masuSize, GHandle[1], TRUE);
		}
	}
	// ���O��̈�
	for (int i = 63; i >= 0; i--) {
		if (((putPlace >> i) & 1) == 1) {
			int xx = abs((63 - i) % 8);
			int yy = abs((63 - i) / 8);
			DrawExtendGraph((baseX + justBeforeSize) + xx * masuSize, (baseY + justBeforeSize) + yy * masuSize, ((baseX - justBeforeSize) + xx * masuSize) + masuSize, ((baseY - justBeforeSize) + yy * masuSize) + masuSize, GHandle[13], TRUE);
		}
	}


	// �p�X�{�^���̕`��i���@�肪�Ȃ��ꍇ�j
	if (bSelectable == 0) DrawGraph(0, 0, GHandle[10], TRUE);


	// �����Ɋe�F�̐΂̐���`��
	// �P���ƂQ���ŕʂ̒���������	
	if (cntB < 10) DrawFormatStringToHandle(71, 460, 0xFFFFFF, FHandle[0], "%d", cntB);
	else if (cntB >= 10) DrawFormatStringToHandle(63, 460, 0xFFFFFF, FHandle[0], "%2d", cntB);
	if (cntW < 10) DrawFormatStringToHandle(133, 460, 0x000000, FHandle[0], "%d", cntW);
	else if (cntW >= 10) DrawFormatStringToHandle(125, 460, 0x000000, FHandle[0], "%2d", cntW);

	// ��Ԃ�\��
	if (playerColor == 0 && turn == 0) DrawGraph(0, 0, GHandle[8], TRUE);
	else if (playerColor == 1 && turn == 0) DrawGraph(0, 0, GHandle[9], TRUE);
	// ��Ԃ�\��
	if (playerColor == 0 && turn == 1) DrawGraph(0, 0, GHandle[9], TRUE);
	else if (playerColor == 1 && turn == 1) DrawGraph(0, 0, GHandle[8], TRUE);

	// ���݂̎萔��\��
	DrawFormatStringToHandle(327, 460, 0xDCDCDC, FHandle[2], "%2d���", (cntB + cntW) - 3);


	// �I�����Ă��郌�x����`��
	if(randomFreq == 3) DrawFormatStringToHandle(22, 13, 0xE6E607, FHandle[4], "��");
	if(randomFreq == 5) DrawFormatStringToHandle(22, 13, 0xE6E607, FHandle[4], "����");
	if(randomFreq == 8) DrawFormatStringToHandle(22, 13, 0xE6E607, FHandle[4], "������");
	if(randomFreq == 0) DrawFormatStringToHandle(22, 13, 0xE6E607, FHandle[4], "��������");


	// ���j���[�{�^��
	DrawFormatStringToHandle(388, 14, 0x000000, FHandle[4], "���j���[");
	DrawFormatStringToHandle(385, 12, 0xFFFFFF, FHandle[4], "���j���[");
}

void printBits(long long n) {
	for (int i = 63; i >= 0; i--) {
		printf("%lld ", (n >> i) & 1);
		if (i % 8 == 0) printf("\n");  // 8�r�b�g���Ƃɉ��s������
	}
	printf("\n");
}

void put(int x, int y, int color) {
	// turn :: 0 = �� , 1 = ��

	unsigned long long d = 0b1000000000000000000000000000000000000000000000000000000000000000;	// ����̈ʒu�ɐ΂�u�����߂̃r�b�g��@�_���V�t�g���Ďg��

	unsigned long long n = y * 8 + x;	// �Y���ӏ��Ƀr�b�g�𗧂Ă�

	putPlace = d >> n;	// �΂�u�����ꏊ�Ƀr�b�g���������r�b�g��


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
	// turn :: 0 = �� , 1 = ��

	unsigned long long d = 0b1000000000000000000000000000000000000000000000000000000000000000;	// ����̈ʒu�ɐ΂�u�����߂̃r�b�g��@�_���V�t�g���Ďg��

	unsigned long long n = y * 8 + x;	// �Y���ӏ��Ƀr�b�g�𗧂Ă�

	putPlace = d >> n;	// �΂�u�����ꏊ�Ƀr�b�g���������r�b�g��


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

	unsigned long long bC1 = 0;		// Bitboard Color1	��Ԃ̐΂̐F
	unsigned long long bC2 = 0;		// Bitboard Color2
	unsigned long long bTC1 = 0;		// Bitboard Color2
	unsigned long long bTC2 = 0;		// Bitboard Color2
	unsigned long long bTmp = 0;	// �ꎞ�ϐ�
	bSelectable = 0;	// ���@������Z�b�g

	if (color == 0) {
		bC1 = bB;
		bC2 = bW;
	}
	else if (color == 1) {
		bC1 = bW;
		bC2 = bB;
	}


	// �e�����̍��@���T��
	{
		// �E
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			// horizontalMask �̂O�̕����� bC1 �̃r�b�g�������������ǁA���� bC1 ��ύX����ƌ�̃R�[�h�Ŏg���Ȃ��Ȃ�̂ŁAbTC1 �Ƃ����ꎞ�ϐ��ɑޔ�������
			bTC1 = (horizontalMask & bC2) & (bTC1 >> 1);
			bTmp |= (horizontalMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp >> 1) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;

		// ��
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			bTC1 = (horizontalMask & bC2) & (bTC1 << 1);
			bTmp |= (horizontalMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp << 1) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;

		// ��
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			bTC1 = (verticalMask & bC2) & (bTC1 << 8);
			bTmp |= (verticalMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp << 8) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;

		// ��
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			bTC1 = (verticalMask & bC2) & (bTC1 >> 8);
			bTmp |= (verticalMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp >> 8) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;

		// �E��
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			bTC1 = (allsideMask & bC2) & (bTC1 << 7);
			bTmp |= (allsideMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp << 7) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;

		// ����
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			bTC1 = (allsideMask & bC2) & (bTC1 << 9);
			bTmp |= (allsideMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp << 9) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;

		// �E��
		bTC1 = bC1;
		for (int i = 1; i <= 7; i++) {
			bTC1 = (allsideMask & bC2) & (bTC1 >> 9);
			bTmp |= (allsideMask & bC2) & bTC1;
		}
		bSelectable |= (bTmp >> 9) & ~(bB | bW);
		bTmp = 0;
		bTC1 = 0;

		// ����
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

	int mx, my;		// �}�E�X�̍��W

	GetMousePoint(&mx, &my);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {


			if ((mx > baseX + (masuSize * j) && mx < baseX + (masuSize * j) + masuSize) &&
				(my > baseY + (masuSize * i) && my < baseY + (masuSize * i) + masuSize)) {

				if (clickFlag == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {

					clickFlag = 1;	// �N���b�N�t���O�𗧂Ă�

					// �N���b�N�����}�X�����@�肩���肷��B���@��̏ꍇ�̂ݐ΂�u��
					for (int k = 63; k >= 0; k--) {

						if ((bSelectable >> k) & 1) {

							// x���W��y���W
							int xx = abs((63 - k) % 8);
							int yy = abs((63 - k) / 8);

							if (xx == j && yy == i) {
								put(j, i, playerColor);		// �΂�u��
								reverse(playerColor);			// �΂��Ђ�����Ԃ�
								turn = (turn + 1) % 2;	// ��Ԃ����ւ���
								cntPass = 0;	// �p�X�̃J�E���g��������
								steps++;		// �萔���J�E���g
								if (mode == 1) playerColor = (playerColor + 1) % 2;		// player vs player �Ŏ�Ԃ����ւ���
							}
						}
					}
				}
			}
		}
	}

	// �p�X�{�^�����N���b�N
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
	if (bSelectable == 0) {		// �u���܂����Ȃ��ꍇ�p�X��I��
		turn = (turn + 1) % 2;
		cntPass++;
		WaitTimer(1000);
		return;
	}
	draw_board();
	ScreenFlip();
	
	if(randomFreq > 0) WaitTimer(500);		// com �̃��x�����Ⴂ�Ƃ��ɐݒ肷��i��ʂ�������̂�h�����߁j



	// �����_���Ɏw�����v�l���[�`����ʂ��������Ŕ���
	int random = GetRand(randomFreq - 1);


	if (random == 0 && randomFreq != 0) {
		// �����_���Ɏw��
		printf("�����_��\n");

		int ranX = 0;			// �����_���Ɏw���}�X�w
		int ranY = 0;			// �����_���Ɏw���}�X�x
		int ranPutPlace = 0;	// �����_���ɐ΂�u�����}�X������
		unsigned long long d = 0b1000000000000000000000000000000000000000000000000000000000000000;	// ����̈ʒu�ɐ΂�u�����߂̃r�b�g��@�_���V�t�g���Ďg��
		unsigned long long n = 0;	// �Y���ӏ��Ƀr�b�g�𗧂Ă�

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
		// �~�j�}�b�N�X�@
		printf("�~�j�}�b�N�X\n");

		depthNum = 0;	// ���݂̒T���������Z�b�g

		// ���x���S�̎�������΂��w��
		if (playStanderdMovesFlag == 1) playStanderdMovesFlag = playStanderdMoves();		// ��x��΂𔲂����炻�̐��΂ɍ������Ă���΂��w���Ȃ��i�듮��h�~�j

		if (playStanderdMovesFlag == 0) {
			if (steps <= 20) {		// �R�O��ڂ܂ł͊J���x���_�̃~�j�}�b�N�X�@���g��
				int dumy = 0;	// �ŏ������_�~�[�ŃA�h���X��n��
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
	printf("%8d  depthNow = %2d  %2d���\n", depthNum, depthNow, steps);
	turn = (turn + 1) % 2;	// ��Ԃ����ւ���
	steps++;
	cntPass = 0;	// �p�X�̃J�E���g��������
}

void countStone() {

	// �΂̃J�E���g��������
	cntB = 0;
	cntW = 0;

	// ���Ɣ��̐΂̐��𐔂���
	for (int i = 0; i < 64; i++) {
		if ((bB >> i) & 1) cntB++;
		if ((bW >> i) & 1) cntW++;
	}
}

void reverse(int color) {

	unsigned long long bC1 = 0;  // ��Ԃ̐΂̐F
	unsigned long long bC2 = 0;  // ����̐΂̐F

	// ��Ԃɉ������{�[�h��ݒ�
	if (color == 0) {
		bC1 = bB;
		bC2 = bW;
	}
	else if (color == 1) {
		bC1 = bW;
		bC2 = bB;
	}

	rev = 0;  // ���]����ʒu���i�[����r�b�g�{�[�h

	// �W�������`�F�b�N����
	for (int i = 0; i < 8; i++) {

		unsigned long long rev_ = 0;  // �e�����Ŕ��]����ꏊ
		unsigned long long mask = transfer(putPlace, i);	  // mask : �Ђ�����Ԃ���΂̏ꏊ
		unsigned long long maskPrev = 0;  // ��񓮂����O�� mask ��ۑ�

		// �΂�u�����ꏊ����C�ӂ̕����ɓ������A����̐΂������Ă��邩���m�F
		while ((mask != 0) && ((mask & bC2) != 0)) {
			rev_ |= mask;			   // ����̐΂𔽓]���ɒǉ�
			maskPrev = mask;
			mask = transfer(mask, i);  // ���̏ꏊ�𒲂ׂ�
		}

		// �[�܂ōs���� mask �͔ԕ��ɂ���Ēl���O�ɂȂ��Ă��܂��̂ŁA
		// ���̈�O�̒l�� maskPrev �ɑޔ������ĔC�ӂ̕����ɂP�}�X������ bC1 �Ƃ̘_���ς��Ƃ�
		switch (i) {
		case 0: //�E
			maskPrev = (maskPrev >> 1);
			break;
		case 1: //��
			maskPrev = (maskPrev << 1);
			break;
		case 2: //��
			maskPrev = (maskPrev << 8);
			break;
		case 3: //��
			maskPrev = (maskPrev >> 8);
			break;
		case 4: //�E��
			maskPrev = (maskPrev << 7);
			break;
		case 5: //����
			maskPrev = (maskPrev << 9);
			break;
		case 6: //�E��
			maskPrev = (maskPrev >> 9);
			break;
		case 7: //����
			maskPrev = (maskPrev >> 7);
			break;
		}

		// �Ō�Ɏ����̐΂�����΁A���̕����Ŕ��]�ł���
		if ((maskPrev & bC1) != 0) {
			rev |= rev_;  // ���]����ꏊ���m��
		}
	}



	// ���]����
	bC1 |= (putPlace | rev);  // �����̐΂Ɣ��]����ꏊ�𔽓]
	bC2 ^= rev;				  // ����̐΂𔽓]

	// ���]��A�O���[�o���{�[�h�ɍX�V�𔽉f������
	if (color == 0) {
		bB = bC1;  // ���̃{�[�h�ɔ��f�i��Ԃ����̏ꍇ�j
		bW = bC2;  // ���̃{�[�h�ɔ��f�i��Ԃ����̏ꍇ�j
	}
	else if (color == 1) {
		bB = bC2;  // ���̃{�[�h�ɔ��f�i��Ԃ����̏ꍇ�j
		bW = bC1;  // ���̃{�[�h�ɔ��f�i��Ԃ����̏ꍇ�j
	}
}

unsigned long long transfer(unsigned long long  putPlace, int k) {

	switch (k) {
	case 0: //�E
		return (putPlace >> 1) & horizontalMask;
	case 1: //��
		return (putPlace << 1) & horizontalMask;
	case 2: //��
		return (putPlace << 8) & verticalMask;
	case 3: //��
		return (putPlace >> 8) & verticalMask;
	case 4: //�E��
		return (putPlace << 7) & allsideMask;
	case 5: //����
		return (putPlace << 9) & allsideMask;
	case 6: //�E��
		return (putPlace >> 9) & allsideMask;
	case 7: //����
		return (putPlace >> 7) & allsideMask;
	default:
		return 0;
	}
}

int playStanderdMoves() {

	char nowPut[10] = { "" };
	int xx = 0;
	int yy = 0;
	char cx[5];			// ��΂̃A���t�@�x�b�g�������i�[
	char dumy[128];		// ��΂̍ŏ��̓񕶎��ȍ~�̕������֋X��i�[
	int cntNewValid = 0;	// �L���Ȓ�΂̐����J�E���g
	int validFlag = 0;	// �L���Ȓ�΂̐����J�E���g


	// com �����̎��̏�������߂�
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

			sprintf(nowPut, "%c%d", 'a' + bestX, bestY + 1);		// �w������΂̌`���ɕϊ�


			// �w����ɑΉ������΂� valid[] �Ɋi�[����i�擪����Q�����폜����j
			for (int i = 0; i < MAXSTANDERDMOVES; i++) {
				if (strncmp(nowPut, standerdMoves[i], 2) == 0) {
					strcpy(valid[cntValid++], standerdMoves[i] + 2);
					printf("valid[%d] = %s\n", cntValid - 1, valid[cntValid - 1]);
				}
			}

			return 1;
		}
	}



	// �v���C���[���u�����΂̍��W�� 'a3' �̂悤�Ȍ^���ɕϊ�
	for (int i = 63; i >= 0; i--) {
		if ((putPlace >> i) & 1) {
			xx = abs((63 - i) % 8);		// �v���C���[���u�����΂̍��W X
			yy = abs((63 - i) / 8);		// �v���C���[���u�����΂̍��W Y
			sprintf(nowPut, "%c%d", 'a' + xx, yy + 1);		// �w������΂̌`���ɕϊ�
		}
	}

	// ���肾���S��΂��猟������
	if (steps == 1 || steps == 2) {
		cntValid = 0;
		// �w����ɑΉ������΂� valid[] �Ɋi�[����i�擪����Q�����폜����j
		for (int i = 0;i < MAXSTANDERDMOVES;i++) {
			if (strncmp(nowPut, standerdMoves[i], 2) == 0) {
				strcpy(valid[cntValid++], standerdMoves[i] + 2);
				validFlag = 1;
			}
		}
	}
	else {		// �Q��ڈȍ~�͗L���Ȓ�΂̔z�񂩂猟������
		cntNewValid = 0;
		for (int i = 0; i < cntValid; i++) {
			if (strncmp(nowPut, valid[i], 2) == 0) {
				strcpy(valid[cntNewValid++], valid[i] + 2);

				if (valid[cntNewValid - 1][0] == '\0') return 0;
				validFlag = 1;
			}
		}
		cntValid = cntNewValid;		// �L���Ȓ�΂̐����X�V
	}


	// ���ݗL���Ȓ�΂̒����玟�̎w�����I������
	if (validFlag == 1) {
		validFlag = 0;

		int ranNum = GetRand(cntValid - 1);

		// �L���Ȓ�΂̒����炩�烉���_���ɑI���@
		(void)sscanf(valid[ranNum], "%c%d%s", cx, &yy, dumy);
		xx = cx[0] - 97;	// ASCII�R�[�h�\�� abcdefgh �𐔎��ɕϊ�����
		yy -= 1;

		bestX = xx;		// �őP����X�V
		bestY = yy;


		sprintf(nowPut, "%c%d", 'a' + xx, yy + 1);		// �w������΂̌`���ɕϊ�


		// �L���Ȓ�΂��X�V
		cntNewValid = 0;
		for (int i = 0; i < cntValid; i++) {
			if (strncmp(nowPut, valid[i], 2) == 0) {
				strcpy(valid[cntNewValid++], valid[i] + 2);
				printf("valid[%d] = %s\n", cntNewValid - 1, valid[cntNewValid - 1]);
			}
		}
		cntValid = cntNewValid;	// �L���Ȓ�΂̐����X�V

		return 1;
	}

	return 0;
}


int maxLevel(int alpha, int beta, int depth) {
	if (depth >= depthNow) {
		//printf("koko");
		return evaluation_function(0); // �]���֐��̒l��Ԃ�
	}
	countStone();
	if (cntB + cntW == 64) return evaluation_function(0);		// �Ֆʂ��S�����܂�����]���֐��̒l��Ԃ�
	if (cntB == 0 || cntW == 0) return evaluation_function(0);	// �ǂ��炩����̐΂��[���ɂȂ�����]���֐���Ԃ�

	int arrScore[30] = { 0 };	// depth = 1 �̎��̍��@��̃X�R�A���i�[
	int cntArrScore = 0;		// arrScore[] �̃C���f�b�N�X�p

	int score_max = (int)(-INFINITY);  // �ő�X�R�A�i���̖�����ŏ�����)

	int xx = 0;		// ���u���̍��W X	�ł��]���̍�������őP��ɂ��邽�߂ɗp�ӂ���
	int yy = 0;		// ���u���̍��W Y



	for (int i = 63; i >= 0; i--) {
		search_selectable(0);		// �����łĂ�ꏊ��T��
		if ((bSelectable >> i) & 1) {

			if (++depthNum > DEPTHNUM) {
				//printf("koko");
				return evaluation_function(0);		//  ���炩���ߌ��߂��ǖʐ��ɒB������]���֐��̒l��Ԃ�
			}

			int score = 0;
			xx = abs((63 - i) % 8);		// ���u���̍��W X
			yy = abs((63 - i) / 8);		// ���u���̍��W Y

			put(xx, yy, 0);		// ���̐΂��b��Œu��
			steps++;			// �萔���J�E���g
			int tmpCntPass = cntPass;
			cntPass = 0;		// �p�X�̃J�E���g�����Z�b�g
			reverse(0);			// �΂��Ђ�����Ԃ�
			unsigned long long tmpPutPlace = putPlace;	 // �u�����΂̃}�X��ۑ��@�ǖʂ��Ƃɕۑ����Ƃ��Ȃ��ƒl���X�V����Ă��܂�����
			unsigned long long tmpRev = rev;			 // �Ђ�����Ԃ����΂̃}�X��ۑ��@���R�͓���


			// �J���x���_ ***********



			// �b��ō��΂�u�������ƂŔ��΂��p�ɒu����悤�ɂȂ��Ă��Ȃ����`�F�b�N����
			// ���΂̍��@��̒��Ŋp���܂܂�Ă��Ȃ����𔻒�i�p���܂܂�Ă���ꍇ���̎�͍��@��ł͂Ȃ������Ɣ��f����j
			search_selectable(1);
			countStone();
			if ((((bSelectable >> 63) & 1) || ((bSelectable >> 56) & 1) ||
				((bSelectable >> 7) & 1) || ((bSelectable >> 0) & 1)) && (endThink != 1)) {

				//// reverse �̔��΂̑���	
				score = -500000 - cntB;						// �v���X�}�C�i�X�ς��Ă݂�
				depthNum--;		// �萔���ЂƂ߂�
				goto SKIP;		// ���̌����T������
			}

			// X �̃}�X�ɒu���Ȃ��悤�ɂ���
			if ((((tmpPutPlace >> 54) & 1) || ((tmpPutPlace >> 49) & 1) ||
				((tmpPutPlace >> 14) & 1) || ((tmpPutPlace >> 9) & 1)) && (endThink != 1)) {

				score = -99999 - cntB;
				goto SKIP;		// ���̌����T������
			}


			score = minLevel(alpha, beta, depth + 1);  // ���͑���̃^�[��


		SKIP:


			// reverse �̔��΂̑���
			bB = bB ^ tmpRev;		// �Ђ�����Ԃ����΂̃}�X����ɂ���
			bW = bW | tmpRev;		// �Ђ�����Ԃ���Ă������̃}�X�Ƀr�b�g�𗧂Ă�
			bB = bB ^ tmpPutPlace;	// �b��Œu�������̐΂���菜��
			steps--;				// �萔���ЂƂ߂�




			// ���l�𒴂���Ǝ}����
			if (score >= beta) {
				return score;
			}
			// ���l���X�V
			if (score > score_max) {
				score_max = score;
				alpha = max(alpha, score_max);

				if (depth == 1) {
					// �őP����L�^
					bestX = xx;
					bestY = yy;
				}
			}

			// depth = 1 �̎��̃X�R�A���i�[
			if (depth == 1) {
				arrScore[cntArrScore++] = score;
			}
		}
	}

	for (int i = 0; i < cntArrScore; i++) printf("%6d  ", arrScore[i]);		// debug �p���@��̃X�R�A�\��

	// �u���ꏊ���Ȃ� = �p�X
	if (bSelectable == 0) {
		cntPass++;

		// ���݂����p�X�̏ꍇ����ȍ~�΂��u����邱�Ƃ��Ȃ����炻�̔Ֆʂ�]������
		if (cntPass == 2) {
			cntPass = 0;
			return evaluation_function(3);
		}

		int score = minLevel(alpha, beta, depth + 1);  // ���͑���̃^�[��;

		// ���l�𒴂���Ǝ}����
		if (score >= beta) {
			return score;
		}

		// ���l���X�V
		if (score > score_max) {
			score_max = score;
			alpha = max(alpha, score_max);

			if (depth == 1) {
				// �őP����L�^
				bestX = xx;
				bestY = yy;
			}
		}
	}


	//// �u����ꏊ���Ȃ�������̋L�q������
	//if (score_max == (int)(-INFINITY)) return evaluation_function(0); // �]���֐��̒l��Ԃ�


	return score_max;
}

int minLevel(int alpha, int beta, int depth) {
	if (depth >= depthNow) {
		//printf("koko");
		return evaluation_function(1); // �]���֐��̒l��Ԃ�
	}
	countStone();
	if (cntB + cntW == 64) return evaluation_function(1);	 // �Ֆʂ��S�����܂�����]���֐��̒l��Ԃ�
	if (cntB == 0 || cntW == 0) return evaluation_function(1);	// �ǂ��炩����̐΂��[���ɂȂ�����]���֐���Ԃ�

	int arrScore[30] = { 0 };	// depth = 1 �̎��̍��@��̃X�R�A���i�[
	int cntArrScore = 0;		// arrScore[] �̃C���f�b�N�X�p

	int score_min = (int)(INFINITY);  // �ő�X�R�A�i���̖�����ŏ�����)

	int xx = 0;		// ���u���̍��W X	�ł��]���̍�������őP��ɂ��邽�߂ɗp�ӂ���
	int yy = 0;		// ���u���̍��W Y


	// �w�����Ƃ��\�ȍ��W�S�Ăɑ΂��ĒT�����s��
	for (int i = 63; i >= 0; i--) {

		search_selectable(1);

		if ((bSelectable >> i) & 1) {

			if (++depthNum > DEPTHNUM) {
				return evaluation_function(1);		//  ���炩���ߌ��߂��ǖʐ��ɒB������]���֐��̒l��Ԃ�
			}

			int score = 0;
			xx = abs((63 - i) % 8);		// ���u���̍��W X	
			yy = abs((63 - i) / 8);		// ���u���̍��W Y

			put(xx, yy, 1);		// ���̐΂��b��Œu��
			steps++;			// �萔���J�E���g
			int tmpCntPass = cntPass;
			cntPass = 0;		// �p�X�̃J�E���g�����Z�b�g
			reverse(1);		// �΂��Ђ�����Ԃ�
			unsigned long long tmpPutPlace = putPlace;	 // �u�����΂̃}�X��ۑ��@�ǖʂ��Ƃɕۑ����Ƃ��Ȃ��ƒl���X�V����Ă��܂�����
			unsigned long long tmpRev = rev;			 // �Ђ�����Ԃ����΂̃}�X��ۑ��@���R�͓���


			// �b��Ŕ��΂�u�������Ƃō��΂��p�ɒu����悤�ɂȂ��Ă��Ȃ����`�F�b�N����
			// ���΂̍��@��̒��Ŋp���܂܂�Ă��Ȃ����𔻒�i�p���܂܂�Ă���ꍇ���̎�͍��@��ł͂Ȃ��Ɣ��f����j
			search_selectable(0);
			countStone();
			if ((((bSelectable >> 63) & 1) || ((bSelectable >> 56) & 1) ||
				((bSelectable >> 7) & 1) || ((bSelectable >> 0) & 1)) && (endThink != 1)) {	// ����p

				//// reverse �̔���
				score = 500000 + cntW;		// ( + cntW ) ���������̂��v���� **************************************************
				depthNum--;
				goto SKIP;		// ���̌����T������

			}

			// X �̃}�X�ɒu���Ȃ��悤�ɂ���
			if ((((tmpPutPlace >> 54) & 1) || ((tmpPutPlace >> 49) & 1) ||
				((tmpPutPlace >> 14) & 1) || ((tmpPutPlace >> 9) & 1)) && (endThink != 1)) {

				score = 99999 + cntW;		// ( + cntW ) ���������̂��v���� **************************************************
				goto SKIP;		// ���̌����T������
			}


			score = maxLevel(alpha, beta, depth + 1);  // ���͑���̃^�[��


		SKIP:


			// reverse �̔���
			bW = bW ^ tmpRev;		// �Ђ�����Ԃ����΂̃}�X����ɂ���
			bB = bB | tmpRev;		// �Ђ�����Ԃ���Ă������̃}�X�Ƀr�b�g�𗧂Ă�
			bW = bW ^ tmpPutPlace;	// �b��Œu�������̐΂���菜��	
			steps--;				// �萔���ЂƂ߂�


			// ���l�𒴂���Ǝ}����
			if (score <= alpha) {
				return score;
			}
			// ���l���X�V
			if (score < score_min) {
				score_min = score;
				beta = min(beta, score_min);

				if (depth == 1) {
					// �œK�Ȏ���L�^
					bestX = xx;
					bestY = yy;
				}
			}

			// depth = 1 �̎��̃X�R�A���i�[
			if (depth == 1) {
				arrScore[cntArrScore++] = score;
			}
		}
	}

	for (int i = 0; i < cntArrScore; i++) printf("%6d  ", arrScore[i]);		// debug �p���@��̃X�R�A�\��

	// �u���ꏊ���Ȃ� = �p�X
	if (bSelectable == 0) {
		cntPass++;

		// ���݂����p�X�̏ꍇ����ȍ~�΂��u����邱�Ƃ��Ȃ����炻�̔Ֆʂ�]������
		if (cntPass == 2) {
			cntPass = 0;
			return evaluation_function(3);
		}

		int score = maxLevel(alpha, beta, depth + 1);  // ���͑���̃^�[��;


		// ���l�𒴂���Ǝ}����
		if (score <= alpha) {
			return score;
		}
		// ���l���X�V
		if (score < score_min) {
			score_min = score;
			beta = min(beta, score_min);

			if (depth == 1) {
				// �œK�Ȏ���L�^
				bestX = xx;
				bestY = yy;
			}
		}

	}


	//// �u����ꏊ���Ȃ�������̋L�q������
	//if (score_min == (int)(INFINITY)) return evaluation_function(1); // �]���֐��̒l��Ԃ�


	return score_min;
}

int evaluation_function(int index) {

	int end = 0;
	int scoreXB = 0, scoreXW = 0;	// �R�̕]���֐��̒l���i�[����ϐ�
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
		// �΂��O���ɂȂ������̋L�q
		if (cntB == 0) {
			end = -99999;
			goto SKIP;
		}
		else if (cntW == 0) {
			end = 99999;
			goto SKIP;
		}


		// �������̐΂̐����O���̏ꍇ�A�X�R�A�� 99999 �ɂ���

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
		// �p�ɑ���̐΂�����ꍇ�]����������
		{
			if (endThink == 0) {
				// ��
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
				// ��
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



	// �΂�����ꏊ�ɂ��]���i X �j**********************************
	for (int i = 0; i < 64; i++) {
		if ((bB >> i) & 1) scoreXB += point[i];		// ���̓��_
		if ((bW >> i) & 1) scoreXW += point[i];		// ���̓��_
	}


	// ����\�ӏ��ɂ��]���i Y �j**********************************
	search_selectable(0);
	for (int i = 0; i < 64; i++) {
		if ((bSelectable >> i) & 1) scoreYB += (20 + point[i]);
	}
	search_selectable(1);
	for (int i = 0; i < 64; i++) {
		if ((bSelectable >> i) & 1) scoreYW += (20 + point[i]);
	}





	// �m��΂ɂ��]���i Z �j****************************************
	// �p�ɐ΂����邩��T�����Ă�������㉺���E�ɘA������΂̐��𐔂���
	// ��
	if ((bB >> 63) & 1) {	// ����p����E�ɒT��
		for (int i = 1; i < 8; i++) {
			if ((bB >> (63 - i)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	if ((bB >> 56) & 1) {	// �E��p���獶�ɒT��
		for (int i = 1; i < 8; i++) {
			if ((bB >> (56 + i)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	scoreZB += cnt;
	cnt = 0;		// �e�ӂ̒T�����ƂɃ��Z�b�g����i�e�ӂ̍ő�ΐ����W������j

	if ((bB >> 63) & 1) {	// ����p���牺�ɒT��
		for (int i = 1; i < 8; i++) {
			if ((bB >> (63 - i * 8)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	if ((bB >> 7) & 1) {	// �����p�����ɒT��
		for (int i = 1; i < 8; i++) {
			if ((bB >> (7 + i * 8)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	scoreZB += cnt;
	cnt = 0;		// �e�ӂ̒T�����ƂɃ��Z�b�g����i�e�ӂ̍ő�ΐ����W������j

	if ((bB >> 56) & 1) {	// �E��p���牺�ɒT��
		for (int i = 1; i < 8; i++) {
			if ((bB >> (56 - i * 8)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	if ((bB >> 0) & 1) {	// �E���p�����ɒT��
		for (int i = 1; i < 8; i++) {
			if ((bB >> (0 + i * 8)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	scoreZB += cnt;
	cnt = 0;		// �e�ӂ̒T�����ƂɃ��Z�b�g����i�e�ӂ̍ő�ΐ����W������j

	if ((bB >> 7) & 1) {	// �����p����E�ɒT��
		for (int i = 1; i < 8; i++) {
			if ((bB >> (7 - i)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	if ((bB >> 0) & 1) {	// �E���p���獶�ɒT��
		for (int i = 1; i < 8; i++) {
			if ((bB >> (0 + i)) & 1) cnt++;
			if (cnt > 8) cnt = 8;
		}
	}
	scoreZB += cnt;
	cnt = 0;		// �e�ӂ̒T�����ƂɃ��Z�b�g����i�e�ӂ̍ő�ΐ����W������j]



	//// ��		// ���̊m��΂̔���͂���Ȃ���������
	{
		if ((bW >> 63) & 1) {	// ����p����E�ɒT��
			for (int i = 1; i < 8; i++) {
				if ((bW >> (63 - i)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		if ((bW >> 56) & 1) {	// �E��p���獶�ɒT��
			for (int i = 1; i < 8; i++) {
				if ((bW >> (56 + i)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		scoreZW += cnt;
		cnt = 0;		// �e�ӂ̒T�����ƂɃ��Z�b�g����i�e�ӂ̍ő�ΐ����W������j

		if ((bW >> 63) & 1) {	// ����p���牺�ɒT��
			for (int i = 1; i < 8; i++) {
				if ((bW >> (63 - i * 8)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		if ((bW >> 7) & 1) {	// �����p�����ɒT��
			for (int i = 1; i < 8; i++) {
				if ((bW >> (7 + i * 8)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		scoreZW += cnt;
		cnt = 0;		// �e�ӂ̒T�����ƂɃ��Z�b�g����i�e�ӂ̍ő�ΐ����W������j

		if ((bW >> 56) & 1) {	// �E��p���牺�ɒT��
			for (int i = 1; i < 8; i++) {
				if ((bW >> (56 - i * 8)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		if ((bW >> 0) & 1) {	// �E���p�����ɒT��
			for (int i = 1; i < 8; i++) {
				if ((bW >> (0 + i * 8)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		scoreZW += cnt;
		cnt = 0;		// �e�ӂ̒T�����ƂɃ��Z�b�g����i�e�ӂ̍ő�ΐ����W������j

		if ((bW >> 7) & 1) {	// �����p����E�ɒT��
			for (int i = 1; i < 8; i++) {
				if ((bW >> (7 - i)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		if ((bW >> 0) & 1) {	// �E���p���獶�ɒT��
			for (int i = 1; i < 8; i++) {
				if ((bW >> (0 + i)) & 1) cnt++;
				if (cnt > 8) cnt = 8;
			}
		}
		scoreZW += cnt;
		cnt = 0;		// �e�ӂ̒T�����ƂɃ��Z�b�g����i�e�ӂ̍ő�ΐ����W������j
	}



	// �Ֆʂ̖����ɂ��]���iA�j
	if (endThink != 1) {		// �S�S��ڂ̋ǖʂ܂ł͎����̐΂̖��������Ȃ��Ֆʂ������]������
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


	//if (cntB == 0 || cntW == 0) return evaluation_function(0);	// �ǂ��炩����̐΂��[���ɂȂ�����]���֐���Ԃ�

	int cntOpennessBW2 = 0;
	int arrScore[30] = { 0 };	// depth = 1 �̎��̍��@��̃X�R�A���i�[
	int cntArrScore = 0;		// arrScore[] �̃C���f�b�N�X�p

	int score_max = (int)(-INFINITY);  // �ő�X�R�A�i���̖�����ŏ�����)

	int xx = 0;		// ���u���̍��W X	�ł��]���̍�������őP��ɂ��邽�߂ɗp�ӂ���
	int yy = 0;		// ���u���̍��W Y
	unsigned long long tmpRev = 0;


	for (int i = 63; i >= 0; i--) {

		search_selectable(0);		// �����łĂ�ꏊ��T��
		if ((bSelectable >> i) & 1) {

			int score = 0;
			int numOpenness = 0;

			xx = abs((63 - i) % 8);		// ���u���̍��W X
			yy = abs((63 - i) / 8);		// ���u���̍��W Y

			put(xx, yy, 0);		// ���̐΂��b��Œu��
			steps++;			// �萔���J�E���g
			int tmpCntPass = cntPass;
			cntPass = 0;		// �p�X�̃J�E���g�����Z�b�g
			reverse(0);			// �΂��Ђ�����Ԃ�
			unsigned long long tmpPutPlace = putPlace;	 // �u�����΂̃}�X��ۑ��@�ǖʂ��Ƃɕۑ����Ƃ��Ȃ��ƒl���X�V����Ă��܂�����
			tmpRev = rev;			 // �Ђ�����Ԃ����΂̃}�X��ۑ��@���R�͓���

			numOpenness = openness(tmpRev);
			*cntOpenness += numOpenness;	// ����u�������Ƃɂ��J���x�𑫂����킹��


			// �b��ō��΂�u�������ƂŔ��΂��p�ɒu����悤�ɂȂ��Ă��Ȃ����`�F�b�N����
			// ���΂̍��@��̒��Ŋp���܂܂�Ă��Ȃ����𔻒�i�p���܂܂�Ă���ꍇ���̎�͍��@��ł͂Ȃ������Ɣ��f����j
			search_selectable(1);
			countStone();
			if ((((bSelectable >> 63) & 1) || ((bSelectable >> 56) & 1) ||
				((bSelectable >> 7) & 1) || ((bSelectable >> 0) & 1)) && (endThink != 1)) {
				score = -500000 - cntB;
				//depthNum--;		// �萔���ЂƂ߂�
				goto SKIP;		// ���̌����T������
			}

			// X �̃}�X�ɒu���Ȃ��悤�ɂ���
			if ((((tmpPutPlace >> 54) & 1) || ((tmpPutPlace >> 49) & 1) ||
				((tmpPutPlace >> 14) & 1) || ((tmpPutPlace >> 9) & 1)) && (endThink != 1)) {

				score = -99999 - cntB;
				goto SKIP;		// ���̌����T������
			}

			// �J���x���_ ***********


			minLevel_openness(alpha, beta, depth + 1, cntOpenness);  // ���͑���̃^�[��

			score = -1 * (*cntOpenness + openness_evaluation_function(0));


		SKIP:


			// reverse �̔��΂̑���
			bB = bB ^ tmpRev;		// �Ђ�����Ԃ����΂̃}�X����ɂ���
			bW = bW | tmpRev;		// �Ђ�����Ԃ���Ă������̃}�X�Ƀr�b�g�𗧂Ă�
			bB = bB ^ tmpPutPlace;	// �b��Œu�������̐΂���菜��
			steps--;				// �萔���ЂƂ߂�

			*cntOpenness -= numOpenness;	// ����u�������Ƃɂ��J���x�����ɖ߂�



			//// ���l�𒴂���Ǝ}����		// �J�b�g�͂���Ȃ�����
			//if (score >= beta) {
			//	return score;
			//}

				// ���l���X�V
			if (score > score_max) {
				score_max = score;
				alpha = max(alpha, score_max);

				if (depth == 1) {
					// �őP����L�^
					bestX = xx;
					bestY = yy;
				}

			}

			arrScore[cntArrScore++] = score;
		}
	}

	if (depth == 1) {
		for (int i = 0; i < cntArrScore; i++) printf("%6d  ", arrScore[i]);		// debug �p���@��̃X�R�A�\��
	}

	// �u���ꏊ���Ȃ� = �p�X
	if (bSelectable == 0) {
		cntPass++;

		//// ���݂����p�X�̏ꍇ����ȍ~�΂��u����邱�Ƃ��Ȃ����炻�̔Ֆʂ�]������
		//if (cntPass == 2) {
		//	cntPass = 0;
		//	return ;
		//}


		minLevel_openness(alpha, beta, depth + 1, cntOpenness);  // ���͑���̃^�[��;

		int score = -1 * (*cntOpenness + openness_evaluation_function(0));


		//// ���l�𒴂���Ǝ}����
		//if (score >= beta) {
		//	return score;
		//}
		// ���l���X�V
		if (score > score_max) {
			score_max = score;
			alpha = max(alpha, score_max);

			if (depth == 1) {
				// �őP����L�^
				bestX = xx;
				bestY = yy;
			}
		}
	}


	//// �u����ꏊ���Ȃ�������̋L�q������
	//if (score_max == (int)(-INFINITY)) return evaluation_function(0); // �]���֐��̒l��Ԃ�


	return;// score_max;
}

void minLevel_openness(int alpha, int beta, int depth, int* cntOpenness) {
	if (depth > depthNowOpenness) {
		return; // openness(tmpRevOut);
	}
	countStone();

	//if (cntB == 0 || cntW == 0) return evaluation_function(1);	// �ǂ��炩����̐΂��[���ɂȂ�����]���֐���Ԃ�

	int arrScore[30] = { 0 };	// depth = 1 �̎��̍��@��̃X�R�A���i�[
	int cntArrScore = 0;		// arrScore[] �̃C���f�b�N�X�p

	int score_min = (int)(INFINITY);  // �ő�X�R�A�i���̖�����ŏ�����)

	int xx = 0;		// ���u���̍��W X	�ł��]���̍�������őP��ɂ��邽�߂ɗp�ӂ���
	int yy = 0;		// ���u���̍��W Y
	unsigned long long tmpRev = 0;


	// �w�����Ƃ��\�ȍ��W�S�Ăɑ΂��ĒT�����s��
	for (int i = 63; i >= 0; i--) {

		search_selectable(1);

		if ((bSelectable >> i) & 1) {


			int score = 0;
			int numOpenness = 0;

			xx = abs((63 - i) % 8);		// ���u���̍��W X
			yy = abs((63 - i) / 8);		// ���u���̍��W Y

			put(xx, yy, 1);		// ���̐΂��b��Œu��
			steps++;			// �萔���J�E���g
			int tmpCntPass = cntPass;
			cntPass = 0;		// �p�X�̃J�E���g�����Z�b�g
			reverse(1);		// �΂��Ђ�����Ԃ�
			unsigned long long tmpPutPlace = putPlace;	 // �u�����΂̃}�X��ۑ��@�ǖʂ��Ƃɕۑ����Ƃ��Ȃ��ƒl���X�V����Ă��܂�����
			tmpRev = rev;			 // �Ђ�����Ԃ����΂̃}�X��ۑ��@���R�͓���

			numOpenness = openness(tmpRev);
			*cntOpenness -= numOpenness;	// ���΂�u�������Ƃɂ��J���x������


			// �b��Ŕ��΂�u�������Ƃō��΂��p�ɒu����悤�ɂȂ��Ă��Ȃ����`�F�b�N����
			// ���΂̍��@��̒��Ŋp���܂܂�Ă��Ȃ����𔻒�i�p���܂܂�Ă���ꍇ���̎�͍��@��ł͂Ȃ��Ɣ��f����j
			search_selectable(0);
			countStone();
			if ((((bSelectable >> 63) & 1) || ((bSelectable >> 56) & 1) ||
				((bSelectable >> 7) & 1) || ((bSelectable >> 0) & 1)) && (endThink != 1)) {	// ����p

				score = 500000 + cntW;		// ( + cntW ) ���������̂��v���� **************************************************
				//depthNum--;
				goto SKIP;		// ���̌����T������

			}

			// X �̃}�X�ɒu���Ȃ��悤�ɂ���
			if ((((tmpPutPlace >> 54) & 1) || ((tmpPutPlace >> 49) & 1) ||
				((tmpPutPlace >> 14) & 1) || ((tmpPutPlace >> 9) & 1)) && (endThink != 1)) {

				score = 99999 + cntW;		// ( + cntW ) ���������̂��v���� **************************************************
				goto SKIP;		// ���̌����T������
			}



			// �J���x���傫������I���������̂� score �� -1 ��������iminLevel_openness �� score �̏���������I�Ԃ��߁j
			maxLevel_openness(alpha, beta, depth + 1, cntOpenness);  // ���͑���̃^�[��

			score = -1 * (*cntOpenness + openness_evaluation_function(1));

		SKIP:


			// reverse �̔���
			bW = bW ^ tmpRev;		// �Ђ�����Ԃ����΂̃}�X����ɂ���
			bB = bB | tmpRev;		// �Ђ�����Ԃ���Ă������̃}�X�Ƀr�b�g�𗧂Ă�
			bW = bW ^ tmpPutPlace;	// �b��Œu�������̐΂���菜��	
			steps--;				// �萔���ЂƂ߂�
			*cntOpenness += numOpenness;	// ����u�������Ƃɂ��J���x�����ɖ߂�


			//// ���l�𒴂���Ǝ}����	 �J�b�g�͂���Ȃ�����
			//if (score <= alpha) {
			//	return score;
			//}

				// ���l���X�V
			if (score < score_min) {
				score_min = score;
				beta = min(beta, score_min);

				if (depth == 1) {
					// �œK�Ȏ���L�^
					bestX = xx;
					bestY = yy;
				}

			}

			arrScore[cntArrScore++] = score;
		}
	}

	if (depth == 1) {
		for (int i = 0; i < cntArrScore; i++) printf("%6d  ", arrScore[i]);		// debug �p���@��̃X�R�A�\��
	}

	// �u���ꏊ���Ȃ� = �p�X
	if (bSelectable == 0) {
		cntPass++;

		//// ���݂����p�X�̏ꍇ����ȍ~�΂��u����邱�Ƃ��Ȃ����炻�̔Ֆʂ�]������
		//if (cntPass == 2) {
		//	cntPass = 0;
		//	return ;
		//}


		maxLevel_openness(alpha, beta, depth + 1, cntOpenness);  // ���͑���̃^�[��;

		int score = -1 * (*cntOpenness + openness_evaluation_function(1));


		//// ���l�𒴂���Ǝ}����
		//if (score <= alpha) {
		//	return score;
		//}
		// ���l���X�V
		if (score < score_min) {
			score_min = score;
			beta = min(beta, score_min);

			if (depth == 1) {
				// �œK�Ȏ���L�^
				bestX = xx;
				bestY = yy;
			}
		}

	}


	//// �u����ꏊ���Ȃ�������̋L�q������
	//if (score_min == (int)(INFINITY)) return evaluation_function(1); // �]���֐��̒l��Ԃ�


	return;// score_min;
}

int openness(unsigned long long rev) {


	int cntOpenness = 0;
	unsigned long long revStone = 0b0;


	// �Ђ�����Ԃ��ꂽ�΂̎��͂̋󂫃}�X�̐��𐔂���
	for (int i = 63; i >= 0; i--) {
		if ((rev >> i) & 1) {		// �Ђ�����Ԃ��ꂽ�΂̓���

			unsigned long long transferRev = 0b1000000000000000000000000000000000000000000000000000000000000000;	// ����̈ʒu�ɐ΂�u�����߂̃r�b�g��@�_���V�t�g���Ďg��
			revStone = 0b0;

			int xx = abs((63 - i) % 8);		// �Ђ�����Ԃ��ꂽ�΂̍��W X	
			int yy = abs((63 - i) / 8);		// �Ђ�����Ԃ��ꂽ�΂̍��W Y


			transferRev = transferRev >> (yy * 8 + xx);		// �Ђ�����Ԃ��ꂽ�΂̍��W�̈ʒu�Ƀr�b�g�𗧂Ă�

			for (int k = 0; k < 8; k++) {
				openness_transfer(transferRev, k, &cntOpenness);	// �Ђ�����Ԃ��ꂽ�΂̎��͂̋󂫃}�X�̐��𐔂���
			}
		}
	}

	//printf("%d\n", cntOpenness);
	return cntOpenness;
}

void openness_transfer(unsigned long long  transferRev, int k, int* cntOpenness) {

	switch (k) {
	case 0: //�E
		if ((((transferRev >> 1) > 0) && ((transferRev >> 1) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	case 1: //��
		if ((((transferRev << 1) > 0) && ((transferRev << 1) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	case 2: //��
		if ((((transferRev << 8) > 0) && ((transferRev << 8) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	case 3: //��
		if ((((transferRev >> 8) > 0) && ((transferRev >> 8) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	case 4: //�E��
		if ((((transferRev << 7) > 0) && ((transferRev << 7) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	case 5: //����
		if ((((transferRev << 9) > 0) && ((transferRev << 9) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	case 6: //�E��
		if ((((transferRev >> 9) > 0) && ((transferRev >> 9) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	case 7: //����
		if ((((transferRev >> 7) > 0) && ((transferRev >> 7) & (bB | bW))) == 0) *cntOpenness += 1;
		return;
	default:
		return;
	}
}

int openness_evaluation_function(int index) {

	int end = 0;

	if (endThink != 1) {
		// ��
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
		// ��
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


// ���܂ł̕]���֐��ɁA���͂�΂ň͂�ꂽ�}�X�ɂ��鎩���̐΂������]������R�[�h������ ***********************