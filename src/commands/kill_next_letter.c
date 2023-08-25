#include <stddef.h>
#include "kill_next_letter.h"
#include "next_char.h"
#include "../key.h"
#include "../buffer.h"

public return_message kill_next_letter_command(char **args)
{
	if (next_char_command(NULL).status == SUCCESS)
		buffer_insert_key(BACKSPACE);
	return create_return_message(SUCCESS, "kill next letter");
}
