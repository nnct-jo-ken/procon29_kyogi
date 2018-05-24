#include <iostream>
#include <stdio.h>
#include "main.h"

int main(void) {
	struct board field[11][11];

	init func_field;
	func_field.rand_board(*field, 11, 11);

	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			printf("%4d ", field[i][j].point);
		}
		putchar('\n');
	}

	return 0;
}