#include "base.h"
#include "q.h"
#include "../editor.h"
#include <stdlib.h>

return_message q_command(char **args)
{
	editor_close();
	exit(0);
	return create_return_message(true, "usless but exited successfuly");
}
