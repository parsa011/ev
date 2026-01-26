#include "commands/kill_buffer.h"
#include "editor.h"
#include "buffer.h"

return_message_t kill_buffer_command(char **args)
{
	buffer_t *buf = editor_buffer();
	buffer_t *new_buf = !L_LINK_NEXT(buf) ? L_LINK_PREV(buf) : L_LINK_NEXT(buf);
	if (!new_buf) {
		editor.exit = true;
		return CREATE_RETURN_MESSAGE(SUCCESS, "last buffer, exiting");
	}
	editor_buffer_change(new_buf);
	L_LINK_REMOVE(buf);
	buffer_free(buf);
	return CREATE_RETURN_MESSAGE(SUCCESS, "killed buffer");
}
