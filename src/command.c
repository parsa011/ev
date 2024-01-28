#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "command.h"
#include "tty.h"
#include "key.h"
#include "editor.h"
#include "commands/commands.h"

command commands[] = {
	MAKE_COMMAND("exit", "C-X C-C", exit_command),
	MAKE_COMMAND("next-line", "C-N", next_line_command),
	MAKE_COMMAND("prev-line", "C-P", prev_line_command),
	MAKE_COMMAND("next-char", "C-F", next_char_command),
	MAKE_COMMAND("prev-char", "C-B", prev_char_command),

	MAKE_COMMAND("next-line", "ARROW_DOWN", next_line_command),
	MAKE_COMMAND("prev-line", "ARROW_UP", prev_line_command),
	MAKE_COMMAND("next-char", "ARROW_RIGHT", next_char_command),
	MAKE_COMMAND("prev-char", "ARROW_LEFT", prev_char_command),

	MAKE_COMMAND("eol", "C-E", end_of_line_command),
	MAKE_COMMAND("bol", "C-A", beginning_of_line_command),
	MAKE_COMMAND("eol", "END", end_of_line_command),
	MAKE_COMMAND("bol", "HOME", beginning_of_line_command),

	MAKE_COMMAND("new-line", "C-M", new_line_command),
	MAKE_COMMAND("kill-line", "C-K", kill_line_command),
	MAKE_COMMAND("kill-next-letter", "C-D", kill_next_letter_command),
	MAKE_COMMAND("kill-next-letter", "DELETE", kill_next_letter_command),
	MAKE_COMMAND("kill-next-word", "M-d", kill_next_word_command),

	MAKE_COMMAND("next-word", "M-f", next_word_command),
	MAKE_COMMAND("prev-word", "M-b", prev_word_command),

	MAKE_COMMAND("page-down", "PAGE_DOWN", page_down_command),
	MAKE_COMMAND("page-up", "PAGE_UP", page_up_command),
	MAKE_COMMAND("page-down", "C-V", page_down_command),
	MAKE_COMMAND("page-up", "M-v", page_up_command),
	MAKE_COMMAND("end-of-buffer", "ESC >", end_of_buffer_command),
	MAKE_COMMAND("beginning-of-buffer", "ESC <", beginning_of_buffer_command),

	MAKE_COMMAND("save-file", "C-X C-S", save_file_command),
	MAKE_COMMAND("open-file", "C-X C-F", open_file_command),

	MAKE_COMMAND("next-buffer", "C-X b n", next_buffer_command),
	MAKE_COMMAND("prev-buffer", "C-X b p", prev_buffer_command),
	MAKE_COMMAND("kill-buffer", "C-X k", kill_buffer_command)
};

public command command_read(char *str)
{
	command cmd = command_get(str);
	int c;
	while (cmd.func == null && command_exists(str)) {
		c = key_read();
		key_combine(str, c);
		cmd = command_get(str);
	}
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
