#include "tty.h"
#include "key.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
			/*
			 * because ARROW keys doesnt have any trailing char, to we have to check if we have anything
			 * remaining or no
			 */
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
	char *buf = (char *)malloc(16 * sizeof(char));
	int len = 0;

#define PUSH(c) {        \
	buf[len++] = c;      \
	buf[len] = '\0';     \
}
#define PUSHS(s) do {     \
	int len = strlen(s);  \
	strcpy(buf, s);       \
	buf[len] = '\0';      \
} while (false);

	switch (key) {
		case F1:
			PUSHS("F1");
			return buf;
		case F2:
			PUSHS("F2");
			return buf;
		case F3:
			PUSHS("F3");
			return buf;
		case F4:
			PUSHS("F4");
			return buf;
		case HOME:
			PUSHS("HOME");
			return buf;
		case INSERT:
			PUSHS("INSERT");
			return buf;
		case DELETE:
			PUSHS("DELETE");
			return buf;
		case END:
			PUSHS("END");
			return buf;
		case PAGE_UP:
			PUSHS("PAGE_UP");
			return buf;
		case PAGE_DOWN:
			PUSHS("PAGE_DOWN");
			return buf;
		case ARROW_UP:
			PUSHS("ARROW_UP");
			return buf;
		case ARROW_DOWN:
			PUSHS("ARROW_DOWN");
			return buf;
		case ARROW_RIGHT:
			PUSHS("ARROW_RIGHT");
			return buf;
		case ARROW_LEFT:
			PUSHS("ARROW_LEFT");
			return buf;
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
#undef PUSH
#undef PUSHS
	return buf;
}

public char *key_combine(char *str, int key)
{
	char *new_key = key_to_str(key);
	/*
	 * alloc two more bytes for \0 and Space between keys str
	 */
	str = (char *) realloc(str, sizeof(char) * (strlen(str) + strlen(new_key)) + 2);
	strcat(str, " ");
	strcat(str, new_key);
	free(new_key);
	return str;
}
