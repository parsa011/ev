#include <stddef.h>
#include "commands/kill_next_letter.h"
#include "commands/next_char.h"
#include "commands/do_backspace.h"
#include "key.h"
#include "buffer.h"

return_message_t kill_next_letter_command(char **args)
{
	if (next_char_command(NULL).status == SUCCESS) {
		do_backspace_command(null);
	} else {
		return CREATE_RETURN_MESSAGE(ERROR, "end of buffer");
	}
	return CREATE_RETURN_MESSAGE(SUCCESS, "kill next letter");
}
