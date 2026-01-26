#include <stdlib.h>
#include "commands/open_file.h"
#include "editor.h"
#include "prompt.h"

return_message_t open_file_command(char **args)
{
	char *file_path = prompt_string_with_base("open file : ", editor.running_path);
	if (!file_path) {
		return CREATE_RETURN_MESSAGE(TERMINATE, "canceled");
	}
	buffer_t *buf = buffer_init(file_path, BUFFER_ROW);
	editor_buffer_append(buf);
	editor_buffer_change(buf);
	buffer_file_open(file_path);
	if (file_path) {
		free(file_path);
	}
	return CREATE_RETURN_MESSAGE(SUCCESS, "file opened");
}
