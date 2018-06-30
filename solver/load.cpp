#define _CRT_SECURE_NO_WARNINGS
#include "load.h"


//�Ֆʏ��̓ǂݎ��
int load(const char filename[], struct field *field) {
	FILE *fp;

	init_field(field);	//�t�B�[���h���̏�����

	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Can't open \"%s\"\n", filename);
		return -1;
	}

	fscanf(fp, "%d %d\n", &field->width, &field->height);	//�Ֆʂ̑傫������
	for (int i = 0; i < field->width; i++) {	//�Ֆʃf�[�^�̓���
		field->board.push_back(std::vector<int>());	//board[i]��std::vector<int>()������
		for (int j = 0; j < field->height; j++) {
			int point;	//�}�X�̓��_
			fscanf(fp, "%d", &point);
			field->board[i].push_back(point);
		}
	}
	fscanf(fp, "%d %d\n", &field->a1.x, &field->a1.y);	//�G�[�W�F���g�̈ʒu
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
	printf("%-15s %d\n", "�^�[����", field->turn);
	printf("%-15s %d\n", "�c�̑傫��", field->width);
	printf("%-15s %d\n", "���̑傫��", field->height);
	printf("%-15s x:%d y:%d\n", "�G�[�W�F���g1", field->a1.x, field->a1.y);
	printf("%-15s x:%d y:%d\n", "�G�[�W�F���g2", field->a2.x, field->a2.y);
	printf("�Ֆʂ̏�\n");
	for (int i = 0; i < field->width; i++) {
		for (int j = 0; j < field->height; j++) {
			printf("%3d", field->board[i][j]);
		}
		putchar('\n');
	}
}
