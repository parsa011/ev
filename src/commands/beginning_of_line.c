#include <stddef.h>
#include "commands/end_of_line.h"
#include "editor.h"
#include "util.h"

return_message_t beginning_of_line_command(char **args)
{
	buffer_t *buf = editor_buffer();
	buf->char_offset = 0;
	buf->pos.col = 1;
	return CREATE_RETURN_MESSAGE(SUCCESS, "beginning of line");
}
