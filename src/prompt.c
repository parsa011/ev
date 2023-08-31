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
    buffer_t *buf = editor_buffer();
    cursor_pos_t prev_pos = MAKE_POS(buf->pos.row, buf->pos.col);

    tty_cursor_move(MAKE_POS(editor.rows, 1));
	tty_put_string(true, "%s (y/n) ", message);
	bool ans;
    int c;
    while (true) {
        c = key_read();
        if (c == 'y' || c == 'n') {
            ans = c == 'y';
            break;
        }
    }
    tty_cursor_move(prev_pos);
	return ans;
}

public char *prompt_string(char *message)
{
	buffer_t *buf = editor_buffer();
	cursor_pos_t prev_pos = MAKE_POS(buf->pos.row, buf->pos.col);

    tty_cursor_move(MAKE_POS(editor.rows, 1));
    tty_clear_eol();
	tty_put_string(true, message);
	int msg_len =  strlen(message);
	int cursor_col = msg_len + 1;
#define CHAR_OFFSET (cursor_col - msg_len - 1)
	line_t *line = line_init("", 0);

	int c = 0;
	while (true) {
		c = key_read();
		if (c == CTRL_KEY('g'))
			goto cancel;
		else if (c == CTRL_KEY('m'))
			break;
		else if (c == BACKSPACE) {
			if (CHAR_OFFSET == 0)
				continue;
			line_delete_char(line, CHAR_OFFSET - 1);
			cursor_col--;
			goto print;
		} else if (c == ARROW_LEFT || c == CTRL_KEY('b')) {
			if (cursor_col - 1 <= msg_len) {
				continue;
			}
			cursor_col--;
			goto print;
		} else if (c == ARROW_RIGHT  || c == CTRL_KEY('F')) {
			if (CHAR_OFFSET + 1 > line->len)
				continue;
			cursor_col++;
			goto print;
		}
		char *str = key_to_str(c);
		line_insert_string(line, str, CHAR_OFFSET);
		cursor_col += strlen(str);
print:
		tty_cursor_move(MAKE_POS(editor.rows, msg_len));
		tty_clear_eol();
		tty_put_string(true, " %s", line->str);
		tty_cursor_move(MAKE_POS(editor.rows, cursor_col));
		if (str) {
			free(str);
			str = NULL;
		}
	}

	tty_cursor_move(prev_pos);
	char *str = strdup(line->str);
	line_free(line);
#undef CHAR_OFFSET
	return str;
cancel:
    tty_cursor_move(prev_pos);
#undef CHAR_OFFSET
	return NULL;
}
