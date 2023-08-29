#include "commands/new_line.h"
#include "buffer.h"

public return_message new_line_command(char **args)
{
	buffer_open_line();
	return create_return_message(SUCCESS, "new line");
}
