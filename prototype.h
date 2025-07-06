

void Handle_load();					// �e��n���h���̓ǂݍ���
void title();						// �^�C�g�����
void load_standerdMoves();			// ��΃t�@�C���̓ǂݍ���
void initialize();					// �e��ϐ��̏�����
void ending();						// �������ʂ�\��
void draw_board();					// �Ֆʂ̕`��
void printBits(long long n);		// ����̔Ֆʂ��R���\�[���ɕ\������
void put(int x, int y, int color);	// �΂�u��  turn : 0 = �� , 1 = ��
void remove(int x, int y, int color);	// �΂���菜���@minimax�@�Ŏg��
void search_selectable(int color);	// ���@���T��
void select_player();				// �΂�u���}�X��I������
void select_com();					// �v�l���[�`���Ő΂�u��
void countStone();					// �΂̐��𐔂���
void reverse(int color);			// �΂��Ђ�����Ԃ�
unsigned long long transfer(unsigned long long  putPlace, int k);	// �΂�u�����ꏊ����W�����̂����ꂩ�P�����ɂP�}�X�i�߂�
void loadStanderdMoves();				// ��΂̓ǂݍ���
int playStanderdMoves();				// ��΂����s

int maxLevel(int alpha, int beta, int depth);	// minimax�@
int minLevel(int alpha, int beta, int depth);	// minimax�@
int evaluation_function(int index);		// �]���֐� 

void maxLevel_openness(int alpha, int beta, int depth, int* cntOpenness);		// �J���x���_�p�̃~�j�}�b�N�X�@���
void minLevel_openness(int alpha, int beta, int depth, int* cntOpenness);		// �J���x���_�p�̃~�j�}�b�N�X�@���
int openness(unsigned long long rev);	// �J���x���_
void openness_transfer(unsigned long long  transferRev, int k, int* cntOpenness);	// �J���x���_�ŊJ���x���J�E���g����
int openness_evaluation_function(int index);	// �J���x���_�p�̕]���֐�