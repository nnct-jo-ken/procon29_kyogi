#include "main.h"

int main() {
	struct field *field;
	field = load("shape_info.txt");
	view(field);
	
	system("pause");
	return 0;
}