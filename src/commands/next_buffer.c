#include "commands/next_buffer.h"
#include "editor.h"

public return_message next_buffer_command(char **args)
{
	buffer_t *next_buf = L_LINK_NEXT(editor_buffer());
	if (next_buf) {
		editor_buffer_change(next_buf);
		return create_return_message(SUCCESS, "next buffer");
	} else {
		return create_return_message(ERROR, "last buffer");
	}
}
