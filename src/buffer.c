#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "key.h"
#include "file.h"
#include "util.h"
#include "commands/commands.h"

public buffer_t *buffer_init(char *filepath)
{
	buffer_t *buf = (buffer_t *) malloc(sizeof(buffer_t));
	assert(buf);
	SET_POS(buf->pos, 1, 1);
	return buf;
}

public return_message buffer_file_open(char *filepath)
{
	if (!file_exists(filepath)) {
		editor.current_buffer->first_line = editor.current_buffer->current_line = line_init("", 0);
		return create_return_message(ERROR, "new buffer");
	}
	buffer_t *buf = editor_buffer();
	buf->filepath = filepath;
	buf->name = file_name(filepath);
	buffer_file_load(filepath, REPLACE);
	editor.current_buffer->render = true;
	return create_return_message(SUCCESS, "file opened");
}

public return_message buffer_file_close()
{
	return create_return_message(SUCCESS, "file closed");
}

public return_message buffer_file_save()
{
	return create_return_message(SUCCESS, "file saved");
}

public return_message buffer_file_load(char *filepath, line_load_mode mode)
{
	// TODO : Implement other modes :))
	if (!file_exists(filepath))
		return create_return_message(ERROR, "file does not exists");
	buffer_t *buf = editor_buffer();
	FILE *fp = fopen(filepath, "r");

	char *line_chars = NULL;
	size_t linecap = 0;
	ssize_t line_length;

	line_t *ln;

	/* read other lines and add them to buffer */  
	while ((line_length = getline(&line_chars, &linecap, fp)) != EOF) {
		while (*(line_chars + line_length - 1) == '\n')
			line_chars[--line_length] = '\0';
		ln = line_init(line_chars, line_length);
		buffer_append_line(ln);
		buf->current_line = ln;
	}
	buf->current_line = buf->first_line;
	free(line_chars);
	fclose(fp);
	return create_return_message(SUCCESS, "file lines loaded");
}

public return_message buffer_append_line(line_t *line)
{
	buffer_t *buf = editor_buffer();
	if (!buf->first_line) {
		buf->first_line = buf->current_line = line;
	} else {
		L_LINK_INSERT(buf->current_line, line);
	}
	buf->line_count++;
	return create_return_message(SUCCESS, "line appended");
}

public void buffer_insert_key(int key)
{
	buffer_t *buf = editor_buffer();
	buf->render = true;
	char *str = key_to_str(key);
	int len = strlen(str);
	if (key == '\t') {
		line_insert_string(buf->current_line, "\t", buf->char_offset);
		buf->pos.col += TAB_SIZE;
		buf->char_offset++;
	} else if (key == BACKSPACE) {
		if (buf->char_offset == 0) {
			line_t *prev_line = L_LINK_PREV(buf->current_line);
			if (!prev_line)
				return;
			int prev_line_len = prev_line->len;
			line_append_string(prev_line, buf->current_line->str);
			line_delete(false);
			buffer_go_to_offset(prev_line_len);
		} else {
			char prev_char = *(buf->current_line->str + buf->char_offset - 1);
			line_delete_char(buf->current_line, buf->char_offset - 1);
			if (prev_char == '\t')
				buf->pos.col -= TAB_SIZE;
			else
				buf->pos.col--;
			buf->char_offset--;
		}
	} else {
		line_insert_string(buf->current_line, str, buf->char_offset);
		buf->pos.col += len;
		buf->char_offset += len;
	}
	free(str);
}

public void buffer_check_offset()
{
	buffer_t *buf = editor_buffer();
	line_t *ln = buf->current_line;
	buf->char_offset = col_to_offset(ln->str, buf->pos.col);
	buf->pos.col = offset_to_col(ln->str, buf->char_offset);
	if (buf->char_offset >= ln->len) {
		end_of_line_command(NULL);
	}
}

public void buffer_go_to_offset(int offset)
{
	buffer_t *buf = editor_buffer();
	/*
	 * offset is index bound and cant be greater or equal to current line len
	 */
	if (offset >= buf->current_line->len)
		return;
	buf->char_offset = offset;
	buf->pos.col = offset_to_col(buf->current_line->str, offset);
}

public void buffer_go_to_line(int index)
{
	buffer_t *buf = editor_buffer();
	if (index + 1 > buf->line_count)
		return;
	buf->line_offset = index;
	buf->current_line = buffer_line_by_index(index);
	buf->pos.row = 1;
	buffer_check_offset();
	buf->render = true;
}

public char buffer_current_char()
{
	buffer_t *buf = editor_buffer();
	return *(buf->current_line->str + buf->char_offset);
}

public line_t *buffer_line_by_index(int index)
{
	line_t *line = editor_buffer()->first_line;
	for (int i = 0; i < index; i++) {
		line = L_LINK_NEXT(line);
	}
	return line;
}
