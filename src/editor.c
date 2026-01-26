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

void editor_init()
{
#ifdef EV_LOG
	start_logger();
#endif

	tty_raw_mode();
	tty_cursor_move(MAKE_POS(1, 1));
	tty_clear();

	editor_change_size();
	editor.tty_in = STDIN_FILENO;
	editor.tty_out = STDOUT_FILENO;

	editor.running_path = getcwd(NULL, 0);

	/*
	 * init base stuff for editor, like buffer
	 */
	editor.current_buffer = buffer_init(NULL, BUFFER_ROW);
	editor.statusbar.margin = editor.rows - 1;
	editor.promptbar.msg = line_init("", 0);
}

void editor_change_size()
{
	tty_window_size(&editor.rows, &editor.cols);
	editor.statusbar.margin = editor.rows - 1;
	buffer_t *buf = editor_buffer();
	if (buf != null) {
		buf->rows = editor.rows - 1;
	}
	editor_render();
}

void editor_close()
{
	tty_cooked_mode();
	tty_clear();
	buffer_t *buf = editor_buffer();
	/*
	 * go to first buffer
	 */
	while (L_LINK_PREV(buf)) {
		buf = L_LINK_PREV(buf);
	}
	buffer_t *next = buf;
	while (next) {
		next = L_LINK_NEXT(buf);
		buffer_free(buf);
		buf = next;
	}
#ifdef EV_LOG
	close_logger();
#endif

}

return_message_t editor_run()
{
	int c;
	char *str;
	buffer_t *buf;
	command cmd;
	return_message_t cmd_res;
	while (true) {
		buf = editor_buffer();
		editor_render();
		if (buf->render) {
			editor_render_buffer();
		}
		if (prompt_msg_is_expired()) {
			editor_render_promptbar();
		}
		tty_cursor_move(buf->pos);
		c = key_read();
		str = key_to_str(c);
		cmd = command_read(str);
		if (cmd.func != null) {
			cmd_res = cmd.func(null);
			if (cmd_res.status != SUCCESS) {
				prompt_message_show(cmd_res.message, strlen(cmd_res.message));
			}
		}
		else {
			if ((c == CTRL_KEY(c) && c != '\t') || c == ALT_KEY(c)) {
    			char *msg = "Command Not Found";
				prompt_message_show(msg, strlen(msg));
			} else {
				buffer_insert_key(c);
			}
		}
		if (str) {
			free(str);
		}
		if (editor.exit) {
			break;
		}
	}
	line_free(editor.promptbar.msg);
	return CREATE_RETURN_MESSAGE(SUCCESS, "editor closed without error");
}

void editor_buffer_append(buffer_t *buf)
{
	L_LINK_INSERT(editor_buffer(), buf);
}

void editor_buffer_change(buffer_t *buf)
{
	editor.current_buffer = buf;
	buf->render = true;
}

return_message_t editor_render()
{
	if (!editor_buffer())
		return CREATE_RETURN_MESSAGE(ERROR, "no buffer");
	tty_clear();
	tty_cursor_hide();
	editor_render_statusbar();
	editor_render_promptbar();
	tty_cursor_show();
	return CREATE_RETURN_MESSAGE(SUCCESS, "buffer rendered");
}

void editor_render_buffer()
{
	tty_cursor_hide();
	buffer_t *buf = editor_buffer();
	line_t *line = buffer_line_by_index(buf->line_offset);
	
	int printed_rows = 1;
	int line_nu = buf->line_offset + 1;
	int line_number_width = int_digits_count(buf->line_count) + 1;
	
	while (line && printed_rows < buf->rows) {
		if (buf->line_number) {
			editor_render_line_number(line_number_width, line_nu++);
		}
		editor_render_line(line);
		line = L_LINK_NEXT(line);
		printed_rows++;
	}
	tty_flush();
	buf->render = false;
	tty_cursor_show();
}

void editor_render_line_number(int lines_width, int line_nu)
{
	tty_put_string(false, "%d", line_nu);
	int c = int_digits_count(line_nu);
	while (c++ < lines_width) {
		tty_put_char(' ');
	}
	tty_put_string(false, "|", line_nu);
}

void editor_render_statusbar()
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
		ADD_TEXTF(" %s", cbuf->name);
	else
		ADD_TEXT(" [NO FILE]");

	ADD_TEXTF(" %d-%ld ", cbuf->char_offset + 1, cbuf->line_offset + cbuf->pos.row);

	ADD_TEXTF(" ----- %ld Line ", cbuf->line_count);

	int space = editor.cols - strlen(buf);
	tty_put_string(false, buf);
	for (int i = 0; i < space; i++) {
		tty_put_char('-');
	}
	tty_put_string(true, "\033[0m");
#undef ADD_TEXT
#undef ADD_TEXTF
}

void editor_render_promptbar()
{
	tty_cursor_move(MAKE_POS(editor.rows, 1));
	if (!prompt_msg_is_expired()) {
	  if (editor.promptbar.msg->str) {
        	tty_put_string(true, "%s", editor.promptbar.msg->str);
	  }
	} else {
	  tty_clear_eol();
	}
}

void editor_render_line(line_t *line)
{
	assert(line);
	int wroted_chars_count = 1;
	char c;
	for (int i = 0; i < line->len; i++) {
		c = line->str[i];
		if (!c) {
			break;
		}
		if (wroted_chars_count + 1 >= editor.cols) {
			break;
		}
		if (c == '\t') {
			wroted_chars_count += TAB_SIZE;
			for (int i = 0; i < TAB_SIZE; i++) {
				tty_put_char(' ');
			}
		} else {
			wroted_chars_count++;
			tty_put_char(c);
		}
	}
	tty_put_string(false, "\r\n");
}

buffer_t *editor_buffer()
{
	return editor.current_buffer;
}
