#include "editor.h"
#include "commands/do_backspace.h"

return_message_t do_backspace_command(char **args)
{
	buffer_t *buf = editor_buffer();
	if (buf->char_offset == 0) {
		line_t *prev_line = L_LINK_PREV(buf->current_line);
		if (!prev_line) {
			return CREATE_RETURN_MESSAGE(SUCCESS, "");
		}
		int prev_line_len = prev_line->len;
		if (buf->current_line->len > 0) {
			line_append_string(prev_line, buf->current_line->str, buf->current_line->len);
		}
		buffer_delete_line(false);
		buffer_go_to_offset(prev_line_len);
		buffer_dirty();
	} else {
		char prev_char = *buffer_char_at(buf->char_offset - 1);
		line_delete_char(buf->current_line, buf->char_offset - 1);
		if (prev_char == '\t') {
			buf->pos.col -= TAB_SIZE;
		} else {
			buf->pos.col--;
		}
		buf->char_offset--;
		buffer_dirty();
	}
	return CREATE_RETURN_MESSAGE(SUCCESS, "");
}
