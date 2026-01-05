#include <stdlib.h>
#include <string.h>
#include "commands/goto_line.h"

char *msg = "enter line number : ";

public return_message goto_line_command(char **args)
{
	int msg_len = strlen(msg);

    int *ln_nu = prompt_int(msg);
	while (!ln_nu) {
		prompt_message_show(msg, msg_len);
		ln_nu = prompt_int(msg);
	}
    if (!ln_nu) {
        return create_return_message(ERROR, "cancelled");
    }
    if (*ln_nu <= editor_buffer()->line_count) {
        buffer_go_to_line(*ln_nu - 1);
    }
	prompt_clear(true);
    if (ln_nu) {
        free(ln_nu);
    }
    return create_return_message(SUCCESS, "command done");
}
