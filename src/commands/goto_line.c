#include <stdlib.h>
#include "commands/goto_line.h"

public return_message goto_line_command(char **args)
{
    int *ln_nu = prompt_int("enter line number : ");
    if (!ln_nu) {
        return create_return_message(ERROR, "cancelled");
    }
    if (*ln_nu <= editor_buffer()->line_count) {
        buffer_go_to_line(*ln_nu - 1);
    }
    if (ln_nu) {
        free(ln_nu);
    }
    return create_return_message(SUCCESS, "command done");
}