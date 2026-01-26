#include <stddef.h>
#include "commands/prev_line.h"
#include "editor.h"
#include "buffer.h"

return_message_t prev_line_command(char **args)
{
	buffer_t *buf = editor_buffer();
	if (L_LINK_PREV(buf->current_line) == NULL || (buf->pos.row == 1 && buf->line_offset == 0)) {
		return CREATE_RETURN_MESSAGE(ERROR, "beginning of buffer");
	}
	if (buf->pos.row > 1) {
		buf->pos.row--;
	} else {
		buf->line_offset--;
		buf->render = true;
	}
	buf->current_line = L_LINK_PREV(buf->current_line);
	buffer_check_offset();
	return CREATE_RETURN_MESSAGE(SUCCESS, "prev line");
}
