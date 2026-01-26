#include <stddef.h>
#include "commands/end_of_line.h"
#include "editor.h"
#include "util.h"

return_message_t end_of_buffer_command(char **args)
{
    buffer_go_to_line(editor_buffer()->line_count - 1);
	return CREATE_RETURN_MESSAGE(SUCCESS, "end of buffer");
}