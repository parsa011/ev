#include <stdlib.h>
#include "exit.h"
#include "../editor.h"

public return_message exit_command(char **args)
{
	editor_close();
	exit(0);
	return create_return_message(true, "Usless Message But Exited Successfuly");
}
