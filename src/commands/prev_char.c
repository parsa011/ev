#include <stddef.h>
#include "commands/next_char.h"
#include "commands/prev_line.h"
#include "commands/end_of_line.h"
#include "editor.h"

return_message_t prev_char_command(char **args)
{
	buffer_t *buf = editor_buffer();
	if (buf->char_offset == 0) {
		if (L_LINK_PREV(buf->current_line)) {
			prev_line_command(NULL);
			return end_of_line_command(NULL);
		}
		return CREATE_RETURN_MESSAGE(ERROR, "no any line");
	}
	buf->char_offset--;
	if (*buffer_current_char() == '\t') {
		buf->pos.col -= TAB_SIZE;
	} else {
		buf->pos.col--;
	}
	return CREATE_RETURN_MESSAGE(SUCCESS, "prev char");
}
