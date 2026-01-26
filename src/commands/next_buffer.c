#include "commands/next_buffer.h"
#include "editor.h"

return_message_t next_buffer_command(char **args)
{
	buffer_t *next_buf = L_LINK_NEXT(editor_buffer());
	if (next_buf) {
		editor_buffer_change(next_buf);
		return CREATE_RETURN_MESSAGE(SUCCESS, "next buffer");
	} else {
		return CREATE_RETURN_MESSAGE(ERROR, "last buffer");
	}
}
