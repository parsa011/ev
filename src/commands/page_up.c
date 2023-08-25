#include "commands/page_up.h"
#include "editor.h"

public return_message page_up_command(char **args)
{
	buffer_t *buf = editor_buffer();
	if (buf->line_offset + 1 - editor.rows < 1)
		buffer_go_to_line(0);
	buffer_go_to_line(buf->line_offset - editor.rows);
	return create_return_message(SUCCESS, "page up");
}
