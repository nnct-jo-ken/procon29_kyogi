#define _CRT_SECURE_NO_WARNINGS
#include "field.h"


//�Ֆʏ��̓ǂݎ��
int field::load (const char filename[], struct board_template & field) {
	FILE *fp;

	init_field(field);	//�t�B�[���h���̏�����

	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Can't open \"%s\"\n", filename);
		return -1;
	}

	fscanf(fp, "%d %d\n", &field.width, &field.height);	//�Ֆʂ̑傫������
	for (int i = 0; i < field.width; i++) {	//�Ֆʃf�[�^�̓���
		field.value.push_back(std::vector<int>());	//value[i]��std::vector<int>()������
		field.state.push_back(std::vector<int>());	//state[i]��std::vector<int>()������
		for (int j = 0; j < field.height; j++) {
			int point;	//�}�X�̓��_
			fscanf(fp, "%d", &point);
			field.value[i].push_back(point);
			field.state[i].push_back(0);
		}
	}
	fscanf(fp, "%d %d\n", &field.a1.x, &field.a1.y);	//�G�[�W�F���g�̈ʒu
	fscanf(fp, "%d %d\n", &field.a2.x, &field.a2.y);

	fclose(fp);
	return 0;
}

void field::init_field(struct board_template & field) {
	field.turn = 0;
	field.width = 0;
	field.height = 0;
	field.a1.x = 0;
	field.a1.y = 0;
	field.a2.x = 0;
	field.a2.y = 0;
}

void field::view(struct board_template & field)
{
	printf("%-15s %d\n", "�^�[����", field.turn);
	printf("%-15s %d\n", "�c�̑傫��", field.width);
	printf("%-15s %d\n", "���̑傫��", field.height);
	printf("%-15s x:%d y:%d\n", "�G�[�W�F���g1", field.a1.x, field.a1.y);
	printf("%-15s x:%d y:%d\n", "�G�[�W�F���g2", field.a2.x, field.a2.y);
	printf("�}�X�̓_��\n");
	for (int i = 0; i < field.width; i++) {
		for (int j = 0; j < field.height; j++) {
			printf("%3d", field.value[i][j]);
		}
		putchar('\n');
	}
	printf("�w�`\n");
	for (int i = 0; i < field.width; i++) {
		for (int j = 0; j < field.height; j++) {
			printf("%3d", field.state[i][j]);
		}
		putchar('\n');
	}
}
