#include <stddef.h>
#include "commands/next_line.h"
#include "editor.h"
#include "buffer.h"

public return_message next_line_command(char **args)
{
	buffer_t *buf = editor_buffer();
	/*
	 * if buffer row is less than editor row count, go to next line
	 * otherwise scroll one line to down (if there is any more lines)
	 */
	if (L_LINK_NEXT(buf->current_line) == NULL)
		return create_return_message(ERROR, "end of buffer");
	if (buf->pos.row < editor.rows - 1) {
		buf->pos.row++;
		tty_cursor_line_next();
	} else {
		if (buf->line_offset + 1 >= buf->line_count)
			return create_return_message(ERROR, "end of buffer");
		buf->line_offset++;
		buf->render = true;
	}
	buf->current_line = L_LINK_NEXT(buf->current_line);
	buffer_check_offset();
	return create_return_message(SUCCESS, "Next Line");
}
