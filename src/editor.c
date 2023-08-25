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
	editor.current_buffer = buffer_init(NULL);
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
		}
		if (str)
			free(str);
	}
	return create_return_message(SUCCESS, "editor closed without error");
}

public return_message editor_render()
{
	tty_clear();
	buffer_t *buf = editor_buffer();
	line_t *line = buffer_line_by_index(buf->line_offset);
	int printed_rows = 1;
	while (line && printed_rows < editor.rows - 3) {
		editor_render_line(line);
		line = L_LINK_NEXT(line);
		printed_rows++;
	}
	buf->render = false;
	log("%s", buf->current_line->str);
	log("%d", buf->line_count);
	log("offset : %d\t Row : %d\t col : %d\t len : %d", buf->char_offset, buf->pos.row, buf->pos.col, buf->current_line->len);
	return create_return_message(SUCCESS, "buffer rendered");
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
