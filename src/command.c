#include "command.h"
#include "tty.h"
#include "commands/commands.h"
#include <stdio.h>
#include <string.h>

command commands[] = {
	MAKE_COMMAND("close buffer", "q", q_command),
	MAKE_COMMAND("open file into a buffer", "oa", q_command)
};

command command_read()
{
	static char buffer[32];
	static int buffer_len;
get_again :
	buffer[buffer_len++] = tty_get_char();
	buffer[buffer_len] = '\0';
	command cmd = command_get(buffer);
	if (cmd.func == null) {
		if (command_exists(buffer))
			goto get_again;
		else
			buffer[buffer_len = 0] = '\0';
	} else
		buffer[buffer_len = 0] = '\0';
	return cmd;
}

command command_get(char *pattern)
{
	int len = sizeof(commands) / sizeof(commands[0]);
	for (int i = 0; i < len; i++) {
		if (strcmp(commands[i].key_codes, pattern) == 0)
			return commands[i];
	}
	return MAKE_COMMAND("Empty Command", "[NULL]", NULL);
}

bool command_exists(char *pattern)
{
	int len = sizeof(commands) / sizeof(commands[0]);
	int str_len = strlen(pattern);
	for (int i = 0; i < len; i++) {
		if (strncmp(commands[i].key_codes, pattern, str_len) == 0)
			return true;
	}
	return false;
}

void command_print(command cmd)
{
	printf("%s\t%s\r\n", cmd.key_codes, cmd.desc);
}
