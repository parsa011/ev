#include <stddef.h>
#include "end_of_line.h"
#include "../editor.h"
#include "../util.h"

public return_message beginning_of_line_command(char **args)
{
	editor_buffer_t *buf = editor_buffer();
	buf->char_offset = 0;
	buf->pos.col = 1;
	return create_return_message(SUCCESS, "next char");
}
