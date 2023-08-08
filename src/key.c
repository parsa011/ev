#include "tty.h"
#include "key.h"
#include <stdio.h>

public int key_read()
{
	int pending = 0;
	int c = tty_get_char(&pending);
	if (c == ESC) {
		if (pending == 0) {
			printf("Escape key\r\n");
			return ESC;
		}
		c = tty_get_char(&pending);
		if (c == 'O') {
			c = tty_get_char(&pending);
			switch (c) {
				case 'P':
					return F1;
				case 'Q':
					return F2;
				case 'R':
					return F3;
					printf("F3 Presssed\r\n");
					break;
				case 'S':
					return F4;
					printf("F4 Presssed\r\n");
					break;

			}
		} else if (c == '[') {
			c = tty_get_char(&pending);
			tty_get_char(&pending);
			switch (c) {
				case '1' :
					return HOME;
					printf("Home\r\n");
					break;
				case '2' :
					return INSERT;
					printf("Insert\r\n");
					break;
				case '3' :
					return DELETE;
					printf("Delete\r\n");
					break;
				case '4' :
					return END;
					printf("End\r\n");
					break;
				case '5' :
					return PAGE_UP;
					printf("page up\r\n");
					break;
				case '6' :
					return PAGE_DOWN;
					printf("Page down\r\n");
					break;
				case 'A' :
					return ARROW_UP;
					printf("Arrow Up\r\n");
					break;
				case 'B' :
					return ARROW_DOWN;
					printf("Arrow Down\r\n");
					break;
				case 'C' :
					return ARROW_RIGHT;
					printf("Arrow Right\r\n");
					break;
				case 'D' :
					return ARROW_LEFT;
					printf("Arrow Left\r\n");
					break;
			}
		}
		// meta key and control meta key
		if (pending == 0) {
			if (c == CTRL_KEY(c)) {
				printf("C-M Key\r\n");
				return ALT_KEY(CTRL_KEY(c));
			}
			printf("M Key\r\n");
			return ALT_KEY(c);
		}
	}
	if (c == CTRL_KEY(c))
		printf("C Key\r\n");
	return c;
}
