#pragma once

#include <random>

struct board
{
	int point;	//マスの点数
	int state;	//マスの状態 0:空 1:自陣 2:敵陣
};

class init {

public:
	int rand_board(struct board *board, int row, int column); //*board:2次元配列 row:行 column:列
};