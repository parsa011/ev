#include <stddef.h>
#include "commands/beginning_of_line.h"
#include "editor.h"
#include "util.h"

return_message_t beginning_of_buffer_command(char **args)
{
    buffer_go_to_line(0);
	return CREATE_RETURN_MESSAGE(SUCCESS, "beginning of buffer");
}