#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "key.h"
#include "file.h"
#include "util.h"
#include "commands/commands.h"

public buffer_t *buffer_init(char *filepath, int rows)
{
	buffer_t *buf = (buffer_t *) malloc(sizeof(buffer_t));
	assert(buf);
	memset(buf, 0, sizeof(buffer_t));
	buf->rows = rows;
	SET_POS(buf->pos, 1, 1);
	return buf;
}

public void buffer_dirty()
{
	buffer_t *buf = editor_buffer();
	buf->dirty = buf->render = true;
}

public return_message buffer_file_open(char *filepath)
{
	buffer_t *buf = editor_buffer();
	if (filepath) {
		buffer_set_file_name(buf, filepath);
	}
	if (!file_exists(filepath)) {
		buffer_append_line(line_init("", 0));
		return create_return_message(ERROR, "new buffer");
	}
	buffer_file_load(filepath, REPLACE);
	buf->render = true;
	return create_return_message(SUCCESS, "file opened");
}

public void buffer_set_file_name(buffer_t *buf, char *filepath)
{
	buf->filepath = strdup(filepath);
	buf->name = strdup(file_name(filepath));
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
	if (!file_exists(filepath)) {
		return create_return_message(ERROR, "file does not exists");
	}
	buffer_t *buf = editor_buffer();
	FILE *fp = fopen(filepath, "r");

	char *line_chars = NULL;
	size_t linecap = 0;
	size_t line_length;

	line_t *ln;

	/* read other lines and add them to buffer */
	while ((line_length = getline(&line_chars, &linecap, fp)) != EOF) {
		while (
			line_length > 0 
			&&
			(
				line_chars[line_length - 1]  == '\n'
				||
				line_chars[line_length - 1]  == '\r'
			)
		) {
			line_length--;
		}
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
		line_insert_string(buf->current_line, "\t", 1, buf->char_offset);
		buf->pos.col += TAB_SIZE;
		buf->char_offset++;
	} else if (key == BACKSPACE) {
		// cursor is in the beginning of line, after remove char, we should append remaining chars into previous line
		// also if line is empty (no any remaining char) we should 'remove' the line
		if (buf->char_offset == 0) {
			line_t *prev_line = L_LINK_PREV(buf->current_line);
			if (!prev_line) {
				goto ret;
			}
			int prev_line_len = prev_line->len;
			if (buf->current_line->len > 0) {
				line_append_string(prev_line, buf->current_line->str, buf->current_line->len);
			}
			buffer_delete_line(false);
			buffer_go_to_offset(prev_line_len - 1);
		} else {
			char prev_char = *buffer_char_at(buf->char_offset - 1);
			line_delete_char(buf->current_line, buf->char_offset - 1);
			if (prev_char == '\t') {
				buf->pos.col -= TAB_SIZE;
			} else {
				buf->pos.col--;
			}
			buf->char_offset--;
		}
	} else {
		line_insert_string(buf->current_line, str, strlen(str), buf->char_offset);
		buf->pos.col += len;
		buf->char_offset += len;
	}
  ret:
	free(str);
	buf->dirty = true;
}

public void buffer_check_offset()
{
	buffer_t *buf = editor_buffer();
	line_t *ln = buf->current_line;
	static int offset = 0;
	offset = MAX(buf->char_offset, offset);
	if (ln->len > offset) {
		buffer_go_to_offset(offset);
	}
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
	if (offset >= buf->current_line->len) {
		return;
	}
	buf->char_offset = offset;
	buf->pos.col = offset_to_col(buf->current_line->str, buf->current_line->len, offset);
}

public void buffer_go_to_line(int index)
{
	buffer_t *buf = editor_buffer();
	if (index >= buf->line_count) {
		return;
	}
	buf->line_offset = index;
	buf->current_line = buffer_line_by_index(index);
	buf->pos.row = 1;
	buffer_check_offset();
	buf->render = true;
}

public char *buffer_current_char()
{
	buffer_t *buf = editor_buffer();
	return buf->current_line->str + buf->char_offset;
}

public char *buffer_char_at(int offset)
{
	buffer_t *buf = editor_buffer();
	return buf->current_line->str + offset;
}

public line_t *buffer_line_by_index(int index)
{
	line_t *line = editor_buffer()->first_line;
	for (int i = 0; i < index; i++) {
		line = L_LINK_NEXT(line);
	}
	return line;
}

public void buffer_delete_line(bool go_next)
{
	buffer_t *buf = editor_buffer();
	line_t *line = buf->current_line;
	if (go_next) {
		if (L_LINK_NEXT(line)) {
			next_line_command(NULL);
			buf->current_line = L_LINK_NEXT(line);
		}
	} else {
		if (L_LINK_PREV(line)) {
			prev_line_command(NULL);
			buf->current_line = L_LINK_PREV(line);
		}
	}
	L_LINK_REMOVE(line);
	line_free(line);
	buf->line_count--;
	buf->render = true;
}

public void buffer_open_line()
{
	buffer_t *buf = editor_buffer();
	line_t *current_line = buf->current_line;
	line_t *new_line = line_init("", 0);

	line_append_string(new_line, buffer_current_char(), current_line->len - buf->char_offset);
	line_delete_range(current_line, buf->char_offset, current_line->len);
	buffer_append_line(new_line);
	next_line_command(NULL);
	beginning_of_line_command(NULL);

	buf->render = true;
	buf->dirty = true;
}

public void buffer_free(buffer_t *buf)
{
	if (buf) {
		line_t *ln = buf->first_line;
		line_t *next = ln;
		while (next) {
			next = L_LINK_NEXT(ln);
			line_free(ln);
			ln = next;
		}
		if (buf->name) {
			free(buf->name);
		}
		if (buf->filepath) {
			free(buf->filepath);
		}
		free(buf);
	}
}
