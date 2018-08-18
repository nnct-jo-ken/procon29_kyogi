#include "search.h"
#include <vector>

int minimax(struct board_template & field, int depth)
{
	int score = 0;
	int tmp1, tmp2;	//�ړ���̈ړ��O�̏�Ԃ̕ۑ�

	if (depth == 0) return value(field);	//���[�܂ŒB������A���̃{�[�h�̕]���l��Ԃ�

	if (depth % 2 == 0)	//���w
	{
		int max = -1000;	//�Ƃ肠���������Ȓl�ɂ���

		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				for (int k = -1; k <= 1; k++) {
					for (int l = -1; l <= 1; l++) {
						if (field.own_a1.x + i < 1 || field.own_a1.x + i > field.width)	//�̈�˔j�����ꍇ�́A�Ƃ΂�
							continue;
						if (field.own_a1.y + j < 1 || field.own_a1.y + j > field.height)
							continue;
						if (field.own_a2.x + k < 1 || field.own_a2.x + k > field.width)
							continue;
						if (field.own_a2.y + l < 1 || field.own_a2.y + l > field.height)
							continue;

						//field�����̎�ɕω�������
						field.own_a1.x += i;
						field.own_a1.y += j;
						tmp1 = field.state[field.own_a1.x -1][field.own_a1.y -1];
						field.state[field.own_a1.x -1][field.own_a1.y -1] = 1;

						field.own_a2.x += k;
						field.own_a2.y += l;
						tmp2 = field.state[field.own_a2.x -1][field.own_a2.y -1];
						field.state[field.own_a2.x -1][field.own_a2.y -1] = 1;

						score = minimax(field, depth - 1);
						if (score > max) max = score;

						//field�����ɖ߂�
						field.own_a1.x -= i;
						field.own_a1.y -= j;
						field.state[field.own_a1.x -1][field.own_a1.y -1] = tmp1;

						field.own_a2.x -= k;
						field.own_a2.y -= l;
						field.state[field.own_a2.x -1][field.own_a2.y -1] = tmp2;
					}
				}
			}
		}
		return max;
	}
	else	//�G�w
	{		
		int min = 1000;	//�Ƃ肠�����傫���l�ɂ���

		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				for (int k = -1; k <= 1; k++) {
					for (int l = -1; l <= 1; l++) {
						if (field.own_a1.x + i < 1 || field.own_a1.x + i > field.width)	//�̈�˔j�����ꍇ�́A�Ƃ΂�
							continue;
						if (field.own_a1.y + j < 1 || field.own_a1.y + j > field.height)
							continue;
						if (field.own_a2.x + k < 1 || field.own_a2.x + k > field.width)
							continue;
						if (field.own_a2.y + l < 1 || field.own_a2.y + l > field.height)
							continue;

						//field�����̎�ɕω�������
						field.own_a1.x += i;
						field.own_a1.y += j;
						tmp1 = field.state[field.own_a1.x -1][field.own_a1.y -1];
						field.state[field.own_a1.x -1][field.own_a1.y -1] = 1;

						field.own_a2.x += k;
						field.own_a2.y += l;
						tmp2 = field.state[field.own_a2.x -1][field.own_a2.y -1];
						field.state[field.own_a2.x -1][field.own_a2.y -1] = 1;

						score = minimax(field, depth - 1);
						if (score < min) min = score;

						//field�����ɖ߂�
						field.own_a1.x -= i;
						field.own_a1.y -= j;
						field.state[field.own_a1.x -1][field.own_a1.y -1] = tmp1;

						field.own_a2.x -= k;
						field.own_a2.y -= l;
						field.state[field.own_a2.x -1][field.own_a2.y -1] = tmp2;
					}
				}
			}
		}
		return min;
	}
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

	//���ォ��A�S�}�X���N�_�Ɉ͂܂�Ă��邩����
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
	
	//*** debug begin ***
	//�]���l�����߂邽�߂Ƀt�B�[���h�����������Ă��邩��A���ۂ̏󋵂Ƃ͈Ⴄ
	putchar('\n');
	printf("inclose\n");
	for (int i = 0; i < field.width; i++) {
		for (int j = 0; j < field.height; j++) {
			printf("%3d", inclose[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');

	putchar('\n');
	printf("state\n");
	for (int i = 0; i < field.width; i++) {
		for (int j = 0; j < field.height; j++) {
			printf("%3d", field.state[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');
	//*** debug end ***

	/*
	�̈�|�C���g�ƃ^�C���|�C���g�̍��Z
	*/
	for (int i = 0; i < field.width; i++) {
		for (int j = 0; j < field.height; j++) {
			if (field.state[i][j] == 1)	//���w
				value += field.value[i][j];
			else if (field.state[i][j] == 2)//�G�w
				value -= field.value[i][j];

			//if (inclose[i][j] == 1)			//�͂܂�Ă���
			//	value += field.value[i][j];

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
int inclose_check(const struct board_template & field, int x, int y, int team,std::vector<std::vector<int>> & tmp)
{
	//�t�B�[���h�̒[�ɂ��鎞�_�ŁA�͂��Ȃ�
	if (x < 1 || x > field.width-2 || y < 1 || y > field.height-2)
		return 0;

	if (tmp[x][y] == team)	//�T���ς�
		return 1;

	int another_team;	//team�̔���
	int val;

	if (team == 1)
		another_team = 2;
	else
		another_team = 1;

	tmp[x][y] = team;

	if (field.state[x - 1][y] == 0 || field.state[x - 1][y] == another_team) {
		val = inclose_check(field, x - 1, y, team, tmp);
		if (val == 0) return 0;	//�͂��Ȃ��Ƃ킩������A�㗬�܂�0��Ԃ��Ă���
	}
	if (field.state[x + 1][y] == 0 || field.state[x + 1][y] == another_team) {
		val = inclose_check(field, x + 1, y, team, tmp);
		if (val == 0) return 0;
	}
	if (field.state[x][y - 1] == 0 || field.state[x][y - 1] == another_team) {
		val = inclose_check(field, x, y - 1, team, tmp);
		if (val == 0) return 0;
	}
	if (field.state[x][y + 1] == 0 || field.state[x][y + 1] == another_team) {
		val = inclose_check(field, x, y + 1, team, tmp);
		if (val == 0) return 0;
	}
		
	return 1;
}
