#include <stddef.h>
#include "next_char.h"
#include "../editor.h"

public return_message prev_char_command(char **args)
{
	editor_buffer_t *buf = editor_buffer();
	if (buf->char_offset == 0)
		return create_return_message(ERROR, "beginning of line");
	buf->char_offset--;
	if (*(buf->current_line->str + buf->char_offset) == '\t')
		buf->pos.col -= TAB_SIZE;
	else
		buf->pos.col--;
	return create_return_message(SUCCESS, "prev char");
}
