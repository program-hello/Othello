

int GHandle[15];		// �摜�n���h��
int FHandle[7];		// �t�H���g�n���h��
int clickFlag = 0;	// �N���b�N�t���O�@�N���b�N���Ă���ԁA�A�����ē��삷��̂�h��

int baseX = 58, baseY = 58;		// ��ԍ���̍��W
int mode = 0;	// 0 : player �� com , 1 : player �� player
int turn = 0;	// 0 : PLAYER , 1 : COM
int comColor = 0;	// 0 : �� , 1 : ��
int playerColor = 0;	// 0 : �� , 1 : ��
int cntB = 0, cntW = 0;		// ���Ɣ��̐�
int cntPass = 0;	// �p�X�̃J�E���g�@�A���łQ��p�X�������ꂽ�炻��͂ǂ�����΂�łĂȂ��̂Ŏ����I��
int steps = 1;		// ���݂̎萔
int endThink = 0;	// �S6��ȍ~���𔻒�p
int playStanderdMovesFlag = 1;	// ��΂𔲂������̃t���O	�P�F��Β��@�Q�F��΂𔲂��Ă���
int edgeFlag[4] = { 0 };		// ���܂��Ă���p��\���t���O


unsigned long long bB = 0b0000000000000000000000000000100000010000000000000000000000000000;	// bitboard Black
unsigned long long bW = 0b0000000000000000000000000001000000001000000000000000000000000000;	// bitboard White
unsigned long long bN = 0b0000000000000000000000000000000000000000000000000000000000000000;	// bitboard Now  ���݂̔Ֆ�
unsigned long long bSelectable = 0b0;	// ���@���ǉ����Ă���
unsigned long long putPlace = 0b0;		// �΂�u�����ꏊ
unsigned long long rev = 0;		   // ���]����ʒu���i�[����r�b�g�{�[�h
unsigned long long bC1 = 0;        // Bitboard Color1 (current player's stones)
unsigned long long bC2 = 0;        // Bitboard Color2 (opponent's stones)

unsigned long long horizontalMask = 0x7e7e7e7e7e7e7e7e;		// ���p�}�X�N
unsigned long long verticalMask = 0xffffffffffff00;		// ���p�}�X�N
unsigned long long allsideMask = 0x7e7e7e7e7e7e00;		// �ȂȂߗp�}�X�N

int depthNow = 10;	// �T���̐[��
int depthNum = 0;	// ���݂̒T����
int depthNowOpenness = 6;	// �J���x���_�̐[���ݒ�
int randomFreq = 0;	// �����_���ɑłm��
int bestX = 0, bestY = 0;	// �őP��̍��W

int ngs = 0;	// �S�S��ڈȍ~�����Ă���ꍇ�A�ŏI�ǖʂł̖������ő�ɂȂ�悤�ȍ��@��̃X�R�A���i�[  neber giveup score


// �]���֐��Ŏg���}�X���Ƃ̓��_
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


int cntValid = 0;	// �L���Ȓ�΂̐����J�E���g
char valid[MAXSTANDERDMOVES][128] = { "" };
char standerdMoves[MAXSTANDERDMOVES][128];