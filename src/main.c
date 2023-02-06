#include <stdio.h>
#include <unistd.h>
#include "tty.h"
#include <ctype.h>

int main()
{
	tty_raw_mode();
	char c = tty_get_char();
	while (c) {
		printf("%c\n", c);
		if (CTRL_KEY('q') == c) {
			printf("we got a 'q'\n");
			break;
		}
		c = tty_get_char();
	}
	tty_cooked_mode();
	return 0;
}
