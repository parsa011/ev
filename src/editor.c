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
	editor.current_buffer = buffer_init(NULL, BUFFER_ROW);

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
	buffer_t *buf = editor.current_buffer;
	/*
	 * go to first buffer
	 */
	while (L_LINK_PREV(buf))
		buf = L_LINK_PREV(buf);
	buffer_t *next = buf;
	while (next) {
		next = L_LINK_NEXT(buf);
		buffer_free(buf);
		buf = next;

	}
}

public return_message editor_run()
{
	int c;
	char *str;
	buffer_t *buf;
	editor_render();
	while (true) {
		buf = editor_buffer();
		if (buf->render)
			editor_render();
		tty_cursor_move(buf->pos);
		c = key_read();
		str = key_to_str(c);
		command cmd = command_read(str);
		if (cmd.func != null) {
			return_message res = cmd.func(null);
			if (res.status == ERROR) {
			}
		}
		else {
			buffer_insert_key(c);
			buffer_dirty();
		}
		if (str)
			free(str);
		if (editor.exit)
			break;
	}
	return create_return_message(SUCCESS, "editor closed without error");
}

public void editor_buffer_append(buffer_t *buf)
{
	L_LINK_INSERT(editor_buffer(), buf);
}

public void editor_buffer_change(buffer_t *buf)
{
	editor.current_buffer = buf;
	buf->render = true;
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
	tty_cursor_move(MAKE_POS(editor.statusbar.margin, 1));
	tty_put_string(true, "\033[107m\033[30m");
	char buf[editor.cols];
	char *bufp = buf;
	// cbuf stands for current_buffer :)
	buffer_t *cbuf = editor_buffer();

#define ADD_TEXT(s) do {				\
	bufp += sprintf(bufp, s);			\
} while (false);

#define ADD_TEXTF(s, ...) do {					\
		bufp += sprintf(bufp, s, __VA_ARGS__);	\
} while (false)

	ADD_TEXT("EV-Editor ");
	if (cbuf->dirty) {
		ADD_TEXT("*");
	} else
		ADD_TEXT("-");

	if (cbuf->name)
		ADD_TEXTF(" %s ", cbuf->name);
	else
		ADD_TEXT(" [NO FILE] ");

	ADD_TEXTF(" ----- %ld Line ", cbuf->line_count);

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
