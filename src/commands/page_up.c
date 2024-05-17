#include "commands/page_up.h"
#include "editor.h"

public return_message page_up_command(char **args)
{
	buffer_t *buf = editor_buffer();
	if (buf->line_offset <= editor.rows) {
		buffer_go_to_line(0);
	} else {
		buffer_go_to_line(buf->line_offset - editor.rows);
	}
	return create_return_message(SUCCESS, "page up");
}
