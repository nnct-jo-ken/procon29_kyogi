#pragma once

#include <random>

struct board
{
	int point;	//�}�X�̓_��
	int state;	//�}�X�̏�� 0:�� 1:���w 2:�G�w
};

class init {

public:
	int rand_board(struct board *board, int row, int column); //*board:2�����z�� row:�s column:��
};