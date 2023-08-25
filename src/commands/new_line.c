#include "new_line.h"
#include "../line.h"

public return_message new_line_command(char **args)
{
	line_open();
	return create_return_message(SUCCESS, "new line");
}
