#include "CountScore.h"
#include "Tile.h"
#include <queue>

struct ScoreResult CountScore::score(State s, Grid<Tile> tiles, FINFO* info) {
	Grid<bool> is_field_end(tiles.size(), false);
	Grid<bool> is_searched(tiles.size(), false);
	int a = 0;
	struct ScoreResult result;

	//�^�C���|�C���g�����Z
	for (auto p : step(tiles.size()))
	{
		if (tiles[p].get_state() == s) {
			result.tile += tiles[p].get_score();
		}
	}
	// �؂�ݒ�
	for (auto p : step(tiles.size()))
	{
		if (p.x == 0 || p.y == 0 || p.x == info->row - 1 || p.y == info->row - 1)
		{
			is_field_end[p] = true;
		}
	}

	// �̈�|�C���g���v�Z
	for (auto p : step(tiles.size())) {
		if (tiles[p].get_state() != s && !is_field_end[p] && !is_searched[p]) {
			bool success = true;
			int sco = 0;
			std::queue<Point> qu;
			qu.push(p);
			is_searched[qu.front()] = true;
			while (true) {
				if (qu.empty()) {
					if (success) {
						result.area += sco;
					}
					break;
				}

				sco += tiles[qu.front()].get_score() >= 0 ? tiles[qu.front()].get_score() : -tiles[qu.front()].get_score();

				if (is_field_end[qu.front()]) {
					qu.pop();
					success = false;
					continue;
				}

				// x+1�̏ꍇ
				Point ps = qu.front() + Point(1, 0);
				if (tiles[ps].get_state() != s && !is_searched[ps]) {
					qu.push(ps);
					is_searched[ps] = true;
				}
				// x-1�̏ꍇ
				ps = qu.front() + Point(-1, 0);
				if (tiles[ps].get_state() != s && !is_searched[ps]) {
					qu.push(ps);
					is_searched[ps] = true;
				}
				// y+1�̏ꍇ
				ps = qu.front() + Point(0, 1);
				if (tiles[ps].get_state() != s && !is_searched[ps]) {
					qu.push(ps);
					is_searched[ps] = true;
				}
				// y-1�̏ꍇ
				ps = qu.front() + Point(0, -1);
				if (tiles[ps].get_state() != s && !is_searched[ps]) {
					qu.push(ps);
					is_searched[ps] = true;
				}
				qu.pop();
			}
		}
	}
	return result;
}