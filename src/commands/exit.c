#include <stdlib.h>
#include "commands/exit.h"
#include "editor.h"

public return_message exit_command(char **args)
{
	editor_close();
	exit(0);
	return create_return_message(true, "Usless Message But Exited Successfuly");
}
