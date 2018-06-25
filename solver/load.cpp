#define _CRT_SECURE_NO_WARNINGS
#include "load.h"

//�Ֆʏ��̓ǂݎ��
struct field *load(const char filename[]) {
	FILE *fp;
	struct field field;
	char line[MAX_LINESIZE] = { 0 };	//�t�@�C������1�s���i�[
	int int_line[MAX_LINESIZE] = { 0 };

	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Can't open \"%s\"\n", filename);
		return NULL;
	}

	for (int i = 1; fgets(line, MAX_LINESIZE, fp) != NULL; i++) {	//i�s��
		field.turn = 0;

		int a = 0;	//int_line���ł̎��̑���ʒu
		for (int j = 0; j < MAX_LINESIZE; j++) {	//1�s���ł�j������
			if (line[j] == ' ') continue;	//���p�X�y�[�X�͂Ƃ΂�
			int_line[a] = atoi(&line[j]);	//�����ɕϊ����đ��
			a++;
		}

		if (i == 1) {
			field.width = int_line[0];
			field.height = int_line[1];
			
			field.board = (int **)malloc(field.width * sizeof(int *));	//�Ֆʃf�[�^���i�[����񎟌��z����쐬
			for (int j = 0; j < field.width; j++) {
				field.board[j] = (int *)malloc(field.height * sizeof(int));
			}
		}
		else if (i <= field.width + 1) {
			for (int j = 0; j < field.height; j++) {
				field.board[i][j] = int_line[j];	//�Ֆʃf�[�^�̑��
			}
		}
		else if (i == 1 + field.width + 1) {	//�G�[�W�F���g1�̔z�u���
			field.a1.x = int_line[0];
			field.a1.y = int_line[1];
		}
		else if (i == 1 + field.width + 2) {	//�G�[�W�F���g2�̔z�u���
			field.a2.x = int_line[0];
			field.a2.y = int_line[1];
		}
	}

	fclose(fp);
	return &field;
}

void view(struct field *field)
{
	printf("%10s %d\n", "�^�[����", field->turn);
	printf("%10s %d\n", "�c�̑傫��", field->width);
	printf("%10s %d\n", "���̑傫��", field->height);
	printf("%10s x:%d y:%d\n", "�G�[�W�F���g1", field->a1.x, field->a1.y);
	printf("%10s x:%d y:%d\n", "�G�[�W�F���g2", field->a2.x, field->a2.y);
	printf("�Ֆʂ̏�\n");
	for (int i = 0; i < field->width; i++) {
		for (int j = 0; j < field->height; j++) {
			printf("%3d", field->board[i][j]);
		}
		putchar('\n');
	}
}
