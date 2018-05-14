#pragma once
#include "Tile.h"
#include "Main.h"

struct ScoreResult {
	int area = 0;
	int tile = 0;
};

class CountScore
{
public:
	static struct ScoreResult score(State s, Grid<Tile> tiles, FINFO* info);
};

