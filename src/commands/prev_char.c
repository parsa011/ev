#include <stddef.h>
#include "commands/next_char.h"
#include "commands/prev_line.h"
#include "commands/end_of_line.h"
#include "editor.h"

public return_message prev_char_command(char **args)
{
	buffer_t *buf = editor_buffer();
	if (buf->char_offset == 0) {
		if (L_LINK_PREV(buf->current_line)) {
			prev_line_command(NULL);
			return end_of_line_command(NULL);
		}
		return create_return_message(ERROR, "no any line");
	}
	buf->char_offset--;
	if (*(buf->current_line->str + buf->char_offset) == '\t') {
		buf->pos.col -= TAB_SIZE;
	} else {
		buf->pos.col--;
	}
	return create_return_message(SUCCESS, "prev char");
}
