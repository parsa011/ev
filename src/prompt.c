#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prompt.h"
#include "editor.h"
#include "tty.h"
#include "line.h"
#include "key.h"

public bool prompt_bool(char *message)
{
	puts(message);
	return true;
}

public char *prompt_string(char *message)
{
	buffer_t *buf = editor_buffer();
	cursor_pos_t prev_pos = MAKE_POS(buf->pos.row, buf->pos.col);

	tty_cursor_move(MAKE_POS(editor.rows, 1));
	tty_put_string(true, message);
	line_t *line = line_init("", 0);
	int c = 0;
	while (true) {
		c = key_read();
		if (c == CTRL_KEY('g'))
			goto cancel;
		else if (c == CTRL_KEY('m'))
			break;
		else if (c == BACKSPACE) {
			if (line->len < 1)
				continue;
			line_delete_char(line, line->len);
			goto print;
		}
		char *str = key_to_str(c);
		line_insert_string(line, str, line->len);
print:
		tty_cursor_move(MAKE_POS(editor.rows, strlen(message)));
		tty_clear_eol();
		tty_put_string(true, " %s", line->str);
		if (str) {
			free(str);
			str = NULL;
		}
	}

	tty_cursor_move(prev_pos);
	char *str = strdup(line->str);
	line_free(line);
	return str;
cancel:
	return NULL;
}
