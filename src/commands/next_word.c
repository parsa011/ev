#include <stddef.h>
#include <ctype.h>
#include "commands/next_word.h"
#include "commands/next_char.h"
#include "buffer.h"

return_message_t next_word_command(char **args)
{
#define IN_WORD isalpha(*buffer_current_char())
	if (IN_WORD) {
		do {
			if (!IN_WORD) {
				break;
			}
		} while(next_char_command(NULL).status == SUCCESS);
	} else {
		do {
			if (IN_WORD) {
				break;
			}
		} while(next_char_command(NULL).status == SUCCESS);
	}
#undef IN_WORD
	return CREATE_RETURN_MESSAGE(SUCCESS, "next word");
}
