#pragma once
#include <vector>
#include <iostream>
#include <stdlib.h>

struct board_template {
	int turn;	//ターン数
	int width;	//縦の大きさ
	int height;	//横の大きさ
	struct agent {
		int x;	//横方向の位置
		int y;	//縦方向の位置
	}own_a1, own_a2, enemy_a1, enemy_a2;
	std::vector<std::vector<int>> value;	//点数
	std::vector<std::vector<int>> state;	//0:どちらでもない 1:自陣 2:敵陣
};

class field
{
public:
	struct board_template board;

	//ファイルからフィールド情報を取得
	int load(const char filename[], struct board_template & field);
	//フィールド情報の初期化
	void init_field(struct board_template & field);
	//フィールド情報の表示
	void view(struct board_template & field);
};