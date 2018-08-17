#pragma once
#include "field.h"
#include <vector>

struct next_move {
	int x, y;	//�ړ���̍��W
	int value;	//�]���l
};

struct nextmoves {
	next_move a1, a2;
};

///<summary>
///MiniMax�@
///</summary>
///<params  name="depth">�T���[��</params>
int minimax(struct board_template & field, int depth);

/*
///<summary>
///�Ֆʂ���͂���ƁA�e��̕]���l���\���̔z��ŕԂ�
///field������������񂷂̂ŁA���̂Ŏ󂯎��
///</summary>
std::vector<int> search(struct board_template field);
*/

///<summary>
///�Ֆʂ̕]���l����
///</summary>
int value(const struct board_template & field);

///<summary>
///�w�肵���_���͂܂�Ă�����1��Ԃ�
///</summary>
///<params name="state">�w�`</params>
///<params name="x">�͂܂�Ă��邩���ׂ�ʒu��X���W</params>
///<params name="y">�͂܂�Ă��邩���ׂ�ʒu��Y���W</params>
///<params name="team">1:���w 2:�G�w</params>
///<params name="tmp">�͂��Ȃ�:0 �͂���:1</params>
int inclose_check(const struct board_template & field, int x, int y, int team, std::vector<std::vector<int>> & tmp);