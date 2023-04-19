#include "editor.h"
#include "prompt.h"
#include "tty.h"
#include "command.h"
#include "key.h"
#include "commands/commands.h"
#include <string.h>
#include <stdio.h>

private editor_t editor;

void editor_init()
{
	tty_raw_mode();
}

void editor_close()
{
	tty_cooked_mode();
}

return_message editor_run()
{
	while (true) {
		command cmd = command_read();
		command_print(cmd);
		if (strcmp(cmd.key_codes, "q") == 0) {
			q_command(null);
		}
	}
	return create_return_message(SUCCESS, "editor closed without error");
}

return_message editor_file_open(char *file_path)
{
	if (strlen(file_path)) {
		if (!prompt_bool("a file is open, are you sure ?"))
			return create_return_message(TERMINATE, "operation terminated");
	}
	bool save = prompt_bool("file changed, save ?");
	editor.file_path = file_path;
	if (save)
		editor_file_save();
	return create_return_message(SUCCESS, "file opened");
}

return_message editor_file_close()
{
	return create_return_message(SUCCESS, "file closed");
}

return_message editor_file_save()
{
	return create_return_message(SUCCESS, "file saved");
}
