#include "command.h"
#include "tty.h"
#include "commands/commands.h"
#include <stdio.h>
#include <string.h>

command commands[] = {
	MAKE_COMMAND("close buffer", "q", q_command),
	MAKE_COMMAND("open file into a buffer", "o", NULL)
};

command command_read()
{
	static char buffer[32];
	static int buffer_len;
	buffer[buffer_len] = tty_get_char();
	return command_get(buffer);
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

void command_print(command cmd)
{
	printf("%s\t%s\r\n", cmd.key_codes, cmd.desc);
}
