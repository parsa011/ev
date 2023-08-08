#ifndef _KEY_H
# define _KEY_H

#define ESC     0x1B		/* ESC character.               	*/
#define CTRL_KEY(k) ((k) & 0x1F)
#define ALT_KEY(k) ((k) | 0x1000)

#define KEY_EXIT 'q'

enum {
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

int key_read();

#endif
