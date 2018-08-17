#pragma once
#include "field.h"
#include <vector>

struct next_move {
	int x, y;	//移動先の座標
	int value;	//評価値
};

struct nextmoves {
	next_move a1, a2;
};

///<summary>
///MiniMax法
///</summary>
///<params  name="depth">探索深さ</params>
int minimax(struct board_template & field, int depth);

/*
///<summary>
///盤面を入力すると、各手の評価値を構造体配列で返す
///field情報をいじくり回すので、実体で受け取り
///</summary>
std::vector<int> search(struct board_template field);
*/

///<summary>
///盤面の評価値生成
///</summary>
int value(const struct board_template & field);

///<summary>
///指定した点が囲まれていたら1を返す
///</summary>
///<params name="state">陣形</params>
///<params name="x">囲まれているか調べる位置のX座標</params>
///<params name="y">囲まれているか調べる位置のY座標</params>
///<params name="team">1:自陣 2:敵陣</params>
///<params name="tmp">囲われない:0 囲われる:1</params>
int inclose_check(const struct board_template & field, int x, int y, int team, std::vector<std::vector<int>> & tmp);