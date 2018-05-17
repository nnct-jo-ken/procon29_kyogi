#include <iostream>
#include "GameServer.h"

void GameServer::init(int row_, int column_, int turn_) {
	row = (row_ == NULL) ? 10 : row_;
	row = (column_ == NULL) ? 10 : column_;
	turn = (turn_ == NULL) ? 5 : turn_;
	// タイルの初期化
	field = new Tile*[row];
	for (int i = 0; i < row; i++) {
		field[i] = new Tile[column];
	}

	// エージェントの初期化
	agent[0].state = A;
	agent[1].state = A;
	agent[2].state = B;
	agent[3].state = B;

	// エージェントの初期位置のタイルの設定
	for (int i = 0; i < 4; i++) {
		field[agent[i].posx][agent[i].posy].state = agent[i].state;
		field[agent[i].posx][agent[i].posy].point = 0;
	}
}

void GameServer::server() {
	// 全てのエージェントが行動する方向を決定したか
	int ready = 0;
	for (int i = 0; i < 4; i++) {
		if (agent[i].next_step != N_A) {
			ready++;
		}
	}
	// エージェントの移動の処理
	if (ready == 4) {
		bool can_move[] = { true, true, true, true };

		//エージェント同士の干渉がないかチェック ここから
		for (int i = 0; i < 3; i++) {
			if (agent[0].next_posx == agent[1].next_posx && agent[0].next_posy == agent[1].next_posy) {
				can_move[0] = false; can_move[1] = false;
			}
			if (agent[0].next_posx == agent[2].next_posx && agent[0].next_posy == agent[2].next_posy) {
				can_move[0] = false; can_move[2] = false;
			}
			if (agent[0].next_posx == agent[3].next_posx && agent[0].next_posy == agent[3].next_posy) {
				can_move[0] = false; can_move[3] = false;
			}
			if (agent[1].next_posx == agent[2].next_posx && agent[1].next_posy == agent[2].next_posy) {
				can_move[1] = false; can_move[2] = false;
			}
			if (agent[1].next_posx == agent[3].next_posx && agent[1].next_posy == agent[3].next_posy) {
				can_move[1] = false; can_move[3] = false;
			}
			if (agent[2].next_posx == agent[3].next_posx && agent[2].next_posy == agent[3].posy) {
				can_move[2] = false; can_move[3] = false;
			}
		}

		for (int i = 0; i < 4; i++) {
			if (!can_move[i]) {
				agent[i].next_posx = agent[i].posx;
				agent[i].next_posy = agent[i].posy;
			}
		}

		// タイル除去
		for (int i = 0; i < 4; i++) {
			State s = field[agent[i].next_posx][agent[i].next_posy].state;
			if (s != NEUTRAL && s != agent[i].state) {
				field[agent[i].next_posx][agent[i].next_posy].state = NEUTRAL;
				agent[i].next_posx = agent[i].posx;
				agent[i].next_posy = agent[i].posy;
			}
			if (agent[i].isRemoveTile && can_move[i]) {
				field[agent[i].posx][agent[i].posy].state = NEUTRAL;
				agent[i].next_posx = agent[i].posx;
				agent[i].next_posy = agent[i].posy;
			}
			agent[i].isRemoveTile = false;
		}

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

		for (int i = 0; i < 4; i++) {
			if (!can_move[i]) {
				agent[i].next_posx = agent[i].posx;
				agent[i].next_posy = agent[i].posy;
			}
		}
		//　ここまで

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
	// ゲーム終了時の処理j
	if (turn == 0) {
	}
}

bool GameServer::move_agent(Step step, int agent_id, Agent agent[4], bool isRemoveTile_) {
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

// 得点計算
game_score count_score(State team) {
	if (team == NEUTRAL) {

	}
}