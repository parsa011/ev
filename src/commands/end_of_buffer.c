#include <stddef.h>
#include "commands/end_of_line.h"
#include "editor.h"
#include "util.h"

public return_message end_of_buffer_command(char **args)
{
    buffer_go_to_line(editor_buffer()->line_count - 1);
	return create_return_message(SUCCESS, "end of buffer");
}