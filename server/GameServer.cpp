#include "GameServer.h"

// �T�[�o�[�I�u�W�F�N�g�̏����ݒ�
// ������NULL��n���ƁA�K���Ȓl���g�p�����
void GameServer::init(int row_, int column_, int turn_) {
	row = (row_ == NULL) ? 10 : row_;
	row = (column_ == NULL) ? 10 : column_;
	turn = (turn_ == NULL) ? 5 : turn_;
	// �^�C���̐���
	field = new Tile*[row];
	for (int i = 0; i < row; i++) {
		field[i] = new Tile[column];
	}

	// �G�[�W�F���g�̏�����
	agent[0].state = 1;
	agent[1].state = 1;
	agent[2].state = 2;
	agent[3].state = 2;

	agent[0].id = 0;
	agent[1].id = 1;
	agent[2].id = 2;
	agent[3].id = 3;

	// �G�[�W�F���g�̏����ʒu�̃^�C���̐ݒ�
	for (int i = 0; i < 4; i++) {
		field[agent[i].posx][agent[i].posy].state = agent[i].state;
	}

	isGame = true;
}

// ���[�v���ł��̃��\�b�h���Ăяo��������
// ������true��n�����Ƃ�1�^�[���o�߂���
void GameServer::server(bool transition_turn) {
	// �Q�[���I�����̏���
	if (transition_turn) {
		transition();
	}
	if (turn == 0) {
		isGame = false;
		game_score team1_score = count_score(1);
		game_score team2_score = count_score(2);
	}
}

// �^�[���ڍs
// �G�[�W�F���g���s������
void GameServer::transition() {
	// �G�[�W�F���g�̈ړ��̏���

	for (int i = 0; i < 4; i++) {
		if (agent[i].step == N_A) {
			this->move_agent(i, STAY, false);
		}
	}

	bool can_move[] = { true, true, true, true };

	//�G�[�W�F���g���m�̊����Ȃ����`�F�b�N
	for (int i = 0; i < 3; i++) {
		interference_agent(can_move);
	}

	for (int i = 0; i < 4; i++) {
		if (!can_move[i]) {
			agent[i].next_posx = agent[i].posx;
			agent[i].next_posy = agent[i].posy;
		}
	}

	// �^�C������
	for (int i = 0; i < 4; i++) {
		int _state = field[agent[i].next_posx][agent[i].next_posy].state;
		if (_state != 0 && _state != agent[i].state) {
			field[agent[i].next_posx][agent[i].next_posy].state = 0;
			agent[i].next_posx = agent[i].posx;
			agent[i].next_posy = agent[i].posy;
		}
		if (agent[i].isRemoveTile && can_move[i]) {
			field[agent[i].posx][agent[i].posy].state = 0;
			agent[i].next_posx = agent[i].posx;
			agent[i].next_posy = agent[i].posy;
		}
		agent[i].isRemoveTile = false;
	}

	// �ĂуG�[�W�F���g���m�̊����`�F�b�N
	interference_agent(can_move);

	for (int i = 0; i < 4; i++) {
		if (!can_move[i]) {
			agent[i].next_posx = agent[i].posx;
			agent[i].next_posy = agent[i].posy;
		}
	}

	// �G�[�W�F���g�̈ړ�
	for (int i = 0; i < 4; i++) {
		agent[i].posx = agent[i].next_posx;
		agent[i].posy = agent[i].next_posy;
	}

	// �㏈��
	for (int i = 0; i < 4; i++) {
		agent[i].next_step = N_A;
	}

	turn--;

}

// �G�[�W�F���g�̈ړ�������ݒ肷��
// �Ԃ�l�͈����ŗ^����ꂽ�����Ɉړ����ł��邩�ǂ���
bool GameServer::move_agent(int agent_id, Step step, bool isRemoveTile_) {
	agent[agent_id].isRemoveTile = isRemoveTile_;
	switch (step)
	{
	case N:
		if (agent[agent_id].posy != 0) {
			agent[agent_id].next_step = step;
			agent[agent_id].next_posx = agent[agent_id].posx + 0;
			agent[agent_id].next_posy = agent[agent_id].posy - 1;
			return true;
		}
		else
			return false;
		break;
	case NE:
		if (agent[agent_id].posy != 0 && agent[agent_id].posx != (row - 1)) {
			agent[agent_id].next_step = step;
			agent[agent_id].next_posx = agent[agent_id].posx + 1;
			agent[agent_id].next_posy = agent[agent_id].posy - 1;
			return true;
		}
		else
			return false;
		break;
	case E:
		if (agent[agent_id].posx != (row - 1)) {
			agent[agent_id].next_step = step;
			agent[agent_id].next_posx = agent[agent_id].posx + 1;
			agent[agent_id].next_posy = agent[agent_id].posy + 0;
			return true;
		}
		else
			return false;
		break;
	case SE:
		if (agent[agent_id].posx != (row - 1) && agent[agent_id].posy != (column - 1)) {
			agent[agent_id].next_step = step;
			agent[agent_id].next_posx = agent[agent_id].posx + 1;
			agent[agent_id].next_posy = agent[agent_id].posy + 1;
			return true;
		}
		else
			return false;
		break;
	case S:
		if (agent[agent_id].posy != (column - 1)) {
			agent[agent_id].next_step = step;
			agent[agent_id].next_posx = agent[agent_id].posx + 0;
			agent[agent_id].next_posy = agent[agent_id].posy + 1;
			return true;
		}
		else
			return false;
		break;
	case SW:
		if (agent[agent_id].posx != 0 && agent[agent_id].posy != (column - 1)) {
			agent[agent_id].next_step = step;
			agent[agent_id].next_posx = agent[agent_id].posx - 1;
			agent[agent_id].next_posy = agent[agent_id].posy + 1;
			return true;
		}
		else
			return false;
		break;
	case W:
		if (agent[agent_id].posx != 0) {
			agent[agent_id].next_step = step;
			agent[agent_id].next_posx = agent[agent_id].posx - 1;
			agent[agent_id].next_posy = agent[agent_id].posy + 0;
			return true;
		}
		else
			return false;
		break;
	case NW:
		if (agent[agent_id].posx != 0 && agent[agent_id].posy != 0) {
			agent[agent_id].next_step = step;
			agent[agent_id].next_posx = agent[agent_id].posx - 1;
			agent[agent_id].next_posy = agent[agent_id].posy - 1;
			return true;
		}
		else
			return false;
		break;
	case STAY:
		agent[agent_id].next_step = step;
		agent[agent_id].next_posx = agent[agent_id].posx;
		agent[agent_id].next_posy = agent[agent_id].posy;
		return true;
		break;
	default:
		return false;
		break;
	}
}

// �G�[�W�F���g���m���ړ���Ŋ����邩���`�F�b�N����A���ʂ�can_move�ɕۑ������
void GameServer::interference_agent(bool can_move[4]) {
	if (agent[0].posx == agent[1].posx && agent[0].posy == agent[1].posy) {
		can_move[0] = false; can_move[1] = false;
	}
	if (agent[0].posx == agent[2].posx && agent[0].posy == agent[2].posy) {
		can_move[0] = false; can_move[2] = false;
	}
	if (agent[0].posx == agent[3].posx && agent[0].posy == agent[3].posy) {
		can_move[0] = false; can_move[3] = false;
	}
	if (agent[1].posx == agent[2].posx && agent[1].posy == agent[2].posy) {
		can_move[1] = false; can_move[2] = false;
	}
	if (agent[1].posx == agent[3].posx && agent[1].posy == agent[3].posy) {
		can_move[1] = false; can_move[3] = false;
	}
	if (agent[2].posx == agent[3].posx && agent[2].posy == agent[3].posy) {
		can_move[2] = false; can_move[3] = false;
	}
}

// ���_�v�Z
game_score GameServer::count_score(int _state) {
	bool * * is_field_end = new bool*[row];
	for (int i = 0; i < row; i++) {
		is_field_end[i] = new bool[column];
	}
	bool * * is_searched = new bool*[row];
	for (int i = 0; i < row; i++) {
		is_searched[i] = new bool[column];
	}

	// �؂�ݒ�
	for (int x = 0; x < row; x++) {
		for (int y = 0; y < column; y++) {
			is_field_end[x][y] = false;
			if (x == 0 || y == 0 || x == row - 1 || y == column - 1) {
				is_field_end[x][y] = true;
			}
		}
	}
	// �^�C���|�C���g���v�Z
	int tile_point = 0;
	for (int x = 0; x < row; x++) {
		for (int y = 0; y < column; y++) {
			if (field[x][y].state == _state) {
				is_searched[x][y] = true;
				tile_point += field[x][y].point;
			}
		}
	}

	// �̈�|�C���g���v�Z
	for (int x = 0; x < row; x++) {
		for (int y = 0; y < column; y++) {
			if (field[x][y].state != _state && !is_field_end[x][y] && !is_searched[x][y]) {
				bool success = true;
				int sco = 0;
				std::queue<int> qux;
				std::queue<int> quy;
				qux.push(x);
				quy.push(y);
				is_searched[qux.front()][quy.front()] = true;
				while (true) {
					if (qux.empty()) {
						if (success) {
							//result += sco;
						}
						break;
					}

					sco += field[qux.front()][quy.front()].point >= 0 ? field[qux.front()][quy.front()].point : -field[qux.front()][quy.front()].point;

					if (is_field_end[qux.front()][quy.front()]) {
						qux.pop();
						quy.pop();
						success = false;
						continue;
					}

					// x+1�̏ꍇ
					if (field[qux.front() + 1][quy.front()].state != _state && !is_searched[qux.front() + 1][quy.front()]) {
						qux.push(qux.front() + 1);
						quy.push(quy.front());
						is_searched[qux.front() + 1][quy.front()] = true;
					}

					// x-1�̏ꍇ
					if (field[qux.front() - 1][quy.front()].state != _state && !is_searched[qux.front() - 1][quy.front()]) {
						qux.push(qux.front() - 1);
						quy.push(quy.front());
						is_searched[qux.front() - 1][quy.front()] = true;
					}

					// y+1�̏ꍇ
					if (field[qux.front()][quy.front() + 1].state != _state && !is_searched[qux.front()][quy.front() + 1]) {
						qux.push(qux.front());
						quy.push(quy.front() + 1);
						is_searched[qux.front()][quy.front() + 1] = true;
					}

					// y-1�̏ꍇ
					if (field[qux.front()][quy.front() - 1].state != _state && !is_searched[qux.front()][quy.front() - 1]) {
						qux.push(qux.front());
						quy.push(quy.front() - 1);
						is_searched[qux.front()][quy.front() - 1] = true;
					}
				}
			}
		}
	}
}