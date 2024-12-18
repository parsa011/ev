#include <stddef.h>
#include "commands/next_char.h"
#include "commands/next_line.h"
#include "commands/beginning_of_line.h"
#include "editor.h"

public return_message next_char_command(char **args)
{
	buffer_t *buf = editor_buffer();
	line_t *cl = buf->current_line;
	if (buf->char_offset + 1 > cl->len) {
		if (next_line_command(NULL).status == SUCCESS) {
			return beginning_of_line_command(NULL);
		} else {
			return create_return_message(ERROR, "end of buffer");
		}
	}
	if (*buffer_current_char() == '\t') {
		buf->pos.col += TAB_SIZE;
	} else {
		buf->pos.col++;
	}
	buf->char_offset++;
	return create_return_message(SUCCESS, "next char");
}
