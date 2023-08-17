#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "command.h"
#include "tty.h"
#include "key.h"
#include "editor.h"
#include "commands/commands.h"

command commands[] = {
	MAKE_COMMAND("Close Buffer", "C-X C-C", q_command),
	MAKE_COMMAND("Close Buffer", "q", q_command),
	MAKE_COMMAND("Open File Into a Buffer", "o a", o_command),
	MAKE_COMMAND("Goto Next Line", "C-J", next_line_command),
};

public command command_read()
{
	int c = key_read();
	char *str = key_to_str(c);
	command cmd = command_get(str);
	while (cmd.func == null && command_exists(str)) {
		c = key_read();
		key_combine(str, c);
		cmd = command_get(str);
	}
	if (str)
		free(str);
	return cmd;
}

public command command_get(char *pattern)
{
	int len = sizeof(commands) / sizeof(commands[0]);
	for (int i = 0; i < len; i++) {
		if (strcmp(commands[i].key_codes, pattern) == 0)
			return commands[i];
	}
	return MAKE_COMMAND("Empty Command", "NULL", NULL);
}

public bool command_exists(char *pattern)
{
	int len = sizeof(commands) / sizeof(commands[0]);
	int str_len = strlen(pattern);
	for (int i = 0; i < len; i++) {
		if (strncmp(commands[i].key_codes, pattern, str_len) == 0)
			return true;
	}
	return false;
}

public void command_print(command cmd)
{
	tty_put_string(true, "[%s] --> %s\r\n", cmd.key_codes, cmd.desc);
}
