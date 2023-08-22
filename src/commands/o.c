#include "o.h"
#include "../editor.h"
#include "../prompt.h"

public return_message o_command(char **args)
{
	return create_return_message(SUCCESS, "hi"); 
}
