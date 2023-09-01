#include "commands/prev_buffer.h"
#include "editor.h"

public return_message prev_buffer_command(char **args)
{
	buffer_t *prev_buf = L_LINK_PREV(editor_buffer());
	if (prev_buf) {
		editor_buffer_change(prev_buf);
		return create_return_message(SUCCESS, "prev buffer");
	} else {
		return create_return_message(ERROR, "first buffer");
	}
}
