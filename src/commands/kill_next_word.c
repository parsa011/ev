#include <ctype.h>
#include <stddef.h>
#include "commands/kill_next_word.h"
#include "commands/kill_next_letter.h"
#include "editor.h"

public return_message kill_next_word_command(char **args)
{
#define IN_WORD isalpha(buffer_current_char())
	if (IN_WORD) {
		do {
			if (!IN_WORD) {
				break;
			}
		} while(kill_next_letter_command(NULL).status == SUCCESS);
	} else {
		do {
			if (IN_WORD) {
				break;
			}
		} while(kill_next_letter_command(NULL).status == SUCCESS);
	}
#undef IN_WORD
	return create_return_message(SUCCESS, "kill word");
}
