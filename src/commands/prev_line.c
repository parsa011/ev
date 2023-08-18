#include <stddef.h>
#include "prev_line.h"
#include "../editor.h"

public return_message prev_line_command(char **args)
{
	editor_buffer_t *buf = editor_buffer();
	/*
	 * if buffer row is less than editor row count, go to next line
	 * otherwise scroll one line to down (if there is any more lines)
	 */
	if (L_LINK_PREV(buf->current_line) == NULL)
		return create_return_message(ERROR, "beginning of buffer");
	if (buf->pos.row > 1) {
		buf->pos.row--;
		buf->current_line = L_LINK_PREV(buf->current_line);
		tty_cursor_line_prev();
	}
	return create_return_message(SUCCESS, "prev line");
}