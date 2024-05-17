#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "prompt.h"
#include "editor.h"
#include "tty.h"
#include "line.h"
#include "key.h"

public void prompt_clear(bool restore)
{
	buffer_t *buf = editor_buffer();
    cursor_pos_t prev_pos = MAKE_POS(buf->pos.row, buf->pos.col);

	tty_cursor_move(MAKE_POS(editor.rows, 1));
	tty_clear_eol();

	if (restore) {
		tty_cursor_move(prev_pos);
	}
}

public void prompt_message_show(char *str, int str_len)
{
    line_delete_range(editor.promptbar.msg, 0, editor.promptbar.msg->len);
    line_append_string(editor.promptbar.msg, str, strlen(str));
    editor.promptbar.msg_time = time(NULL);
    editor_buffer()->render = true;
}

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
    prompt_clear(false);
    tty_cursor_move(prev_pos);
	return ans;
}

public int *prompt_int(char *message)
{
	char *str = prompt_string(message);
	if (str) {
		int *n = malloc(sizeof(int));
		*n = atoi(str);
		return n;
	}
	return 0;
}

public char *prompt_string(char *message)
{
	buffer_t *buf = editor_buffer();
	cursor_pos_t prev_pos = MAKE_POS(buf->pos.row, buf->pos.col);

    prompt_clear(false);
	tty_put_string(true, message);
	int msg_len =  strlen(message);
#define START_COL msg_len + 1
	int cursor_col = START_COL;
	line_t *line = line_init("", 0);
	int c = 0;
	char *str = NULL;

#define CHAR_OFFSET (cursor_col - msg_len - 1)
#define CURRENT_CHAR line->str[CHAR_OFFSET]
#define DO_CANCEL goto cancel

#define DO_BACKSPACE                              \
do {                                              \
    if (CHAR_OFFSET != 0) {                       \
    	line_delete_char(line, CHAR_OFFSET - 1);  \
    	cursor_col--;                             \
    }                                             \
} while (false);

#define DO_GO_BACK                                \
do {                                              \
    if (cursor_col - 1 >  msg_len) {              \
	    cursor_col--;                             \
	}                                             \
} while (false);

#define DO_GO_FORWARD                             \
do {                                              \
    if (CHAR_OFFSET < line->len) {                \
        cursor_col++;                             \
    }                                             \
} while (false);

#define DO_EOL                                    \
do {                                              \
	cursor_col = msg_len + line->len + 1;         \
} while (false);

#define DO_BOL                                    \
do {                                              \
    cursor_col = msg_len + 1;                     \
} while (false);

#define DO_GO_FWORD                               \
do {                                              \
    if (CURRENT_CHAR) {                           \
        if (!isalpha(CURRENT_CHAR)) {             \
            while (!isalpha(CURRENT_CHAR)) {      \
                DO_GO_FORWARD;                    \
            }                                     \
        }                                         \
        while (isalpha(CURRENT_CHAR)) {           \
            DO_GO_FORWARD;                        \
        }                                         \
    }                                             \
    DO_GO_FORWARD;                                \
} while (false);

#define DO_GO_BWORD                               \
do {                                              \
    if (!isalpha(CURRENT_CHAR)) {                 \
        while (cursor_col != START_COL            \
               && !isalpha(CURRENT_CHAR)) {       \
            DO_GO_BACK;                           \
        }                                         \
    }                                             \
    while (cursor_col != START_COL                \
           && isalpha(CURRENT_CHAR)) {            \
        DO_GO_BACK;                               \
    }                                             \
} while (false);

#define DO_KILL_LINE                              \
do {                                              \
    line_delete_range(line,                       \
        CHAR_OFFSET, line->len);                  \
} while (false);

#define DO_KILL_CHAR                              \
do {                                              \
    if (CHAR_OFFSET < line->len) {                \
        DO_GO_FORWARD;                            \
        DO_BACKSPACE;                             \
    }                                             \
} while (false);

#define DO_KILL_WORD                              \
do {                                              \
    if (isalpha(CURRENT_CHAR)) {                  \
        while (isalpha(CURRENT_CHAR)              \
               && CHAR_OFFSET < line->len) {      \
            DO_KILL_CHAR;                         \
        }                                         \
    } else {                                      \
        while (!isalpha(CURRENT_CHAR)             \
               && CHAR_OFFSET < line->len) {      \
            DO_KILL_CHAR;                         \
        }                                         \
    }                                             \
} while (false);

	while (true) {
		c = key_read();
		if (c == CTRL_KEY('g')) {
    		DO_CANCEL;
		}
		else if (c == CTRL_KEY('m')) {
			break;
		}
		else if (c == BACKSPACE) {
    		DO_BACKSPACE;
    		goto print;
		} else if (c == ARROW_LEFT || c == CTRL_KEY('b')) {
    		DO_GO_BACK;
    		goto print;
		} else if (c == ARROW_RIGHT  || c == CTRL_KEY('F')) {
    		DO_GO_FORWARD;
    		goto print;
		} else if (c == CTRL_KEY('e')) {
    		DO_EOL;
    		goto print;
		} else if (c == CTRL_KEY('a')) {
    		DO_BOL;
    		goto print;
		} else if (c == ALT_KEY('f')) {
    		DO_GO_FWORD;
    		goto print;
		} else if (c == ALT_KEY('b')) {
    		DO_GO_BWORD;
    		goto print;
		} else if (c == CTRL_KEY('k')) {
            DO_KILL_LINE;
            goto print;
		} else if (c == CTRL_KEY('d')) {
    		DO_KILL_CHAR;
    		goto print;
		} else if (c == ALT_KEY('d')) {
    		DO_KILL_WORD;
    		goto print;
		}
		str = key_to_str(c);
		line_insert_string(line, str, strlen(str), CHAR_OFFSET);
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
	char *res = strdup(line->str);
	line_free(line);
	prompt_clear(false);
#undef CHAR_OFFSET
	return res;
cancel:
	prompt_clear(false);
    tty_cursor_move(prev_pos);
#undef CHAR_OFFSET
	return NULL;
}

public bool prompt_msg_is_expired()
{
	return time(NULL) - editor.promptbar.msg_time > PROMPT_MSG_TIME;
}
