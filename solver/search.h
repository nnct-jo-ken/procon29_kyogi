#pragma once
#include "load.h"

struct next_move {
	int x, y;	//���W
	int value;	//�]���l
};

///<summary>
///�Ֆʂ���͂���ƁA�e��̕]���l���\���̔z��ŕԂ�
///</summary>
std::vector<std::vector<int>> search(struct field field);

///<summary>
///�Ֆʂ̕]���l����
///</summary>
int value(struct field field);