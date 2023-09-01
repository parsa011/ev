#include "commands/open_file.h"
#include "editor.h"
#include "prompt.h"

public return_message open_file_command(char **args)
{
	char *file_path = prompt_string("file path : ");
	buffer_t *buf = buffer_init(file_path, BUFFER_ROW);
	editor_buffer_append(buf);
	editor_buffer_change(buf);
	buffer_file_open(file_path);
	return create_return_message(SUCCESS, "file opened");
}
