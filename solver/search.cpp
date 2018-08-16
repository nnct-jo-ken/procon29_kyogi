#include "search.h"
#include <vector>

struct nextmoves minimax(struct board_template & field, int depth)
{
	nextmoves next = { 0,0,0,0,0,0 };

	return next;
}

std::vector<int> search(struct board_template field)
{
	std::vector<int> values;
	int val;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				for (int l = -1; l <= 1; l++) {
					val = 0;

					if (field.a1.x + i < 0 || field.a1.x + i > field.width)	//�̈�˔j�����ꍇ�́A�Ƃ΂�
						continue;
					if (field.a1.y + j < 0 || field.a1.y + j > field.height)	//�̈�˔j�����ꍇ�́A�Ƃ΂�
						continue;
					field.a1.x += i;
					field.a1.y += j;
					field.state[field.a1.x + i][field.a1.x + j] = 1;
					val += value(field);

					if (field.a2.x + k < 0 || field.a2.x + k > field.width)	//�̈�˔j�����ꍇ�́A�Ƃ΂�
						continue;
					if (field.a2.y + l < 0 || field.a2.y + l > field.height)	//�̈�˔j�����ꍇ�́A�Ƃ΂�
						continue;
					field.a2.x += k;
					field.a2.y += l;
					field.state[field.a2.x + k][field.a2.x + l] = 1;
					val += value(field);

					values.push_back(val);
				}
			}
		}
	}

	return values;
}

///<summary>
///�㉺���E�Ɏ��w�̃}�X�����邩�T��
///�Ȃ���΁A���̃}�X�Ɉړ����āA�ċA�I�ɒT��
///�h��Ԃ����ł���΁A�͂܂�Ă���
///�ǂɒB����ƁA�͂܂�Ă��Ȃ�
///
///**** �S�}�X��������T�����Ă��邩��A���P����K�v�A���I�I�I ****
///**** �������ɓ��̈����������@ ****
///**** �u�������v���Q�Ƃ���Ɨǂ����� ****
///</summary>
int value(const struct board_template & field)
{
	int value = 0;
	int tmp_num1, tmp_num2;	//�z���g�Ɉꎞ�ۑ��p
	
	std::vector<std::vector<int>> inclose;	//�͂��Ȃ�:0 ���w���͂�:1 �G�w���͂�:2 ���w���G�w���͂�:3
	std::vector<std::vector<int>> tmp;		//�ꎞ�L���p�@�͂��Ȃ�:0 ���w���͂�:1 �G�w���͂�:2 ���w���G�w���͂�:3

	for (int i = 0; i < field.width; i++) {
		inclose.push_back(std::vector<int>());	//int�^�z��̗v�f��int�^�z����i�[���āA��d�z��ɂ���
		tmp.push_back(std::vector<int>());
		for (int j = 0; j < field.height; j++) {
			inclose[i].push_back(0);
			tmp[i].push_back(0);
		}
	}

	//���ォ��A�S�}�X���N�_�ɒ���
	for (int i = 0; i < field.width; i++) {
		for (int j = 0; j < field.height; j++) {
			//if (inclose[i][j] > 0) continue;	//���łɈ͂���Ƃ킩���Ă���̂Ȃ�A�Ƃ΂� <= �d�����l����ƁA�ۗ�
			//inclose[i][j] = inclose_check(field, i, j, 1, tmp);	//�͂����d�����Ă���̂��l�����Ȃ��Ƃ����Ȃ��I�I�I<=��őΉ�
			tmp_num1 = inclose_check(field, i, j, 1, tmp);
			tmp_num2 = inclose_check(field, i, j, 2, tmp);
			if (tmp_num1 == 0 && tmp_num2 == 0)
				inclose[i][j] = 0;
			else if (tmp_num1 == 1 && tmp_num2 == 0)
				inclose[i][j] = 1;
			else if (tmp_num1 == 0 && tmp_num2 == 1)
				inclose[i][j] = 2;
			else if (tmp_num1 == 1 && tmp_num2 == 1)
				inclose[i][j] = 3;
			/*
			if (inclose[i][j] == 1)	//�͂܂�Ă���
			{
				//tmp��1��inclose�ɔ��f������
				//�T���ς݂̌��ʂ�ۑ����邱�ƂŁA�v�Z�ʍ팸
				for (int k = 0; k < field.width; k++) {
					for (int l = 0; l < field.height; l++) {
						if (tmp[k][l] == 1) {
							inclose[k][l] = tmp[k][l];
						}
					}
				}
			}
			

			for (int k = 0; k < field.width; k++) {
				for (int l = 0; l < field.height; l++) {
					tmp[k][l] = 0;	//������
				}
			}
			*/
		}
	}

	/*
	�̈�|�C���g�ƃ^�C���|�C���g�̍��Z
	*/
	for (int i = 0; i < field.width; i++) {
		for (int j = 0; j < field.height; j++) {
			if (field.state[i][j] == 1)	//���w
				value += field.value[i][j];
			else if (field.state[i][j] == 2)//�G�w
				value -= field.value[i][j];

			if (inclose[i][j] == 1)			//�͂܂�Ă���
				value += field.value[i][j];

			//***�G�w�̈͂��ɂ��Ă͍l�����Ă��Ȃ�***
		}
	}

	return value;
}

///tmp�́A�w�肳�ꂽ�_���͂܂�Ă���ꍇ�́A���̓_�̏󋵂��L�^����Ă��邽�߁A�L�p
///�͂܂�Ă��Ȃ��ꍇ�́A���p

//���w���G�w�̂ǂ��炪�͂��Ă���̂��A��ʂ�����
//* �ǂ̑��݂�state�����āA���ۂɈ͂��������̂́Atmp�̂ق�
//* �ǂ́A���w�ƓG�w��2�ʂ�Ōv�Z = > ���Ԃ͑��v����...�H�킩���B
int inclose_check(const struct board_template & field, int x, int y, int team, std::vector<std::vector<int>> & tmp)
{
	//�̈��˔j������A�͂��Ȃ����Ƃ��m��
	//���Ƃ��Ƃ̔z��ŁA�O���������̐����ň͂�ł���������
	if (x < 0 || x > field.width || y < 0 || y > field.height)
		return 0;

	int another_team;	//team�̔���

	if (team == 1)
		another_team = 2;
	else
		another_team = 1;

	tmp[x][y] = team;

	if(field.state[x-1][y] == 0 || field.state[x - 1][y] == another_team)
		inclose_check(field, x - 1, y, team, tmp);
	if (field.state[x + 1][y] == 0 || field.state[x + 1][y] == another_team)
		inclose_check(field, x + 1, y, team, tmp);
	if (field.state[x][y-1] == 0 || field.state[x][y - 1] == another_team)
		inclose_check(field, x, y - 1, team, tmp);
	if (field.state[x][y+1] == 0 || field.state[x][y + 1] == another_team)
		inclose_check(field, x, y + 1, team, tmp);

	return 1;
}
