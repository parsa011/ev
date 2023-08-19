#include <stddef.h>
#include "next_char.h"
#include "next_line.h"
#include "beginning_of_line.h"
#include "../editor.h"

public return_message next_char_command(char **args)
{
	editor_buffer_t *buf = editor_buffer();
	editor_buffer_line_t *cl = buf->current_line;
	/*
	 * if buffer row is less than editor row count, go to next line
	 * otherwise scroll one line to down (if there is any more lines)
	 */
	if (buf->char_offset + 1 > cl->len) {
		next_line_command(NULL);
		return beginning_of_line_command(NULL);
	}
	if (*(buf->current_line->str + buf->char_offset) == '\t')
		buf->pos.col += TAB_SIZE;
	else
		buf->pos.col++;
	buf->char_offset++;
	return create_return_message(SUCCESS, "next char");
}
