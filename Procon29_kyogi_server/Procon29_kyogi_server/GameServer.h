#pragma once
#include "Main.h"
class GameServer
{
public:
	void init(int row_, int column_, int turn_);
	void server();
	bool move_agent(Step step, int agent_id, Agent agent[4], bool isRemoveTile_);
	// team‚ðNEUTRAL‚É‚µ‚È‚¢‚±‚Æ
	game_score count_score(State team);
private:
	Tile * *field;
	Agent *agent = new Agent[4];
	int row = 0;
	int column = 0;
	int turn = 0;
};