#include "editor.h"
#include "prompt.h"
#include "tty.h"
#include "command.h"
#include "key.h"
#include "commands/commands.h"
#include <string.h>
#include <stdio.h>

private editor_t editor;

public void editor_init()
{
	tty_raw_mode();
	tty_clear();
	tty_window_size(&editor.rows, &editor.cols);
}

public void editor_change_size()
{
	tty_window_size(&editor.rows, &editor.cols);
}

public void editor_close()
{
	tty_cooked_mode();
}

public return_message editor_run()
{
	while (true) {
		command cmd = command_read();
		command_print(cmd);
		if (cmd.func != null)
			cmd.func(null);
	}
	return create_return_message(SUCCESS, "editor closed without error");
}

public return_message editor_file_open(char *file_path)
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

public return_message editor_file_close()
{
	return create_return_message(SUCCESS, "file closed");
}

public return_message editor_file_save()
{
	return create_return_message(SUCCESS, "file saved");
}
