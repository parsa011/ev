#include <ctype.h>
#include "commands/downcase_word.h"

public return_message downcase_word_command(char **args)
{
    buffer_t *buf = editor_buffer();
    char c = *buffer_current_char();
    if (!isalpha(c)) {
        if (next_word_command(NULL).status != SUCCESS) {
            return create_return_message(ERROR, "Oops, no any character");
        }
    }
    c = *buffer_current_char();
    while (isalpha(c)) {
        if (!islower(c)) {
            line_replace_char(buf->current_line, tolower(c), buf->char_offset);
        }
        if (next_char_command(NULL).status != SUCCESS) {
            break;
        }
        c = *buffer_current_char();
    }
    buf->render = true;
    return create_return_message(SUCCESS, "done");
}