#include "main.h"

//test

int main() {
	field field;	//�t�B�[���h���

	field.load("shape_info.txt", field.board);
	field.view(field.board);
	printf("minimax: %d\n", minimax(field.board, 2));
	

	system("pause");
	return 0;
}