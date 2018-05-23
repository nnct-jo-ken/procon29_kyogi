#pragma once
#include "Main.h"
class GameServer
{
public:
	void init(int row_, int column_, int turn_);
	void server();
	bool move_agent(int agent_id, Step step, bool isRemoveTile_);
	game_score count_score(int _state);
	// ƒQ[ƒ€’†‚©
	bool isGame = true;
private:
	Tile * *field;
	Agent *agent = new Agent[4];
	int row = 0;
	int column = 0;
	int turn = 0;
};