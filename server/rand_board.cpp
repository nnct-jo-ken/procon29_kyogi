#include "rand_board.h"

int init::rand_board(struct board *board, int row, int column) {
	std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> rand_score(-16, 33);     // [-16, 32] 範囲の一様乱数

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			int tmp = rand_score(mt);
			if (tmp > 0) {     //正の乱数の割合を高める
				tmp /= 2;
			}
			board[i * column + j].point = tmp;
		}
	}

	return 0;
}