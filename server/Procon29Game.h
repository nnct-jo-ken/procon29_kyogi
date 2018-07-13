#pragma once

#include <iostream>
#include <random>
#include <queue>

#pragma once
enum Step { N, NE, E, SE, S, SW, W, NW, STAY, N_A };

typedef struct tile {
	int point = 1;
	int state = 0;
}Tile;

typedef struct agent {
	bool isRemoveTile = false;
	int state = 0;
	Step step = N_A;
	Step next_step = N_A;
	int next_posx = 0;
	int next_posy = 0;
	int posx = 0;
	int posy = 0;
}Agent;

struct game_score {
	int tile_point = 0;
	int area_point = 0;
};