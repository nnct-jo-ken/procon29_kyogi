#include "rand_board.h"

int init::rand_board(struct board *board, int row, int column) {
	std::random_device rnd;     // �񌈒�I�ȗ���������𐶐�
	std::mt19937 mt(rnd());     //  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
	std::uniform_int_distribution<> rand_score(-16, 33);     // [-16, 32] �͈͂̈�l����

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			int tmp = rand_score(mt);
			if (tmp > 0) {     //���̗����̊��������߂�
				tmp /= 2;
			}
			board[i * column + j].point = tmp;
		}
	}

	return 0;
}