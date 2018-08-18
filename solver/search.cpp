#include "search.h"
#include <vector>

int minimax(struct board_template & field, int depth)
{
	int score = 0;
	int tmp1, tmp2;	//移動先の移動前の状態の保存

	if (depth == 0) return value(field);	//末端まで達したら、そのボードの評価値を返す

	if (depth % 2 == 0)	//自陣
	{
		int max = -1000;	//とりあえず小さな値にする

		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				for (int k = -1; k <= 1; k++) {
					for (int l = -1; l <= 1; l++) {
						if (field.own_a1.x + i < 1 || field.own_a1.x + i > field.width)	//領域突破した場合は、とばす
							continue;
						if (field.own_a1.y + j < 1 || field.own_a1.y + j > field.height)
							continue;
						if (field.own_a2.x + k < 1 || field.own_a2.x + k > field.width)
							continue;
						if (field.own_a2.y + l < 1 || field.own_a2.y + l > field.height)
							continue;

						//fieldを次の手に変化させる
						field.own_a1.x += i;
						field.own_a1.y += j;
						tmp1 = field.state[field.own_a1.x -1][field.own_a1.y -1];
						field.state[field.own_a1.x -1][field.own_a1.y -1] = 1;

						field.own_a2.x += k;
						field.own_a2.y += l;
						tmp2 = field.state[field.own_a2.x -1][field.own_a2.y -1];
						field.state[field.own_a2.x -1][field.own_a2.y -1] = 1;

						score = minimax(field, depth - 1);
						if (score > max) max = score;

						//fieldを元に戻す
						field.own_a1.x -= i;
						field.own_a1.y -= j;
						field.state[field.own_a1.x -1][field.own_a1.y -1] = tmp1;

						field.own_a2.x -= k;
						field.own_a2.y -= l;
						field.state[field.own_a2.x -1][field.own_a2.y -1] = tmp2;
					}
				}
			}
		}
		return max;
	}
	else	//敵陣
	{		
		int min = 1000;	//とりあえず大きい値にする

		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				for (int k = -1; k <= 1; k++) {
					for (int l = -1; l <= 1; l++) {
						if (field.own_a1.x + i < 1 || field.own_a1.x + i > field.width)	//領域突破した場合は、とばす
							continue;
						if (field.own_a1.y + j < 1 || field.own_a1.y + j > field.height)
							continue;
						if (field.own_a2.x + k < 1 || field.own_a2.x + k > field.width)
							continue;
						if (field.own_a2.y + l < 1 || field.own_a2.y + l > field.height)
							continue;

						//fieldを次の手に変化させる
						field.own_a1.x += i;
						field.own_a1.y += j;
						tmp1 = field.state[field.own_a1.x -1][field.own_a1.y -1];
						field.state[field.own_a1.x -1][field.own_a1.y -1] = 1;

						field.own_a2.x += k;
						field.own_a2.y += l;
						tmp2 = field.state[field.own_a2.x -1][field.own_a2.y -1];
						field.state[field.own_a2.x -1][field.own_a2.y -1] = 1;

						score = minimax(field, depth - 1);
						if (score < min) min = score;

						//fieldを元に戻す
						field.own_a1.x -= i;
						field.own_a1.y -= j;
						field.state[field.own_a1.x -1][field.own_a1.y -1] = tmp1;

						field.own_a2.x -= k;
						field.own_a2.y -= l;
						field.state[field.own_a2.x -1][field.own_a2.y -1] = tmp2;
					}
				}
			}
		}
		return min;
	}
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
int value(const struct board_template & field)
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

	//左上から、全マスを起点に囲まれているか調査
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
	
	//*** debug begin ***
	//評価値を求めるためにフィールドを書き換えているから、実際の状況とは違う
	putchar('\n');
	printf("inclose\n");
	for (int i = 0; i < field.width; i++) {
		for (int j = 0; j < field.height; j++) {
			printf("%3d", inclose[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');

	putchar('\n');
	printf("state\n");
	for (int i = 0; i < field.width; i++) {
		for (int j = 0; j < field.height; j++) {
			printf("%3d", field.state[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');
	//*** debug end ***

	/*
	領域ポイントとタイルポイントの合算
	*/
	for (int i = 0; i < field.width; i++) {
		for (int j = 0; j < field.height; j++) {
			if (field.state[i][j] == 1)	//自陣
				value += field.value[i][j];
			else if (field.state[i][j] == 2)//敵陣
				value -= field.value[i][j];

			//if (inclose[i][j] == 1)			//囲まれている
			//	value += field.value[i][j];

			//***敵陣の囲いについては考慮していない***
		}
	}

	return value;
}

///tmpは、指定された点が囲まれている場合は、他の点の状況も記録されているため、有用
///囲まれていない場合は、無用

//自陣か敵陣のどちらが囲っているのか、区別をつける
//* 壁の存在はstateを見て、実際に囲う印をつけるのは、tmpのほう
//* 壁は、自陣と敵陣の2通りで計算 = > 時間は大丈夫かな...？わからん。
int inclose_check(const struct board_template & field, int x, int y, int team,std::vector<std::vector<int>> & tmp)
{
	//フィールドの端にいる時点で、囲えない
	if (x < 1 || x > field.width-2 || y < 1 || y > field.height-2)
		return 0;

	if (tmp[x][y] == team)	//探索済み
		return 1;

	int another_team;	//teamの反対
	int val;

	if (team == 1)
		another_team = 2;
	else
		another_team = 1;

	tmp[x][y] = team;

	if (field.state[x - 1][y] == 0 || field.state[x - 1][y] == another_team) {
		val = inclose_check(field, x - 1, y, team, tmp);
		if (val == 0) return 0;	//囲えないとわかったら、上流まで0を返していく
	}
	if (field.state[x + 1][y] == 0 || field.state[x + 1][y] == another_team) {
		val = inclose_check(field, x + 1, y, team, tmp);
		if (val == 0) return 0;
	}
	if (field.state[x][y - 1] == 0 || field.state[x][y - 1] == another_team) {
		val = inclose_check(field, x, y - 1, team, tmp);
		if (val == 0) return 0;
	}
	if (field.state[x][y + 1] == 0 || field.state[x][y + 1] == another_team) {
		val = inclose_check(field, x, y + 1, team, tmp);
		if (val == 0) return 0;
	}
		
	return 1;
}
