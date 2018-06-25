#pragma once
#include <iostream>
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
	int **board;	//盤面の状況
};

//ファイルからフィールド情報を取得
struct field *load(const char filename[]);
void view(struct field *field);