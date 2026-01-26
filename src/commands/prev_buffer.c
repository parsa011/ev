#include "commands/prev_buffer.h"
#include "editor.h"

return_message_t prev_buffer_command(char **args)
{
	buffer_t *prev_buf = L_LINK_PREV(editor_buffer());
	if (prev_buf) {
		editor_buffer_change(prev_buf);
		return CREATE_RETURN_MESSAGE(SUCCESS, "prev buffer");
	} else {
		return CREATE_RETURN_MESSAGE(ERROR, "first buffer");
	}
}
