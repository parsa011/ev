#include <stddef.h>
#include "commands/kill_line.h"
#include "commands/next_char.h"
#include "commands/next_char.h"
#include "buffer.h"
#include "key.h"

public return_message kill_line_command(char **args)
{
	buffer_t *buf = editor_buffer();
	line_t *current_line = buf->current_line;

	bool is_end = buf->char_offset == current_line->len;
	if (is_end) {
		if (next_char_command(NULL).status == SUCCESS) {
			buffer_insert_key(BACKSPACE);
			next_char_command(null);
		}
	} else {
		line_delete_range(current_line, buf->char_offset, current_line->len);
	}
	buf->render = true;
	return create_return_message(SUCCESS, "kill line");
}
