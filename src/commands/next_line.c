#include <stddef.h>
#include "next_line.h"
#include "../editor.h"

public return_message next_line_command(char **args)
{
	editor_buffer_t *buf = editor_buffer();
	/*
	 * if buffer row is less than editor row count, go to next line
	 * otherwise scroll one line to down (if there is any more lines)
	 */
	if (L_LINK_NEXT(buf->current_line) == NULL)
		return create_return_message(ERROR, "end of buffer");
	if (buf->pos.row < editor.rows) {
		buf->pos.row++;
		buf->current_line = L_LINK_NEXT(buf->current_line);
		tty_cursor_line_next();
	}
	return create_return_message(SUCCESS, "Next Line");
}
