#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include "editor.h"
#include "prompt.h"
#include "tty.h"
#include "command.h"
#include "key.h"
#include "file.h"
#include "commands/commands.h"

editor_t editor;

public void editor_init()
{
	tty_raw_mode();
	tty_clear();

	editor_change_size();
	editor.tty_in = STDIN_FILENO;
	editor.tty_out = STDOUT_FILENO;

	/*
	 * init base stuff for editor, like buffer
	 */
	editor.current_buffer = editor_buffer_init(NULL);
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

public editor_buffer_t *editor_buffer()
{
	return editor.current_buffer;
}

public editor_buffer_t *editor_buffer_init(char *path)
{
	editor_buffer_t *buffer = (editor_buffer_t *) malloc(sizeof(editor_buffer_t));
	assert(buffer);
	return buffer;
}

public return_message editor_file_open(char *file_path)
{
	editor_buffer_t *buf = editor_buffer();
	buf->filepath = file_path;
	buf->name = file_name(file_path);
	tty_put_string(true, buf->filepath);
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
