#include "commands/page_down.h"
#include "editor.h"

return_message_t page_down_command(char **args)
{
	buffer_t *buf = editor_buffer();
	if (buf->line_offset + 1 + editor.rows > buf->line_count) {
		buffer_go_to_line(buf->line_count - 1);
	}
	buffer_go_to_line(buf->line_offset + editor.rows);
	return CREATE_RETURN_MESSAGE(SUCCESS, "page down");
}
