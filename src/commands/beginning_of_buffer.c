#include <stddef.h>
#include "commands/beginning_of_line.h"
#include "editor.h"
#include "util.h"

public return_message beginning_of_buffer_command(char **args)
{
    buffer_go_to_line(0);
	return create_return_message(SUCCESS, "beginning of buffer");
}