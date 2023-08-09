#include "tty.h"
#include "key.h"
#include <stdio.h>
#include <stdlib.h>

public int key_read()
{
	int pending = 0;
	int c = tty_get_char(&pending);
	if (c == ESC) {
		if (pending == 0) {
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
				case 'S':
					return F4;
			}
		} else if (c == '[') {
			c = tty_get_char(&pending);
			if (pending == 1)
				tty_get_char(&pending);
			switch (c) {
				case '1' :
					return HOME;
				case '2' :
					return INSERT;
				case '3' :
					return DELETE;
				case '4' :
					return END;
				case '5' :
					return PAGE_UP;
				case '6' :
					return PAGE_DOWN;
				case 'A' :
					return ARROW_UP;
				case 'B' :
					return ARROW_DOWN;
				case 'C' :
					return ARROW_RIGHT;
				case 'D' :
					return ARROW_LEFT;
			}
		}
		// meta key and control meta key
		if (pending == 0) {
			if (c == CTRL_KEY(c)) {
				return ALT_KEY(CTRL_KEY(c));
			}
			return ALT_KEY(c);
		}
	}
	return c;
}

public char *key_to_str(int key)
{
	switch (key) {
		case F1:
			return "F1";
		case F2:
			return "F2";
		case F3:
			return "F3";
		case F4:
			return "F4";
		case HOME:
			return "HOME";
		case INSERT:
			return "INSERT";
		case DELETE:
			return "DELETE";
		case END:
			return "END";
		case PAGE_UP:
			return "PAGE_UP";
		case PAGE_DOWN:
			return "PAGE_DOWN";
		case ARROW_UP:
			return "ARROW_UP";
		case ARROW_DOWN:
			return "ARROW_DOWN";
		case ARROW_RIGHT:
			return "ARROW_RIGHT";
		case ARROW_LEFT:
			return "ARROW_LEFT";
	}
	char *buf = (char *)malloc(16 * sizeof(char));
	int len = 0;
#define PUSH(c) {        \
	buf[len++] = c;      \
	buf[len] = '\0';     \
}
#define PUSHS(s) {       \
	int len = strlen(s); \
	strcpy(buf, s);      \
	buf[len] = '\0';     \
}
	if (key == CTRL_KEY(key)) {
		PUSH('C');
		PUSH('-');
	}
	if (key == ALT_KEY(key)) {
		PUSH('M');
		PUSH('-');
	}
	if (key == CTRL_KEY(key)) {
		PUSH(key + '@');
	} else
		PUSH(key);
	return buf;
#undef PUSH
#undef PUSHS
}
