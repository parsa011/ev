#include "command.h"
#include "tty.h"
#include "key.h"
#include "editor.h"
#include "commands/commands.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

command commands[] = {
	MAKE_COMMAND("Close Buffer", "q", q_command),
	MAKE_COMMAND("Open File Into a Buffer", "oa", o_command),
	MAKE_COMMAND("Goto Next Line", "j", j_command),
	MAKE_COMMAND("Goto Next Line", "PAGE_UP", j_command),
};

public command command_read()
{
	int c = key_read();
	char *str = key_to_str(c);
	if (str) {
		printf("%s\n", key_to_str(c));
	}
	if (c == 'q') {
		editor_close();
		exit(0);
	}
	//static char buffer[32];
	//static int buffer_len;
//read_again :
	//buffer[buffer_len++] = key_read();
	//buffer[buffer_len] = '\0';
//	command cmd = command_get(buffer);
//	if (cmd.func == null) {
//		if (command_exists(buffer))
//			goto read_again;
//		else
//			buffer[buffer_len = 0] = '\0';
//	} else
//		buffer[buffer_len = 0] = '\0';
	return EMPTY_COMMAND;
}

public command command_get(char *pattern)
{
	int len = sizeof(commands) / sizeof(commands[0]);
	for (int i = 0; i < len; i++) {
		if (strcmp(commands[i].key_codes, pattern) == 0)
			return commands[i];
	}
	return MAKE_COMMAND("Empty Command", "[NULL]", NULL);
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
	printf("%s\t%s\r\n", cmd.key_codes, cmd.desc);
}
