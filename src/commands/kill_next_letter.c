#include <stddef.h>
#include "commands/kill_next_letter.h"
#include "commands/next_char.h"
#include "commands/do_backspace.h"
#include "key.h"
#include "buffer.h"

public return_message kill_next_letter_command(char **args)
{
	if (next_char_command(NULL).status == SUCCESS) {
		do_backspace_command(null);
	} else {
		return create_return_message(ERROR, "end of buffer");
	}
	return create_return_message(SUCCESS, "kill next letter");
}
