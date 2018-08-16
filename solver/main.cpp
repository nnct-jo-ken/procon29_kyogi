#include "main.h"

//test

int main() {
	field field;	//フィールド情報

	field.load("shape_info.txt", field.board);
	field.view(field.board);
	
	

	system("pause");
	return 0;
}