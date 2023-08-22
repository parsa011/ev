#include "kill_line.h"

public return_message kill_line_command(char **args)
{
	buffer_t *buf = editor_buffer();
	line_delete_range(buf->current_line, buf->char_offset, buf->current_line->len);
	buf->render = true;
	return create_return_message(SUCCESS, "kill line");
}
