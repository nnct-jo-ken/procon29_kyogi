#pragma once
#include <vector>
#include <iostream>
#include <stdlib.h>

struct board_template {
	int turn;	//�^�[����
	int width;	//�c�̑傫��
	int height;	//���̑傫��
	struct agent {
		int x;	//�������̈ʒu
		int y;	//�c�����̈ʒu
	}own_a1, own_a2, enemy_a1, enemy_a2;
	std::vector<std::vector<int>> value;	//�_��
	std::vector<std::vector<int>> state;	//0:�ǂ���ł��Ȃ� 1:���w 2:�G�w
};

class field
{
public:
	struct board_template board;

	//�t�@�C������t�B�[���h�����擾
	int load(const char filename[], struct board_template & field);
	//�t�B�[���h���̏�����
	void init_field(struct board_template & field);
	//�t�B�[���h���̕\��
	void view(struct board_template & field);
};