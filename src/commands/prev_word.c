#include <stddef.h>
#include <ctype.h>
#include "commands/prev_word.h"
#include "commands/prev_char.h"
#include "commands/next_char.h"

public return_message prev_word_command(char **args)
{
#define IN_WORD isalpha(*buffer_current_char())
	if (IN_WORD) {
		do {
			if (!IN_WORD) {
				break;
			}
		} while(prev_char_command(NULL).status == SUCCESS);
	} else {
		bool passed_any_word = false;
		do {
			if (IN_WORD) {
				passed_any_word = true;
			}
			if (passed_any_word && !IN_WORD) {
				break;
			}
		} while(prev_char_command(NULL).status == SUCCESS);
	}
#undef IN_WORD
	return create_return_message(SUCCESS, "next word");
}
