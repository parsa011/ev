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
	editor.current_buffer = editor_buffer_init(NULL);
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
	while (true) {
		if (editor_buffer()->render)
			editor_render();
		tty_cursor_move(editor_buffer()->pos);
		command cmd = command_read();
		if (cmd.func != null)
			cmd.func(null);
	}
	return create_return_message(SUCCESS, "editor closed without error");
}

public return_message editor_render()
{
	tty_clear();
	editor_buffer_t *buf = editor_buffer();
	editor_buffer_line_t *line = editor_buffer()->first_line;
	int printed_rows = 1;
	while (line && printed_rows < editor.rows) {
		editor_render_line(line);
		line = L_LINK_NEXT(line);
		printed_rows++;
	}
	buf->render = false;
	//log("%s", buf->current_line->str);
	//log("%c", *(buf->current_line->str + buf->char_offset));
	return create_return_message(SUCCESS, "buffer rendered");
}

public void editor_render_line(editor_buffer_line_t *line)
{
	assert(line);
	char *ptr = line->str;
	while (*ptr) {
		if (*ptr == '\t') {
			for (int i = 0; i < TAB_SIZE; i++) {
				tty_put_char(' ');
			}
		} else
			tty_put_char(*ptr);
		ptr++;
	}
	tty_put_string(true, "\r\n");
}

public void editor_check_offset()
{
	editor_buffer_t *buf = editor_buffer();
	editor_buffer_line_t *ln = buf->current_line;
	if (buf->char_offset >= ln->len) {
		buf->char_offset = ln->len - 1;
		buf->pos.col = string_len_to_offset(ln->str, ln->len);
	}
}

public editor_buffer_t *editor_buffer()
{
	return editor.current_buffer;
}

public editor_buffer_t *editor_buffer_init(char *filepath)
{
	editor_buffer_t *buf = (editor_buffer_t *) malloc(sizeof(editor_buffer_t));
	assert(buf);
	SET_POS(buf->pos, 1, 1);
	return buf;
}

public return_message editor_file_open(char *filepath)
{
	if (!file_exists(filepath)) {
		editor.current_buffer->first_line = editor.current_buffer->current_line = editor_buffer_line_init("", 0);
		return create_return_message(ERROR, "new buffer");
	}
	editor_buffer_t *buf = editor_buffer();
	buf->filepath = filepath;
	buf->name = file_name(filepath);
	editor_file_load_lines(filepath, REPLACE);
	editor.current_buffer->render = true;
	return create_return_message(SUCCESS, "file opened");
}

public return_message editor_file_close()
{
	return create_return_message(SUCCESS, "file closed");
}

public return_message editor_file_save()
{
	return create_return_message(SUCCESS, "file saved");
}

public return_message editor_file_load_lines(char *filepath, line_load_mode mode)
{
	// TODO : Implement other modes :))
	if (!file_exists(filepath))
		return create_return_message(ERROR, "file does not exists");
	editor_buffer_t *buf = editor_buffer();
	FILE *fp = fopen(filepath, "r");

	char *line_chars = NULL;
	size_t linecap = 0;
	ssize_t line_length;

	editor_buffer_line_t *ln;

	/* read other lines and add them to buffer */  
	while ((line_length = getline(&line_chars, &linecap, fp)) != EOF) {
		while (*(line_chars + line_length - 1) == '\n')
			line_length--;
		ln = editor_buffer_line_init(line_chars, line_length);
		editor_buffer_line_append(ln);
		buf->current_line = ln;
	}
	buf->current_line = buf->first_line;
	free(line_chars);
	fclose(fp);
	return create_return_message(SUCCESS, "file lines loaded");
}

public editor_buffer_line_t *editor_buffer_line_init(char *str, int len)
{
	editor_buffer_line_t *line = (editor_buffer_line_t *) malloc(sizeof(editor_buffer_line_t));
	line->str = (char *) malloc((len + 1) * sizeof(char));
	strncpy(line->str, str, len);
	line->len = len;
	return line;
}

public return_message editor_buffer_line_append(editor_buffer_line_t *line)
{
	editor_buffer_t *buf = editor_buffer();
	if (!buf->first_line) {
		buf->first_line = buf->current_line = line;
	} else {
		L_LINK_INSERT(buf->current_line, line);
	}
	buf->line_count++;
	return create_return_message(SUCCESS, "line appended");
}
