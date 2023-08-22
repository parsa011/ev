#include <stddef.h>
#include "prev_line.h"
#include "../editor.h"

public return_message prev_line_command(char **args)
{
	buffer_t *buf = editor_buffer();
	if (L_LINK_PREV(buf->current_line) == NULL)
		return create_return_message(ERROR, "beginning of buffer");
	if (buf->pos.row > 1) {
		buf->pos.row--;
	} else {
		if (buf->line_offset == 0) {
			return create_return_message(ERROR, "beginning of buffer");
		}
		buf->line_offset--;
		buf->render = true;
	}
	buf->current_line = L_LINK_PREV(buf->current_line);
	editor_check_offset();
	return create_return_message(SUCCESS, "prev line");
}
