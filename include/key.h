#ifndef _KEY_H
# define _KEY_H

#include "basic.h"

#define ESC         0x1B		/* ESC character.               	*/
#define ALT_MASK    0x1000
#define CTRL_KEY(k) (k & 0x1F)
#define ALT_KEY(k)  (k | ALT_MASK)

#define KEY_EXIT 'q'

enum {
	BACKSPACE = 127,
	F1 = 1000,
	F2,
	F3,
	F4,
	HOME,
	INSERT,
	DELETE,
	END,
	PAGE_UP,
	PAGE_DOWN,
	ARROW_UP,
	ARROW_DOWN,
	ARROW_RIGHT,
	ARROW_LEFT
};

/*
 *	read key from terminal and convert special key to local key
 *	also we mask meta keys with ALT_KEY macro
 */
int key_read();

char *key_to_str(int key);

char *key_combine(char *str, int key);

#endif
