#include "o.h"
#include "../editor.h"
#include "../prompt.h"

public return_message o_command(char **args)
{
	char *file_path;
	if (ARRAY_LENGTH(args) == 1)
		file_path = args[0];
	else
		file_path = prompt_string("File Path : ");
	return editor_file_open(file_path);
}
