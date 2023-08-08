#include "j.h"

public return_message j_command(char **args)
{
	tty_cursor_line_next();
	return create_return_message(true, "Next Line");
}
