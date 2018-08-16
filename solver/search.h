#pragma once
#include "load.h"

struct next_move {
	int x, y;	//���W
	int value;	//�]���l
};

///<summary>
///MiniMax�@
///</summary>
///<params  name="depth">�T���[��</params>
struct next_move minimax(struct field & field, int depth);

///<summary>
///�Ֆʂ���͂���ƁA�e��̕]���l���\���̔z��ŕԂ�
///field������������񂷂̂ŁA���̂Ŏ󂯎��
///</summary>
std::vector<int> search(struct field field);

///<summary>
///�Ֆʂ̕]���l����
///</summary>
int value(const struct field & field);

///<summary>
///�w�肵���_���͂܂�Ă�����1��Ԃ�
///</summary>
///<params name="state">�w�`</params>
///<params name="x">�͂܂�Ă��邩���ׂ�ʒu��X���W</params>
///<params name="y">�͂܂�Ă��邩���ׂ�ʒu��Y���W</params>
///<params name="team">1:���w 2:�G�w</params>
///<params name="tmp">�͂��Ȃ�:0 �͂���:1</params>
int inclose_check(const field & field, int x, int y, int team, std::vector<std::vector<int>> & tmp);