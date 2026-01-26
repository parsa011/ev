#include <ctype.h>
#include "commands/upcase_word.h"

return_message_t upcase_word_command(char **args)
{
    buffer_t *buf = editor_buffer();
    char c = *buffer_current_char();
    if (!isalpha(c)) {
        if (next_word_command(NULL).status != SUCCESS) {
            return CREATE_RETURN_MESSAGE(ERROR, "Oops, no any character");
        }
    }
    c = *buffer_current_char();
    while (isalpha(c)) {
        if (!isupper(c)) {
            line_replace_char(buf->current_line, toupper(c), buf->char_offset);
        }
        if (next_char_command(NULL).status != SUCCESS) {
            break;
        }
        c = *buffer_current_char();
    }
    buf->render = true;
    return CREATE_RETURN_MESSAGE(SUCCESS, "done");
}