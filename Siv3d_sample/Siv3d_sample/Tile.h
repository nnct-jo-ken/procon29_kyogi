#pragma once

#include<Siv3D.hpp>
#include "Main.h"

enum State { NEUTRAL, ORANGE, BLUE };


class Tile
{		
	Rect rect = Rect(0, 0, 0, 0);
	State state = NEUTRAL;
	int score = 0;
	bool score_ = true;
public:
	Tile();
	Tile(Rect r, State s, int sc);
	void set_color(State s);
	void draw_tile(Font sfont);
	State get_state();
	int get_score();
	void set_score_false();
};

