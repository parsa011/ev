#include "commands/new_line.h"
#include "buffer.h"

return_message_t new_line_command(char **args)
{
	buffer_open_line();
	return CREATE_RETURN_MESSAGE(SUCCESS, "new line");
}
