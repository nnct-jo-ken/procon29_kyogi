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

// サーバーの初期化
// 引数にNULLを渡すと、適当な値が使用される
void init(int row_, int column_, int turn_) {
	row = (row_ == NULL) ? 10 : row_;
	column = (column_ == NULL) ? 10 : column_;
	turn = (turn_ == NULL) ? 5 : turn_;
	// タイルの生成
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			// 範囲内のタイル
			if (i < row && j < column) {
				field[i][j].point = 1;
				field[i][j].state = 0;
			}
			// 範囲外のタイル
			else {
				field[i][j].point = -10000;
				field[i][j].state = 0;
			}
		}
	}

	// エージェントの初期化
	agent[0].state = 1;
	agent[1].state = 1;
	agent[2].state = 2;
	agent[3].state = 2;

	// エージェントの初期位置のタイルの設定
	for (int i = 0; i < 4; i++) {
		field[agent[i].posx][agent[i].posy].state = agent[i].state;
	}

	isGame = true;
}

// ループ内でこのメソッドを呼び出し続ける
// 引数にtrueを渡すか、すべてのエージェントが行動を決定すると1ターン経過する
void server(bool transition_turn = false) {
	if (transition_turn || (agent[0].state != N_A && agent[1].state != N_A && agent[2].state != N_A && agent[3].state != N_A)) {
		transition();
	}

	// ゲーム終了時の処理
	if (turn == 0) {
		isGame = false;
		struct game_score team1_score = count_score(1);
		struct game_score team2_score = count_score(2);
	}
}

// ターン移行
// エージェントが行動する
void transition() {
	// エージェントの移動の処理

	for (int i = 0; i < 4; i++) {
		if (agent[i].step == N_A) {
			move_agent(i, STAY, false);
		}
	}

	bool can_move[] = { true, true, true, true };

	//エージェント同士の干渉がないかチェック
	for (int i = 0; i < 3; i++) {
		interference_agent(can_move);
	}

	for (int i = 0; i < 4; i++) {
		if (!can_move[i]) {
			agent[i].next_posx = agent[i].posx;
			agent[i].next_posy = agent[i].posy;
		}
	}

	// タイル除去
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

	// 再びエージェント同士の干渉をチェック
	interference_agent(can_move);

	for (int i = 0; i < 4; i++) {
		if (!can_move[i]) {
			agent[i].next_posx = agent[i].posx;
			agent[i].next_posy = agent[i].posy;
		}
	}

	// エージェントの移動
	for (int i = 0; i < 4; i++) {
		agent[i].posx = agent[i].next_posx;
		agent[i].posy = agent[i].next_posy;
	}

	// 後処理
	for (int i = 0; i < 4; i++) {
		agent[i].next_step = N_A;
	}

	turn--;

}

// エージェントの移動方向を設定する
// 返り値は引数で与えられた方向に移動ができるかどうか
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

// エージェント同士が移動先で干渉するかをチェックする、結果はcan_moveに保存される
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

// 得点計算
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

	// 筋を設定
	for (int x = 0; x < row; x++) {
		for (int y = 0; y < column; y++) {
			is_field_end[x][y] = false;
			if (x == 0 || y == 0 || x == row - 1 || y == column - 1) {
				is_field_end[x][y] = true;
			}
		}
	}
	// タイルポイントを計算
	for (int x = 0; x < row; x++) {
		for (int y = 0; y < column; y++) {
			if (field[x][y].state == _state) {
				is_searched[x][y] = true;
				result.tile_point += field[x][y].point;
			}
		}
	}

	// 領域ポイントを計算
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

					// x+1の場合
					if (field[qux.front() + 1][quy.front()].state != _state && !is_searched[qux.front() + 1][quy.front()]) {
						qux.push(qux.front() + 1);
						quy.push(quy.front());
						is_searched[qux.front() + 1][quy.front()] = true;
					}

					// x-1の場合
					if (field[qux.front() - 1][quy.front()].state != _state && !is_searched[qux.front() - 1][quy.front()]) {
						qux.push(qux.front() - 1);
						quy.push(quy.front());
						is_searched[qux.front() - 1][quy.front()] = true;
					}

					// y+1の場合
					if (field[qux.front()][quy.front() + 1].state != _state && !is_searched[qux.front()][quy.front() + 1]) {
						qux.push(qux.front());
						quy.push(quy.front() + 1);
						is_searched[qux.front()][quy.front() + 1] = true;
					}

					// y-1の場合
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

// 盤面の大きさ、ターン数をランダムで決定する
void create_rand_info() {
	std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<int> rand_size(8, 12);        // [0, 99] 範囲の一様乱数
	std::uniform_int_distribution<int> rand_turn(40, 60);

	row = rand_size(mt);
	column = rand_size(mt);
	turn = rand_turn(mt);
	int field_point[12][12];
	// 得点生成
	init::rand_board(field_point, row, column);
	// 得点格納
	for (int y = 0; y < column; y++) {
		for (int x = 0; x < row; x++) {
			field[x][y].point = field_point[x][y];
		}
	}
	
	// エージェントの初期位置を決定
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

// １つのクライアントが接続する
// 初期化情報がある場合は受け取りない場合は、サーバーが生成する
int init_board_connect(char bib[6][600]) {
	WSADATA wsaData;
	SOCKET sock0;
	SOCKET sock;
	struct sockaddr_in addr;
	int len;
	struct sockaddr_in client;

	char buf[32];

	// winsock2の初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// ソケットの作成
	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	// ソケットの設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

	// 接続を待てる状態にする
	listen(sock0, 5);
	len = sizeof(client);
	sock = accept(sock0, (struct sockaddr *)&client, &len);
	memset(buf, 0, sizeof(buf));

	// クライアントが盤面初期化情報を送信するか受け取る
	recv(sock, buf, sizeof(buf), 0);

	// char[] -> string変換
	std::string is_get_board(buf, 1);


	// 盤面情報の初期化をするかを受け取る
	if (std::stoi(is_get_board)) {
		std::cout << "true" << std::endl;
		for (int i = 0; i < 6; i++) {
			recv(sock, bib[i], sizeof(bib[i]), 0);

			// タイミング合わせ
			if (send(sock, end_serial, sizeof(end_serial), 0) == SOCKET_ERROR) {
				std::cerr << WSAGetLastError() << std::endl;
			}
		}

		// 受信したデータからTile,Agentへ変換
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
		//ここでサーバーがランダムで盤面を生成する関数を呼ぶ
		//
		*/
		std::cout << "盤面情報を生成します..." << std::endl;
		create_rand_info();
	}

	// セッションを終了
	closesocket(sock);
	// winsock2の終了処理
	WSACleanup();

	return 0;
}

// クライアントから送られてきた盤面情報バッファを各オブジェクトのメンバに変換する
// 初期化専用
void buff_to_data(char buff[6][600]) {
	std::string strs;
	// rowを取り出す
	int i = 0;
	while (true) {
		strs += buff[0][i];
		i++;
		if (buff[0][i] == ' ' || buff[0][i] == '\0') {
			row = std::stoi(strs);
			break;
		}
	}
	// columnを取り出す
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
	// turnを取り出す
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

	// 盤面の得点を取り出す
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

	// 各エージェントの情報を取り出す
	for (int j = 0; j < 4; j++) {
		i = 0;
		strs = "";
		// x座標を取り出す
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
		// y座標を取り出す
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
		// stateを取り出す
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

// 盤面情報をchar[][]に変換する
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

// 盤面情報をクライアントへ送信する
// 各ターン更新時に使用
void send_board_info(SOCKET sock) {
	char board_send_buff[7][600];
	// row column turnを送る
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

	// 盤面情報を送る
	// 得点
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
	// タイルのstate
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

	// 各エージェントの情報
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
	// 盤面情報を送る
	for (int i = 0; i < 7; i++) {
		if (send(sock, board_send_buff[i], sizeof(board_send_buff[i]), 0) == SOCKET_ERROR) {
			std::cerr << WSAGetLastError() << std::endl;
		}
		char cache[20];
		recv(sock, cache, sizeof(cache), 0);
	}
}

// メインの送受信処理
void clinet_connection() {
	WSADATA wsaData;
	SOCKET sock0;
	SOCKET sock;
	struct sockaddr_in addr;
	int len;
	struct sockaddr_in client;

	char buf[600];

	// winsock2の初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// ソケットの作成
	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	// ソケットの設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

	// 接続を待てる状態にする
	listen(sock0, 5);
	len = sizeof(client);
	sock = accept(sock0, (struct sockaddr *)&client, &len);

	memset(buf, 0, sizeof(buf));

	// クライアントの種類を受け取る
	// プレイヤーが１人の場合(1), ２人の場合(2)を受け取る
	
	recv(sock, buf, sizeof(buf), 0);
	if (send(sock, end_serial, sizeof(end_serial), 0) == SOCKET_ERROR) {
		std::cerr << WSAGetLastError() << std::endl;
	}

	// char[] -> string変換
	std::string kind(buf, 1);
	int kindi = std::stoi(kind);
	
	std::cout << "kindi:" << kindi << std::endl;

	// 操作するプレイヤーのid(配列のインデックス)を送る
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

	// クライアントへ操作すべきagentのidを送る
	if (send(sock, send_id_buff, sizeof(send_id_buff), 0) == SOCKET_ERROR) {
		std::cerr << WSAGetLastError() << std::endl;
	}
	char cache[20];
	recv(sock, cache, sizeof(cache), 0);

	//while (turn >= 1) {
		// 盤面情報を送る
		send_board_info(sock);

		// エージェントの移動先を受け取る

	//}

	// セッションを終了
	closesocket(sock);
	// winsock2の終了処理
	WSACleanup();
}

int main() {

	// 盤面初期化情報受信用buffer
	char board_info_buff[6][600];

	// 盤面の初期設定
	std::thread connect1(init_board_connect, board_info_buff);
	connect1.join();

	std::thread connect2(clinet_connection);
	connect2.join();

	return 0;
}