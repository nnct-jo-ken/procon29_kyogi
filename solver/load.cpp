#define _CRT_SECURE_NO_WARNINGS
#include "load.h"


//盤面情報の読み取り
int load(const char filename[], struct field *field) {
	FILE *fp;

	init_field(field);	//フィールド情報の初期化

	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Can't open \"%s\"\n", filename);
		return -1;
	}

	fscanf(fp, "%d %d\n", &field->width, &field->height);	//盤面の大きさを代入
	for (int i = 0; i < field->width; i++) {	//盤面データの投入
		field->board.push_back(std::vector<int>());	//board[i]にstd::vector<int>()を入れる
		for (int j = 0; j < field->height; j++) {
			int point;	//マスの得点
			fscanf(fp, "%d", &point);
			field->board[i].push_back(point);
		}
	}
	fscanf(fp, "%d %d\n", &field->a1.x, &field->a1.y);	//エージェントの位置
	fscanf(fp, "%d %d\n", &field->a2.x, &field->a2.y);

	fclose(fp);
	return 0;
}

void init_field(struct field *field) {
	field->turn = 0;
	field->width = 0;
	field->height = 0;
	field->a1.x = 0;
	field->a1.y = 0;
	field->a2.x = 0;
	field->a2.y = 0;
}

void view(struct field *field)
{
	printf("%-15s %d\n", "ターン数", field->turn);
	printf("%-15s %d\n", "縦の大きさ", field->width);
	printf("%-15s %d\n", "横の大きさ", field->height);
	printf("%-15s x:%d y:%d\n", "エージェント1", field->a1.x, field->a1.y);
	printf("%-15s x:%d y:%d\n", "エージェント2", field->a2.x, field->a2.y);
	printf("盤面の状況\n");
	for (int i = 0; i < field->width; i++) {
		for (int j = 0; j < field->height; j++) {
			printf("%3d", field->board[i][j]);
		}
		putchar('\n');
	}
}
