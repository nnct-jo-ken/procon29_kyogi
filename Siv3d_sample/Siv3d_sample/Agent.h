#pragma once
#include <Siv3D.hpp>
#include <math.h>
#include "Tile.h"
#include "Main.h"

enum Step{ N, NE, E, SE, S, SW, W, NW, STAY, N_A};

class Agent
{
	Point pos = Point(0, 0);
	Point next_pos = Point(0, 0);
	State team = BLUE;
	Circle circle;
	Step next_step = N_A;
	float time;
	bool show = false;
	bool b1 = false;
public:
	Agent();
	Agent(Point p, State t);
	void move(Point p);
	void draw(FINFO* info);
	void click(FINFO* info);
	void fset_show();
	Step get_step();
	Point get_next_pos();
	void set_next_pos(Point p);
	Point get_pos();
	void set_pos(Point p);
	void move(Tile *tile);
	State get_team();
	bool set_step(Step s, int row, int column);
};

