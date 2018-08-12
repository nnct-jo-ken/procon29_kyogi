#pragma once
#include "load.h"

struct next_move {
	int x, y;	//座標
	int value;	//評価値
};

///<summary>
///盤面を入力すると、各手の評価値を構造体配列で返す
///</summary>
std::vector<std::vector<int>> search(struct field field);

///<summary>
///盤面の評価値生成
///</summary>
int value(struct field field);