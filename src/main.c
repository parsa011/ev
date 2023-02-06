#include <stdio.h>
#include <unistd.h>
#include "tty.h"

int main()
{
	tty_raw_mode();
	char c = tty_get_char();
	while (c) {
		printf("%d\n", c);
		if (c == 'q') {
			printf("we got a 'q'\n");
			break;
		}
		c = tty_get_char();
	}
	/* char c; */
	/* while (read(STDIN_FILENO, &c, 1)) { */
	/* 	if (c == 'q') */
	/* 		break; */
	/* 	printf("%d\n\b", c); */
	/* } */
	/* printf("Hello World!\n"); */
	tty_cooked_mode();
	return 0;
}
