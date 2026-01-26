#include <stdlib.h>
#include "commands/exit.h"
#include "commands/save_file.h"
#include "editor.h"
#include "buffer.h"
#include "prompt.h"

return_message_t exit_command(char **args)
{
	buffer_t *buf = editor_buffer();
	if (buf->dirty) {
		bool ans = prompt_bool("buffer modified; save file ?");
		if (ans) {
			save_file_command(NULL);
		}
		bool exit_anyway = prompt_bool("Modified Buffer exist; exit anyway ?");
		if (!exit_anyway) {
			return CREATE_RETURN_MESSAGE(ERROR, "cancelled");
		}
	}
	editor.exit = true;
	return CREATE_RETURN_MESSAGE(SUCCESS, "Usless Message But Exited Successfuly");
}
