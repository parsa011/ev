#include "kill_line.h"

public return_message kill_line_command(char **args)
{
	buffer_t *buf = editor_buffer();
	line_t *current_line = buf->current_line;

	bool is_end = buf->char_offset + 1 == current_line->len;
	line_delete_range(current_line, buf->char_offset, current_line->len);
	if (is_end) {
	}
		// TODO : append next line to current line and remove it
	buf->render = true;
	return create_return_message(SUCCESS, "kill line");
}
