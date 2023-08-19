#include <stddef.h>
#include "end_of_line.h"
#include "../editor.h"
#include "../util.h"

public return_message end_of_line_command(char **args)
{
	editor_buffer_t *buf = editor_buffer();
	editor_buffer_line_t *cl = buf->current_line;
	buf->char_offset = cl->len - 1;
	buf->pos.col = string_len_to_offset(cl->str, cl->len);
	return create_return_message(SUCCESS, "end of line");
}
