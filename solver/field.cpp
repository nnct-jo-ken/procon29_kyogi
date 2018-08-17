#define _CRT_SECURE_NO_WARNINGS
#include "field.h"


//盤面情報の読み取り
int field::load (const char filename[], struct board_template & field) {
	FILE *fp;

	init_field(field);	//フィールド情報の初期化

	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Can't open \"%s\"\n", filename);
		return -1;
	}

	fscanf(fp, "%d %d\n", &field.width, &field.height);	//盤面の大きさを代入
	for (int i = 0; i < field.width; i++) {	//盤面データの投入
		field.value.push_back(std::vector<int>());	//value[i]にstd::vector<int>()を入れる
		field.state.push_back(std::vector<int>());	//state[i]にstd::vector<int>()を入れる
		for (int j = 0; j < field.height; j++) {
			int point;	//マスの得点
			fscanf(fp, "%d", &point);
			field.value[i].push_back(point);
			field.state[i].push_back(0);
		}
	}
	fscanf(fp, "%d %d\n", &field.own_a1.x, &field.own_a1.y);	//エージェントの位置
	fscanf(fp, "%d %d\n", &field.own_a2.x, &field.own_a2.y);
	field.state[field.own_a1.x - 1][field.own_a1.y - 1] = 1;	//初期位置を自陣としてマーク
	field.state[field.own_a2.x - 1][field.own_a2.y - 1] = 1;

	fclose(fp);
	return 0;
}

void field::init_field(struct board_template & field) {
	field.turn = 0;
	field.width = 0;
	field.height = 0;
	field.own_a1.x = 0;
	field.own_a1.y = 0;
	field.own_a2.x = 0;
	field.own_a2.y = 0;
	field.enemy_a1.x = 0;
	field.enemy_a1.y = 0;
	field.enemy_a2.x = 0;
	field.enemy_a2.y = 0;
}

void field::view(struct board_template & field)
{
	printf("%-15s %d\n", "ターン数", field.turn);
	printf("%-15s %d\n", "縦の大きさ", field.width);
	printf("%-15s %d\n", "横の大きさ", field.height);
	printf("%-15s x:%d y:%d\n", "[自]エージェント1", field.own_a1.x, field.own_a1.y);
	printf("%-15s x:%d y:%d\n", "[自]エージェント2", field.own_a2.x, field.own_a2.y);
	printf("%-15s x:%d y:%d\n", "[敵]エージェント1", field.enemy_a1.x, field.enemy_a1.y);
	printf("%-15s x:%d y:%d\n", "[敵]エージェント2", field.enemy_a2.x, field.enemy_a2.y);
	printf("マスの点数\n");
	for (int i = 0; i < field.width; i++) {
		for (int j = 0; j < field.height; j++) {
			printf("%3d", field.value[i][j]);
		}
		putchar('\n');
	}
	printf("陣形(0:空 1:自 2:敵)\n");
	for (int i = 0; i < field.width; i++) {
		for (int j = 0; j < field.height; j++) {
			printf("%3d", field.state[i][j]);
		}
		putchar('\n');
	}
}
