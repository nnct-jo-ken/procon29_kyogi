#include "Tile.h"

Tile::Tile() {
}

Tile::Tile(Rect r, State s, int sc) {
	rect = r;
	state = s;
	score = sc;
}

void Tile::set_color(State s) {
	state = s;
}

State Tile::get_state() {
	return state;
}

void Tile::draw_tile(Font sfont) {
	switch (state)
	{
	case NEUTRAL:
		rect.draw(Palette::White);
		break;
	case ORANGE:
		rect.draw(Palette::Orange);
		break;
	case BLUE:
		rect.draw(Palette::Skyblue);
		break;
	default:
		break;
	}
	rect.drawFrame(1, 1, Palette::Black);
	if(score_)
		sfont(score).draw(rect.x + 5, rect.y + 5, Palette::Black);
}

int Tile::get_score() {
	return score;
}

void Tile:: set_score_false() {
	score_ = false;
	score = 0;
}