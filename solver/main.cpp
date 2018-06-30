#include "main.h"

//test

int main() {
	struct field field;

	load("shape_info.txt", &field);
	view(&field);
	
	system("pause");
	return 0;
}