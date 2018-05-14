#include "Agent.h"


Agent::Agent() {
}

Agent::Agent(Point p, State t)
{
	pos = p;
	team = t;
}

void Agent::move(Point p) {
	pos = p;
}

void Agent::draw(FINFO* info) {
	int half_tile = info->tile_size / 2;
	const int h = 5;
	if (team == BLUE) {
		circle = Circle(pos.x * info->tile_size + info->margin + half_tile, pos.y * info->tile_size + info->margin + half_tile, half_tile - 3);
		circle.draw(Palette::Skyblue);
		circle.drawFrame(0, 1, Palette::Black);
	}
	else {
		circle = Circle(pos.x * info->tile_size + info->margin + half_tile, pos.y * info->tile_size + info->margin + half_tile, half_tile - 3);
		circle.draw(Palette::Orange);
		circle.drawFrame(0, 1, Palette::Black);
	}

	Point pn = Point(pos.x * info->tile_size + info->margin + half_tile, pos.y * info->tile_size + info->margin + half_tile);
	switch (next_step)
	{
	case N:
		Line(pn, pn + Point(0, -info->tile_size)).draw(2, Palette::Red);
		break;
	case NE:
		Line(pn, pn + Point(info->tile_size, -info->tile_size)).draw(2, Palette::Red);
		break;
	case E:
		Line(pn, pn + Point(info->tile_size, 0)).draw(2, Palette::Red);
		break;
	case SE:
		Line(pn, pn + Point(info->tile_size, info->tile_size)).draw(2, Palette::Red);
		break;
	case S:
		Line(pn, pn + Point(0, info->tile_size)).draw(2, Palette::Red);
		break;
	case SW:
		Line(pn, pn + Point(-info->tile_size, info->tile_size)).draw(2, Palette::Red);
		break;
	case W:
		Line(pn, pn + Point(-info->tile_size, 0)).draw(2, Palette::Red);
		break;
	case NW:
		Line(pn, pn + Point(-info->tile_size, -info->tile_size)).draw(2, Palette::Red);
		break;
	case STAY:
		Circle(pos.x * info->tile_size + info->margin + half_tile, pos.y * info->tile_size + info->margin + half_tile, 4).draw(Palette::Red);
		break;
	case N_A:
		break;
	default:
		break;
	}
}

void Agent::click(FINFO* info) {
	double euler;
	if (circle.leftClicked) {
		show = true;
		next_step = N_A;
		time = 0;
		euler = 0;
	}
	if (show) {
		double dist = ((pos * info->tile_size) + Point(info->tile_size / 2 + info->margin, info->tile_size / 2 + info->margin)).distanceFrom(Mouse::Pos());
		Point mv = (Mouse::Pos() - (pos * info->tile_size + Point(info->tile_size / 2 + info->margin, info->tile_size / 2 + info->margin)));
		double theta_atan = Atan((mv.y / dist) / (mv.x / dist)) + Pi / 2;

		if (Acos(mv.x / dist) <= Pi / 2) {
			euler = theta_atan / (2 * Pi) * 360;
		}
		else
		{
			euler = theta_atan / (2 * Pi) * 360 + 180;
		}

		if (dist < 20) {
			time++;
			if (time >= 15) {
				next_step = STAY;
				next_pos = pos;
				show = false;
			}
		}
		if (dist >= 20 && Input::MouseL.released)
		{
			if (euler >= 337.5 || euler < 22.5) {
				set_step(N, info->row, info->column);
			}
			if (euler >= 22.5 && euler < 67.5) {
				set_step(NE, info->row, info->column);
			}
			if (euler >= 67.5 && euler < 112.5) {
				set_step(E, info->row, info->column);
			}
			if (euler >= 112.5 && euler < 157.5) {
				set_step(SE, info->row, info->column);
			}
			if (euler >= 157.5 && euler < 202.5) {
				set_step(S, info->row, info->column);
			}
			if (euler >= 202.5 && euler < 247.5) {
				set_step(SW, info->row, info->column);
			}
			if (euler >= 247.5 && euler < 292.5) {
				set_step(W, info->row, info->column);
			}
			if (euler >= 302.5 && euler < 337.5) {
				set_step(NW, info->row, info->column);
			}
		}
	}
}

void Agent::fset_show() {
	show = false;
}

Step Agent::get_step() {
	return next_step;
}

void Agent::set_next_pos(Point p) {
	next_pos = p;
}

Point Agent::get_next_pos() {
	return next_pos;
}

void Agent::move(Tile *tile) {
	tile->set_color(team);
	pos = next_pos;
	next_pos = Point(0, 0);
	next_step = N_A;
}

State Agent::get_team() {
	return team;
}

Point Agent::get_pos() {
	return pos;
}

void Agent::set_pos(Point p) {
	pos = p;
}

bool Agent::set_step(Step s, int row, int column) {
	switch (s)
	{
	case N:
		if (pos.y != 0) {
			next_step = s;
			next_pos = pos + Point(0, -1);
			return true;
		}
		else
			return false;
		break;
	case NE:
		if (pos.y != 0 && pos.x != (row - 1)) {
			next_step = s;
			next_pos = pos + Point(1, -1);
			return true;
		}
		else
			return false;
		break;
	case E:
		if (pos.x != (row - 1)) {
			next_step = s;
			next_pos = pos + Point(1, 0);
			return true;
		}
		else
			return false;
		break;
	case SE:
		if (pos.x != (row - 1) && pos.y != (column - 1)) {
			next_step = s;
			next_pos = pos + Point(1, 1);
			return true;
		}
		else
			return false;
		break;
	case S:
		if (pos.y != (column - 1)) {
			next_step = s;
			next_pos = pos + Point(0, 1);
			return true;
		}
		else
			return false;
		break;
	case SW:
		if (pos.x != 0 && pos.y != (column - 1)) {
			next_step = s;
			next_pos = pos + Point(-1, 1);
			return true;
		}
		else
			return false;
		break;
	case W:
		if (pos.x != 0) {
			next_step = s;
			next_pos = pos + Point(-1, 0);
			return true;
		}
		else
			return false;
		break;
	case NW:
		if (pos.x != 0 && pos.y != 0) {
			next_step = s;
			next_pos = pos + Point(-1, -1);
			return true;
		}
		else
			return false;
		break;
	case STAY:
		next_step = s;
		next_pos = pos;
		return true;
		break;
	case N_A:
		return false;
		break;
	default:
		return false;
	}
}