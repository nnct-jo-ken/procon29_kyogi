#include "rand_board.h"

int init::rand_board(struct board *board, int row, int column) {
	std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> rand_score(-16, 16*9);     // [-16, 16*9] 範囲の一様乱数 マイナスの点を10%近くにする

	std::random_device state;     // 非決定的な乱数生成器を生成
	std::mt19937 mt_state(state());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> rand_state(0, 2);     // 0:水平・垂直 1:水平 2:垂直

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			int tmp = rand_score(mt);
			if (tmp > 0) {     //正の乱数の割合を高める
				tmp /= 9;
			}
			int status = rand_state(mt_state);

			board[i * column + j].point = tmp;	//とにかく乱数を生成

			if (status == 0 || status == 1) {	//水平に線対称
				if (row % 2 == 0) {	//偶数
					if (i > row / 2) {
						board[i * column + j] = board[(row - i) * column + j];
					}
				}
				else {	//奇数
					if (i > row / 2 + 1) {
						board[i * column + j] = board[(row - i + 1) * column + j];
					}
				}
			}

			if (status == 0 | status == 2) {	//垂直に線対称
				if (column % 2 == 0) {	//偶数
					if (j > column / 2) {
						board[i * column + j] = board[i * column + (column - j)];
					}
				}
				else {	//奇数
					if (j > column / 2 + 1) {
						board[i * column + j] = board[i * column + (column - j + 1)];
					}
				}
			}

		}
	}

	return 0;
}