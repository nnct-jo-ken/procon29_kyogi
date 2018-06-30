#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>

#define MAX_LINESIZE 64	//1�s������̍ő啶����

struct field {
	int turn;	//�^�[����
	int width;	//�c�̑傫��
	int height;	//���̑傫��
	struct agent {
		int x;	//�������̈ʒu
		int y;	//�c�����̈ʒu
	}a1, a2;
	std::vector<std::vector<int>> board;	//�Ֆʂ̏�
};

//�t�@�C������t�B�[���h�����擾
int load(const char filename[], struct field *field);
void init_field(struct field *field);
void view(struct field *field);