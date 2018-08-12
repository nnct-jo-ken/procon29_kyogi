#pragma once
#include <vector>

struct field {
	int turn;	//ƒ^[ƒ“”
	int width;	//c‚Ì‘å‚«‚³
	int height;	//‰¡‚Ì‘å‚«‚³
	struct agent {
		int x;	//‰¡•ûŒü‚ÌˆÊ’u
		int y;	//c•ûŒü‚ÌˆÊ’u
	}a1, a2;
	std::vector<std::vector<int>> value;	//“_”
	std::vector<std::vector<int>> state;	//0:‚Ç‚¿‚ç‚Å‚à‚È‚¢ 1:©w 2:“Gw
};
