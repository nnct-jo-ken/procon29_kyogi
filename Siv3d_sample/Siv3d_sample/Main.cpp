#include <Siv3D.hpp>
#include <queue>
#include <fstream>
#include <Windows.h>
#include "Tile.h"
#include "Agent.h"
#include "CountScore.h"
#include "Main.h"


bool check_complete(Array<Agent> agents) {
	for (auto p : step(agents.size())) {
		if (agents[p].get_step() == N_A)
			return false;
	}
	return true;
}

void Main()
{
		const int tile_size = 40;
		const int margin = 40;
		const Font font(15);
		const Font sfont(13);
		int turn = Random(60, 120);
		int turn_i = 1;
		int row = (10 + Random(-2, 2));
		int column = (10 + Random(-2, 2));
		Array<Agent> agents;
		if (row == 0) { row = 5; column = 5; }

		FINFO info;
		info.row;
		info.column = column;
		info.tile_size = tile_size;
		info.margin = margin;

		Window::Resize(tile_size * row + margin * 2, tile_size * column + margin * 2);
		Graphics::SetBackground(Palette::White);

		Grid<Tile> tiles(column, row);
		Grid<int> score(column, row);

		for (auto p : step(score.size())){
			score[p] = 1;// Random(-16, 16);
		}
		for (auto p : step(tiles.size())) {
			tiles[p] = Tile(Rect(margin + p.y * tile_size, margin + p.x * tile_size, tile_size, tile_size), NEUTRAL, score[p]);
		}


		Point pos = RandomPoint(row / 2 - 1, column / 2 - 1);

		agents.push_back(Agent(pos, BLUE));
		agents.push_back(Agent((pos * -1) + Point(row - 1, column - 1), BLUE));
		agents.push_back(Agent(Point(pos.x * -1 + row - 1, pos.y), ORANGE));
		agents.push_back(Agent(Point(pos.x, pos.y * -1 + column - 1), ORANGE));

		for (auto p : step(agents.size())) {
			tiles[agents[p].get_pos().x][agents[p].get_pos().y].set_score_false();
		}

		for (auto p : step(agents.size())) {
			agents[p].set_next_pos(agents[p].get_pos());
			agents[p].move(&tiles[agents[p].get_next_pos().x][agents[p].get_next_pos().y]);
		}

		while (System::Update())
		{

			font(turn_i, L" / ", turn, L" ターン").draw(margin, margin / 4, Palette::Black);

			for (auto p : step(tiles.size())) {
				tiles[p].draw_tile(sfont);
			}

			for (auto p : step(agents.size())) {
				agents[p].draw(&info);
			}
			for (auto p : step(agents.size())) {
				agents[p].click(&info);
			}
			if (Input::MouseL.released)
			{
				for (auto p : step(agents.size())) {
					agents[p].fset_show();
				}
			}

			if (check_complete(agents)) {

				bool b[] = { false, false, false, false };
				// 後で関数にまとめること
				for (int i = 0; i < 3; i++) {
					if (agents[0].get_next_pos() == agents[1].get_next_pos()) {
						b[0] = true; b[1] = true;
					}
					if (agents[0].get_next_pos() == agents[2].get_next_pos()) {
						b[0] = true; b[2] = true;
					}
					if (agents[0].get_next_pos() == agents[3].get_next_pos()) {
						b[0] = true; b[3] = true;
					}
					if (agents[1].get_next_pos() == agents[2].get_next_pos()) {
						b[1] = true; b[2] = true;
					}
					if (agents[1].get_next_pos() == agents[3].get_next_pos()) {
						b[1] = true; b[3] = true;
					}
					if (agents[2].get_next_pos() == agents[3].get_next_pos()) {
						b[2] = true; b[3] = true;
					}
				}

				for (auto p : step(agents.size())) {
					if (b[p]) {
						agents[p].set_next_pos(agents[p].get_pos());
					}
				}

				for (auto p : step(agents.size())) {
					if ((tiles[agents[p].get_next_pos().x][agents[p].get_next_pos().y].get_state() != NEUTRAL) && (tiles[agents[p].get_next_pos().x][agents[p].get_next_pos().y].get_state() != agents[p].get_team()))
					{
						tiles[agents[p].get_next_pos().x][agents[p].get_next_pos().y].set_color(NEUTRAL);
						agents[p].set_next_pos(agents[p].get_pos());
					}
				}

				if (agents[0].get_next_pos() == agents[1].get_next_pos()) {
					b[0] = true; b[1] = true;
				}
				if (agents[0].get_next_pos() == agents[2].get_next_pos()) {
					b[0] = true; b[2] = true;
				}
				if (agents[0].get_next_pos() == agents[3].get_next_pos()) {
					b[0] = true; b[3] = true;
				}
				if (agents[1].get_next_pos() == agents[2].get_next_pos()) {
					b[1] = true; b[2] = true;
				}
				if (agents[1].get_next_pos() == agents[3].get_next_pos()) {
					b[1] = true; b[3] = true;
				}
				if (agents[2].get_next_pos() == agents[3].get_next_pos()) {
					b[2] = true; b[3] = true;
				}

				for (auto p : step(agents.size())) {
					if (b[p]) {
						agents[p].set_next_pos(agents[p].get_pos());
					}
				}
				// end

				for (auto p : step(agents.size())) {
					agents[p].move(&tiles[agents[p].get_next_pos().x][agents[p].get_next_pos().y]);
				}
				turn_i++;
			}
			if (turn_i > turn) {
				std::ofstream log_file("./log.txt");
				log_file << "B" << " "
					<< " " << CountScore::score(BLUE, tiles, &info).tile 
					<< " " << CountScore::score(BLUE, tiles, &info).area << std::endl;
				log_file << "O" << " "
					<< " " << CountScore::score(ORANGE, tiles, &info).tile
					<< " " << CountScore::score(ORANGE, tiles, &info).area << std::endl;

				break;
			}
		}
}