#include <ctype.h>
#include <stddef.h>
#include "commands/kill_next_word.h"
#include "commands/kill_next_letter.h"
#include "editor.h"

return_message_t kill_next_word_command(char **args)
{
#define IN_WORD isalpha(*buffer_current_char())
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
	return CREATE_RETURN_MESSAGE(SUCCESS, "kill word");
}
