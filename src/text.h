#ifndef _TEXT_H
# define _TEXT_H

#include "basic.h"
#include "editor.h"

/*
 * insert str into line with given position
 */
public void text_insert_string(editor_buffer_line_t *line, char *str, uint16_t pos);

#endif
