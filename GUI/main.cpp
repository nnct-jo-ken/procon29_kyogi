#include <iostream>
#include <stdio.h>
#include "main.h"

int main(void) {
	struct board field[10][10];

	init func_field;
	func_field.rand_board(*field, 10, 10);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			printf("%4d ", field[i][j].point);
		}
		putchar('\n');
	}

	return 0;
}