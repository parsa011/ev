#include "commands/kill_buffer.h"
#include "editor.h"
#include "buffer.h"

public return_message kill_buffer_command(char **args)
{
	buffer_t *buf = editor_buffer();
	buffer_t *new_buf = L_LINK_NEXT(buf);
	if (!new_buf) {
		new_buf = L_LINK_PREV(buf);
	}
	if (!new_buf) {
		editor.exit = true;
		return create_return_message(SUCCESS, "last buffer, exiting");
	}
	editor_buffer_change(new_buf);
	L_LINK_REMOVE(buf);
	buffer_free(buf);
	return create_return_message(SUCCESS, "killed buffer");
}
