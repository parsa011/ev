#include <stddef.h>
#include "commands/end_of_line.h"
#include "editor.h"
#include "util.h"

public return_message end_of_line_command(char **args)
{
	buffer_t *buf = editor_buffer();
	line_t *cl = buf->current_line;
	buf->char_offset = cl->len;
	buf->pos.col = offset_to_col(cl->str, cl->len, cl->len);
	return create_return_message(SUCCESS, "end of line");
}
