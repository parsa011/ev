#include "tty.h"
#include "key.h"
#include <stdio.h>

public int key_read()
{
	int pending = 0;
	int c = tty_get_char(&pending);
	if (c & 0x1f == ('m' & 0x1f)) {
		printf("Control key\r\n");
	} else if (c == ESC) {
		if (pending == 0) {
			printf("Escape key\r\n");
			return ESC;
		}
		c = tty_get_char(&pending);
		if (c == 'O') {
			c = tty_get_char(&pending);
			switch (c) {
				case 'P':
					printf("F1 Presssed\r\n");
					break;
				case 'Q':
					printf("F2 Presssed\r\n");
					break;
				case 'R':
					printf("F3 Presssed\r\n");
					break;
				case 'S':
					printf("F4 Presssed\r\n");
					break;

			}
		} else if (c == '[') {
			c = tty_get_char(&pending);
			switch (c) {
				case '1' :
					printf("Home\r\n");
					break;
				case '2' :
					printf("Insert\r\n");
					break;
				case '3' :
					printf("Delete\r\n");
					break;
				case '4' :
					printf("End\r\n");
					break;
				case '5' :
					printf("page up\r\n");
					break;
				case '6' :
					printf("Page down\r\n");
					break;
				case 'A' :
					printf("Arrow Up\r\n");
					break;
				case 'B' :
					printf("Arrow Down\r\n");
					break;
				case 'C' :
					printf("Arrow Right\r\n");
					break;
				case 'D' :
					printf("Arrow Left\r\n");
					break;
			}
		}
		// meta key and control meta key
		if (pending == 0) {
			printf("Meta Key\r\n");
		}
	}
	return c;
}
