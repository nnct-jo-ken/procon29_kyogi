#include <iostream>
#include <string>
#include <random>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <thread>
#include <atomic>
#include "Procon29Game.h"
#include "rand_board.h"

char end_serial[] = "end";

bool isGame = true;
Tile field[12][12];
Agent agent[4];
int row = 0;
int column = 0;
int turn = 0;

std::atomic<bool> start_game;
std::atomic<bool> check_send_id = true;


void init(int, int, int);
void server(bool);
game_score count_score(int);
void interference_agent(bool[4]);
bool move_agent(int, Step, bool);
void transition();
void buff_to_data(char[6][600]);

// �T�[�o�[�̏�����
// ������NULL��n���ƁA�K���Ȓl���g�p�����
void init(int row_, int column_, int turn_) {
	row = (row_ == NULL) ? 10 : row_;
	column = (column_ == NULL) ? 10 : column_;
	turn = (turn_ == NULL) ? 5 : turn_;
	// �^�C���̐���
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			// �͈͓��̃^�C��
			if (i < row && j < column) {
				field[i][j].point = 1;
				field[i][j].state = 0;
			}
			// �͈͊O�̃^�C��
			else {
				field[i][j].point = -10000;
				field[i][j].state = 0;
			}
		}
	}

	// �G�[�W�F���g�̏�����
	agent[0].state = 1;
	agent[1].state = 1;
	agent[2].state = 2;
	agent[3].state = 2;

	// �G�[�W�F���g�̏����ʒu�̃^�C���̐ݒ�
	for (int i = 0; i < 4; i++) {
		field[agent[i].posx][agent[i].posy].state = agent[i].state;
	}

	isGame = true;
}

// ���[�v���ł��̃��\�b�h���Ăяo��������
// ������true��n�����A���ׂẴG�[�W�F���g���s�������肷���1�^�[���o�߂���
void server(bool transition_turn = false) {
	if (transition_turn || (agent[0].state != N_A && agent[1].state != N_A && agent[2].state != N_A && agent[3].state != N_A)) {
		transition();
	}

	// �Q�[���I�����̏���
	if (turn == 0) {
		isGame = false;
		struct game_score team1_score = count_score(1);
		struct game_score team2_score = count_score(2);
	}
}

// �^�[���ڍs
// �G�[�W�F���g���s������
void transition() {
	// �G�[�W�F���g�̈ړ��̏���

	for (int i = 0; i < 4; i++) {
		if (agent[i].step == N_A) {
			move_agent(i, STAY, false);
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
bool move_agent(int agent_id, Step step, bool isRemoveTile_) {
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
void interference_agent(bool can_move[4]) {
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
game_score count_score(int _state) {
	game_score result;
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
	for (int x = 0; x < row; x++) {
		for (int y = 0; y < column; y++) {
			if (field[x][y].state == _state) {
				is_searched[x][y] = true;
				result.tile_point += field[x][y].point;
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
							result.area_point += sco;
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
	for (int i = 0; i < row; i++) {
		delete[] is_field_end[i];
		delete[] is_searched[i];
		is_field_end[i] = 0;
		is_searched[i] = 0;
	}
	delete[] is_field_end;
	delete[] is_searched;
	is_field_end = 0;
	is_searched = 0;
	return result;
}

// �Ֆʂ̑傫���A�^�[�����������_���Ō��肷��
void create_rand_info() {
	std::random_device rnd;     // �񌈒�I�ȗ���������𐶐�
	std::mt19937 mt(rnd());     //  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
	std::uniform_int_distribution<int> rand_size(8, 12);        // [0, 99] �͈͂̈�l����
	std::uniform_int_distribution<int> rand_turn(40, 60);

	row = rand_size(mt);
	column = rand_size(mt);
	turn = rand_turn(mt);
	int field_point[12][12];
	// ���_����
	init::rand_board(field_point, row, column);
	// ���_�i�[
	for (int y = 0; y < column; y++) {
		for (int x = 0; x < row; x++) {
			field[x][y].point = field_point[x][y];
		}
	}
	
	// �G�[�W�F���g�̏����ʒu������
	std::uniform_int_distribution<int> rand_x(0, (int)(row / 2) - 1);
	std::uniform_int_distribution<int> rand_y(0, (int)(column / 2) - 1);
	agent[0].posx = rand_x(mt);
	agent[0].posy = rand_y(mt);
	agent[1].posx = row - agent[0].posx - 1;
	agent[1].posy = column - agent[0].posy - 1;
	agent[2].posx = agent[0].posx;
	agent[2].posy = column - agent[0].posy - 1;
	agent[3].posx = row - agent[0].posx - 1;
	agent[3].posy = agent[0].posy;
	agent[0].state = 1;
	agent[1].state = 1;
	agent[2].state = 2;
	agent[3].state = 2;
}

// �P�̃N���C�A���g���ڑ�����
// ��������񂪂���ꍇ�͎󂯎��Ȃ��ꍇ�́A�T�[�o�[����������
int init_board_connect(char bib[6][600]) {
	WSADATA wsaData;
	SOCKET sock0;
	SOCKET sock;
	struct sockaddr_in addr;
	int len;
	struct sockaddr_in client;

	char buf[32];

	// winsock2�̏�����
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// �\�P�b�g�̍쐬
	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	// �\�P�b�g�̐ݒ�
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

	// �ڑ���҂Ă��Ԃɂ���
	listen(sock0, 5);
	len = sizeof(client);
	sock = accept(sock0, (struct sockaddr *)&client, &len);
	memset(buf, 0, sizeof(buf));

	// �N���C�A���g���Ֆʏ��������𑗐M���邩�󂯎��
	recv(sock, buf, sizeof(buf), 0);

	// char[] -> string�ϊ�
	std::string is_get_board(buf, 1);


	// �Ֆʏ��̏����������邩���󂯎��
	if (std::stoi(is_get_board)) {
		std::cout << "true" << std::endl;
		for (int i = 0; i < 6; i++) {
			recv(sock, bib[i], sizeof(bib[i]), 0);

			// �^�C�~���O���킹
			if (send(sock, end_serial, sizeof(end_serial), 0) == SOCKET_ERROR) {
				std::cerr << WSAGetLastError() << std::endl;
			}
		}

		// ��M�����f�[�^����Tile,Agent�֕ϊ�
		buff_to_data(bib);
		std::cout << "row:" << row << " column:" << column << " turn:" << turn << std::endl;
		for (int y = 0; y < column; y++) {
			for (int x = 0; x < row; x++) {
				std::cout << field[x][y].point << " ";
			}
			std::cout << std::endl;
		}
		for (int i = 0; i < 4; i++) {
			std::cout << agent[i].posx << " " << agent[i].posy << " " << agent[i].state << std::endl;
		}
	}
	else {
		/*
		//
		//�����ŃT�[�o�[�������_���ŔՖʂ𐶐�����֐����Ă�
		//
		*/
		std::cout << "�Ֆʏ��𐶐����܂�..." << std::endl;
		create_rand_info();
	}

	// �Z�b�V�������I��
	closesocket(sock);
	// winsock2�̏I������
	WSACleanup();

	return 0;
}

// �N���C�A���g���瑗���Ă����Ֆʏ��o�b�t�@���e�I�u�W�F�N�g�̃����o�ɕϊ�����
// ��������p
void buff_to_data(char buff[6][600]) {
	std::string strs;
	// row�����o��
	int i = 0;
	while (true) {
		strs += buff[0][i];
		i++;
		if (buff[0][i] == ' ' || buff[0][i] == '\0') {
			row = std::stoi(strs);
			break;
		}
	}
	// column�����o��
	i++;
	strs = "";
	while (true) {
		strs += buff[0][i];
		i++;
		if (buff[0][i] == ' ' || buff[0][i] == '\0') {
			column = std::stoi(strs);
			break;
		}
	}
	// turn�����o��
	i++;
	strs = "";
	while (true) {
		strs += buff[0][i];
		i++;
		if (buff[0][i] == ' ' || buff[0][i] == '\0') {
			turn = std::stoi(strs);
			break;
		}
	}

	// �Ֆʂ̓��_�����o��
	i = 0;
	for (int y = 0; y < column; y++) {
		for (int x = 0; x < row; x++) {
			strs = "";
			while (true) {
				strs += buff[1][i];
				i++;
				if (buff[1][i] == ' ' || buff[1][i] == '\0') {
					field[x][y].point = std::stoi(strs);
					i++;
					break;
				}
			}
		}
	}

	// �e�G�[�W�F���g�̏������o��
	for (int j = 0; j < 4; j++) {
		i = 0;
		strs = "";
		// x���W�����o��
		while (true) {
			strs += buff[2 + j][i];
			i++;
			if (buff[2 + j][i] == ' ' || buff[2 + j][i] == '\0') {
				agent[j].posx = std::stoi(strs);
				i++;
				break;
			}
		}
		strs = "";
		// y���W�����o��
		while (true) {
			strs += buff[2 + j][i];
			i++;
			if (buff[2 + j][i] == ' ' || buff[2 + j][i] == '\0') {
				agent[j].posy = std::stoi(strs);
				i++;
				break;
			}
		}
		strs = "";
		// state�����o��
		while (true) {
			strs += buff[2 + j][i];
			i++;
			if (buff[2 + j][i] == ' ' || buff[2 + j][i] == '\0') {
				agent[j].state = std::stoi(strs);
				i++;
				break;
			}
		}
	}
}

// �Ֆʏ���char[][]�ɕϊ�����
void board_data_to_buff(char buff[8][600]) {
	std::string str;
	str += std::to_string(row);
	str += ' ';
	str += std::to_string(column);
	for (int i = 0;; i++) {
		buff[0][i] = str[i];
		if (str[i] == '\0') {
			break;
		}
	}

	str = "";
	for (int y = 0; y < column; y++) {
		for (int x = 0; x < row; x++) {
			str += std::to_string(field[x][y].point);
			str += ' ';
		}
	}

	for (int i = 0;; i++) {
		buff[1][i] = str[i];
		if (str[i] == '\0') {
			break;
		}
	}

	for (int i = 0; i < 4; i++) {
		str = "";
		str += std::to_string(agent[i].posx);
		str += ' ';
		str += std::to_string(agent[i].posy);
		str += ' ';
		str += std::to_string(agent[i].state);
		for (int j = 0;; j++) {
			buff[2 + i][j] = str[j];
			if (str[j] == '\0') {
				break;
			}
		}
	}

}

// �Ֆʏ����N���C�A���g�֑��M����
// �e�^�[���X�V���Ɏg�p
void send_board_info(SOCKET sock) {
	char board_send_buff[7][600];
	// row column turn�𑗂�
	std::string strs;
	strs += std::to_string(row);
	strs += ' ';
	strs += std::to_string(column);
	strs += ' ';
	strs += std::to_string(turn);
	for (int i = 0;; i++) {
		if (strs[i] == '\0') {
			board_send_buff[0][i] = '\0';
			break;
		}
		board_send_buff[0][i] = strs[i];
	}

	// �Ֆʏ��𑗂�
	// ���_
	std::string str;
	for (int y = 0; y < column; y++) {
		for (int x = 0; x < row; x++) {
			str += std::to_string(field[x][y].point);
			str += ' ';
		}
	}

	for (int i = 0;; i++) {
		board_send_buff[1][i] = str[i];
		if (str[i] == '\0') {
			break;
		}
	}
	// �^�C����state
	str = "";
	for (int y = 0; y < column; y++) {
		for (int x = 0; x < row; x++) {
			str += std::to_string(field[x][y].state);
			str += ' ';
		}
	}

	for (int i = 0;; i++) {
		board_send_buff[2][i] = str[i];
		if (str[i] == '\0') {
			break;
		}
	}

	// �e�G�[�W�F���g�̏��
	for (int i = 0; i < 4; i++) {
		str = "";
		str += std::to_string(agent[i].posx);
		str += ' ';
		str += std::to_string(agent[i].posy);
		str += ' ';
		str += std::to_string(agent[i].state);
		for (int j = 0;; j++) {
			board_send_buff[3 + i][j] = str[j];
			if (str[j] == '\0') {
				break;
			}
		}
	}
	// �Ֆʏ��𑗂�
	for (int i = 0; i < 7; i++) {
		if (send(sock, board_send_buff[i], sizeof(board_send_buff[i]), 0) == SOCKET_ERROR) {
			std::cerr << WSAGetLastError() << std::endl;
		}
		char cache[20];
		recv(sock, cache, sizeof(cache), 0);
	}
}

// ���C���̑���M����
void clinet_connection() {
	WSADATA wsaData;
	SOCKET sock0;
	SOCKET sock;
	struct sockaddr_in addr;
	int len;
	struct sockaddr_in client;

	char buf[600];

	// winsock2�̏�����
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// �\�P�b�g�̍쐬
	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	// �\�P�b�g�̐ݒ�
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

	// �ڑ���҂Ă��Ԃɂ���
	listen(sock0, 5);
	len = sizeof(client);
	sock = accept(sock0, (struct sockaddr *)&client, &len);

	memset(buf, 0, sizeof(buf));

	// �N���C�A���g�̎�ނ��󂯎��
	// �v���C���[���P�l�̏ꍇ(1), �Q�l�̏ꍇ(2)���󂯎��
	
	recv(sock, buf, sizeof(buf), 0);
	if (send(sock, end_serial, sizeof(end_serial), 0) == SOCKET_ERROR) {
		std::cerr << WSAGetLastError() << std::endl;
	}

	// char[] -> string�ϊ�
	std::string kind(buf, 1);
	int kindi = std::stoi(kind);
	
	std::cout << "kindi:" << kindi << std::endl;

	// ���삷��v���C���[��id(�z��̃C���f�b�N�X)�𑗂�
	std::string index_buff;
	if (kindi == 2) {
		index_buff = "0 1 2 3\0";
	}
	else if (kindi == 1) {
		if (check_send_id) {
			index_buff = "0 1\0";
		}
		else {
			index_buff = "2 3\0";
		}
	}
	else if (kindi == 0) {
		index_buff = "-1\0";
	}


	char send_id_buff[10];
	for (int i = 0;;i++) {
		if (index_buff[i] == '\0') {
			send_id_buff[i] = '\0';
			break;
		}
		send_id_buff[i] = index_buff[i];
	}

	// �N���C�A���g�֑��삷�ׂ�agent��id�𑗂�
	if (send(sock, send_id_buff, sizeof(send_id_buff), 0) == SOCKET_ERROR) {
		std::cerr << WSAGetLastError() << std::endl;
	}
	char cache[20];
	recv(sock, cache, sizeof(cache), 0);

	//while (turn >= 1) {
		// �Ֆʏ��𑗂�
		send_board_info(sock);

		// �G�[�W�F���g�̈ړ�����󂯎��

	//}

	// �Z�b�V�������I��
	closesocket(sock);
	// winsock2�̏I������
	WSACleanup();
}

int main() {

	// �Ֆʏ���������M�pbuffer
	char board_info_buff[6][600];

	// �Ֆʂ̏����ݒ�
	std::thread connect1(init_board_connect, board_info_buff);
	connect1.join();

	std::thread connect2(clinet_connection);
	connect2.join();

	return 0;
}