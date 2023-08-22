#ifndef _LINE_H
# define _LINE_H

#include "basic.h"
#include "../llink/llist.h"

struct line { 
	L_LINK(line_t) link;

	char *str;
	int len;
};

/*
 * initialie new buffer line and copy given string to line str
 */
public line_t *line_init(char *str, int len);

/*
 * insert str into line with given position
 */
public void line_insert_string(line_t *line, char *str, uint16_t pos);

/*
 * delete from given in string, return if pos is 0
 */
public void line_delete_char(line_t *line, uint16_t pos);

#endif
