#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>

#define MAX_LINESIZE 64	//1行あたりの最大文字数

struct field {
	int turn;	//ターン数
	int width;	//縦の大きさ
	int height;	//横の大きさ
	struct agent {
		int x;	//横方向の位置
		int y;	//縦方向の位置
	}a1, a2;
	std::vector<std::vector<int>> board;	//盤面の状況
};

//ファイルからフィールド情報を取得
int load(const char filename[], struct field *field);
void init_field(struct field *field);
void view(struct field *field);