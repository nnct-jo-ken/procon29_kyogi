#pragma once
#include "load.h"

struct next_move {
	int x, y;	//座標
	int value;	//評価値
};

///<summary>
///MiniMax法
///</summary>
///<params  name="depth">探索深さ</params>
struct next_move minimax(struct field & field, int depth);

///<summary>
///盤面を入力すると、各手の評価値を構造体配列で返す
///field情報をいじくり回すので、実体で受け取り
///</summary>
std::vector<int> search(struct field field);

///<summary>
///盤面の評価値生成
///</summary>
int value(const struct field & field);

///<summary>
///指定した点が囲まれていたら1を返す
///</summary>
///<params name="state">陣形</params>
///<params name="x">囲まれているか調べる位置のX座標</params>
///<params name="y">囲まれているか調べる位置のY座標</params>
///<params name="team">1:自陣 2:敵陣</params>
///<params name="tmp">囲われない:0 囲われる:1</params>
int inclose_check(const field & field, int x, int y, int team, std::vector<std::vector<int>> & tmp);