#define _CRT_SECURE_NO_WARNINGS
#include "load.h"

//盤面情報の読み取り
struct field *load(const char filename[]) {
	FILE *fp;
	struct field field;
	char line[MAX_LINESIZE] = { 0 };	//ファイルから1行ずつ格納
	int int_line[MAX_LINESIZE] = { 0 };

	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Can't open \"%s\"\n", filename);
		return NULL;
	}

	for (int i = 1; fgets(line, MAX_LINESIZE, fp) != NULL; i++) {	//i行目
		field.turn = 0;

		int a = 0;	//int_line内での次の代入位置
		for (int j = 0; j < MAX_LINESIZE; j++) {	//1行内でのj文字目
			if (line[j] == ' ') continue;	//半角スペースはとばす
			int_line[a] = atoi(&line[j]);	//数字に変換して代入
			a++;
		}

		if (i == 1) {
			field.width = int_line[0];
			field.height = int_line[1];
			
			field.board = (int **)malloc(field.width * sizeof(int *));	//盤面データを格納する二次元配列を作成
			for (int j = 0; j < field.width; j++) {
				field.board[j] = (int *)malloc(field.height * sizeof(int));
			}
		}
		else if (i <= field.width + 1) {
			for (int j = 0; j < field.height; j++) {
				field.board[i][j] = int_line[j];	//盤面データの代入
			}
		}
		else if (i == 1 + field.width + 1) {	//エージェント1の配置情報
			field.a1.x = int_line[0];
			field.a1.y = int_line[1];
		}
		else if (i == 1 + field.width + 2) {	//エージェント2の配置情報
			field.a2.x = int_line[0];
			field.a2.y = int_line[1];
		}
	}

	fclose(fp);
	return &field;
}

void view(struct field *field)
{
	printf("%10s %d\n", "ターン数", field->turn);
	printf("%10s %d\n", "縦の大きさ", field->width);
	printf("%10s %d\n", "横の大きさ", field->height);
	printf("%10s x:%d y:%d\n", "エージェント1", field->a1.x, field->a1.y);
	printf("%10s x:%d y:%d\n", "エージェント2", field->a2.x, field->a2.y);
	printf("盤面の状況\n");
	for (int i = 0; i < field->width; i++) {
		for (int j = 0; j < field->height; j++) {
			printf("%3d", field->board[i][j]);
		}
		putchar('\n');
	}
}
