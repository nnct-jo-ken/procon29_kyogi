#pragma once
#include <vector>

struct field {
	int turn;	//ターン数
	int width;	//縦の大きさ
	int height;	//横の大きさ
	struct agent {
		int x;	//横方向の位置
		int y;	//縦方向の位置
	}a1, a2;
	std::vector<std::vector<int>> value;	//点数
	std::vector<std::vector<int>> state;	//0:どちらでもない 1:自陣 2:敵陣
};
