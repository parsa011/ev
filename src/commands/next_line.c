#include <stddef.h>
#include "commands/next_line.h"
#include "editor.h"
#include "buffer.h"

return_message_t next_line_command(char **args)
{
	buffer_t *buf = editor_buffer();
	/*
	 * if buffer row is less than editor row count, go to next line
	 * otherwise scroll one line to down (if there is any more lines)
	 */
	if (L_LINK_NEXT(buf->current_line) == NULL) {
		return CREATE_RETURN_MESSAGE(ERROR, "end of buffer");
	}
	if (buf->pos.row + 1 < buf->rows) {
		buf->pos.row++;
		tty_cursor_line_next();
	} else {
		if (buf->line_offset + 1 >= buf->line_count) {
			return CREATE_RETURN_MESSAGE(ERROR, "end of buffer");
		}
		buf->line_offset++;
		buf->render = true;
	}
	buf->current_line = L_LINK_NEXT(buf->current_line);
	buffer_check_offset();
	return CREATE_RETURN_MESSAGE(SUCCESS, "Next Line");
}
