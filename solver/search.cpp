#include "search.h"
#include <vector>

std::vector<int> search(struct field field)
{
	std::vector<int> values;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (field.a1.x + i < 0 || field.a1.x + i > field.width)	//領域突破した場合は、とばす
				continue;
			if (field.a1.y + i < 0 || field.a1.y + i > field.height)	//領域突破した場合は、とばす
				continue;
			field.a1.x += i;
			field.a1.y += j;
			field.state[field.a1.x + i][field.a1.x + j] = 1;
			values.push_back(value(field));
		}
	}

	return values;
}

///<summary>
///上下左右に自陣のマスがあるか探索
///なければ、そのマスに移動して、再帰的に探索
///塗りつぶしができれば、囲まれている
///壁に達すると、囲まれていない
///
///**** 全マスを何回も探索しているから、改善する必要アリ！！！ ****
///**** さすがに頭の悪い実装方法 ****
///**** 「メモ化」を参照すると良いかも ****
///</summary>
int value(const struct field & field)
{
	int value = 0;
	int tmp_num1, tmp_num2;	//ホントに一時保存用
	
	std::vector<std::vector<int>> inclose;	//囲われない:0 自陣が囲う:1 敵陣が囲う:2 自陣も敵陣も囲う:3
	std::vector<std::vector<int>> tmp;		//一時記憶用　囲われない:0 自陣が囲う:1 敵陣が囲う:2 自陣も敵陣も囲う:3

	for (int i = 0; i < field.width; i++) {
		inclose.push_back(std::vector<int>());	//int型配列の要素にint型配列を格納して、二重配列にする
		tmp.push_back(std::vector<int>());
		for (int j = 0; j < field.height; j++) {
			inclose[i].push_back(0);
			tmp[i].push_back(0);
		}
	}

	//左上から、全マスを起点に調査
	for (int i = 0; i < field.width; i++) {
		for (int j = 0; j < field.height; j++) {
			//if (inclose[i][j] > 0) continue;	//すでに囲われるとわかっているのなら、とばす <= 重複を考えると、保留
			//inclose[i][j] = inclose_check(field, i, j, 1, tmp);	//囲いが重複しているのを考慮しないといけない！！！<=後で対応
			tmp_num1 = inclose_check(field, i, j, 1, tmp);
			tmp_num2 = inclose_check(field, i, j, 2, tmp);
			if (tmp_num1 == 0 && tmp_num2 == 0)
				inclose[i][j] = 0;
			else if (tmp_num1 == 1 && tmp_num2 == 0)
				inclose[i][j] = 1;
			else if (tmp_num1 == 0 && tmp_num2 == 1)
				inclose[i][j] = 2;
			else if (tmp_num1 == 1 && tmp_num2 == 1)
				inclose[i][j] = 3;
			/*
			if (inclose[i][j] == 1)	//囲まれている
			{
				//tmpの1をincloseに反映させる
				//探索済みの結果を保存することで、計算量削減
				for (int k = 0; k < field.width; k++) {
					for (int l = 0; l < field.height; l++) {
						if (tmp[k][l] == 1) {
							inclose[k][l] = tmp[k][l];
						}
					}
				}
			}
			

			for (int k = 0; k < field.width; k++) {
				for (int l = 0; l < field.height; l++) {
					tmp[k][l] = 0;	//初期化
				}
			}
			*/
		}
	}

	/*
	領域ポイントとタイルポイントの合算
	*/
	for (int i = 0; i < field.width; i++) {
		for (int j = 0; j < field.height; j++) {
			if (field.state[i][j] == 1)	//自陣
				value += field.value[i][j];
			else if (field.state[i][j] == 2)//敵陣
				value -= field.value[i][j];

			if (inclose[i][j] == 1)			//囲まれている
				value += field.value[i][j];

			//***敵陣の囲いについては考慮していない***
		}
	}

	return value;
}

///tmpは、指定された点が囲まれている場合は、他の点の状況も記録されているため、有用
///囲まれていない場合は、無用
int inclose_check(const field & field, int x, int y, int team, std::vector<std::vector<int>> & tmp)
{
	//領域を突破したら、囲えないことが確定
	//もともとの配列で、外側を何かの数字で囲んでもいいかも
	if (x < 0 || x > field.width || y < 0 || y > field.height)
		return 0;

	int another_team;	//teamの反対

	if (team == 1)
		another_team = 2;
	else
		another_team = 1;

	tmp[x][y] = team;

	if(field.state[x-1][y] == 0 || field.state[x - 1][y] == another_team)
		inclose_check(field, x - 1, y, team, tmp);
	if (field.state[x + 1][y] == 0 || field.state[x + 1][y] == another_team)
		inclose_check(field, x + 1, y, team, tmp);
	if (field.state[x][y-1] == 0 || field.state[x][y - 1] == another_team)
		inclose_check(field, x, y - 1, team, tmp);
	if (field.state[x][y+1] == 0 || field.state[x][y + 1] == another_team)
		inclose_check(field, x, y + 1, team, tmp);

	return 1;
}
