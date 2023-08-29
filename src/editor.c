#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include "editor.h"
#include "prompt.h"
#include "tty.h"
#include "command.h"
#include "key.h"
#include "file.h"
#include "log.h"
#include "util.h"
#include "text.h"
#include "commands/commands.h"

editor_t editor;

public void editor_init()
{
	tty_raw_mode();
	tty_clear();

	editor_change_size();
	editor.tty_in = STDIN_FILENO;
	editor.tty_out = STDOUT_FILENO;

	/*
	 * init base stuff for editor, like buffer
	 */
	editor.current_buffer = buffer_init(NULL, editor.rows - 1);

	editor.statusbar.margin = editor.rows - 1;
}

public void editor_change_size()
{
	tty_window_size(&editor.rows, &editor.cols);
}

public void editor_close()
{
	tty_cooked_mode();
	tty_clear();
}

public return_message editor_run()
{
	int c;
	char *str;
	buffer_t *buf;
	while (true) {
		buf = editor_buffer();
		if (buf->render)
			editor_render();
		tty_cursor_move(buf->pos);
		c = key_read();
		str = key_to_str(c);
		command cmd = command_read(str);
		if (cmd.func != null)
			cmd.func(null);
		else {
			buffer_insert_key(c);
			buffer_dirty();
		}
		if (str)
			free(str);
	}
	return create_return_message(SUCCESS, "editor closed without error");
}

public return_message editor_render()
{
	tty_clear();
	tty_cursor_hide();
	editor_render_buffer();
	editor_render_statusbar();
	tty_cursor_show();
	return create_return_message(SUCCESS, "buffer rendered");
}

public void editor_render_buffer()
{
	buffer_t *buf = editor_buffer();
	line_t *line = buffer_line_by_index(buf->line_offset);
	int printed_rows = 1;
	while (line && printed_rows < buf->rows) {
		editor_render_line(line);
		line = L_LINK_NEXT(line);
		printed_rows++;
	}
	buf->render = false;
}

public void editor_render_statusbar()
{
	tty_cursor_move(MAKE_POS(1, editor.statusbar.margin));
	tty_put_string(true, "\033[107m\033[30m");
	char buf[editor.cols];
	char *bufp = buf;
	buffer_t *cbuf = editor_buffer();
#define ADD_TEXT(s) {									\
	bufp += sprintf(bufp, s);							\
}
#define ADD_TEXTF(s, ...) {							\
	bufp += sprintf(bufp, s, __VA_ARGS__);			\
}
#if DEBUG
	ADD_TEXTF("Line Count : %ld -- Line Offset : %ld -- "
			"Current Line Index : %ld -- Char Offset : %d -- Cursor Pos : ",
			cbuf->line_count,
			cbuf->line_offset, 0, cbuf->char_offset);
	print_pos(current_window->cursor_pos);
	if (cbuf->current_line) {
		char *c = buf->current_line->str + cbuf->char_offset;
		ADD_TEXTF(" --- Current char : %c", *c == '\t' ? 'T' : *c);
		ADD_TEXTF(" --- Line Length : %d", buf->current_line->len);
	}
#else
	ADD_TEXT("EV-Editor ");
	if (cbuf->dirty) {
		ADD_TEXT("*");
	} else
		ADD_TEXT("-");
	ADD_TEXTF(" %s", cbuf->name);
	ADD_TEXTF(" ----- %ld Line ", cbuf->line_count);
#endif
	int space = editor.cols - strlen(buf);
	tty_put_string(true, buf);
	for (int i = 0; i < space; i++) {
		tty_put_char('-');
	}
	tty_flush();
	tty_put_string(true, "\033[0m");
#undef ADD_TEXT
#undef ADD_TEXTF
}

public void editor_render_line(line_t *line)
{
	assert(line);
	char *ptr = line->str;
	int writed_chars_count = 1;
	/*
	 * dont write more than terminal width size, with checking writed_chars_count everytime
	 * TODO : add ellipsis for large lines
	 */
	while (*ptr) {
		if (writed_chars_count + 1 >= editor.cols)
			break;
		if (*ptr == '\t') {
			writed_chars_count += TAB_SIZE;
			for (int i = 0; i < TAB_SIZE; i++) {
				tty_put_char(' ');
			}
		} else {
			writed_chars_count++;
			tty_put_char(*ptr);
		}
		ptr++;
	}
	tty_put_string(true, "\r\n");
}

public buffer_t *editor_buffer()
{
	return editor.current_buffer;
}
