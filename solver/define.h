#pragma once
#include <vector>

struct field {
	int turn;	//�^�[����
	int width;	//�c�̑傫��
	int height;	//���̑傫��
	struct agent {
		int x;	//�������̈ʒu
		int y;	//�c�����̈ʒu
	}a1, a2;
	std::vector<std::vector<int>> value;	//�_��
	std::vector<std::vector<int>> state;	//0:�ǂ���ł��Ȃ� 1:���w 2:�G�w
};
