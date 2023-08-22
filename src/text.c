#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "text.h"
#include "log.h"

public void text_insert_string(editor_buffer_line_t *line, char *str, uint16_t pos)
{
	int str_len = strlen(str);
	int dest_len = strlen(line->str);
	int len = str_len + dest_len;
	line->str = (char *) realloc(line->str, (len + 1) * sizeof(char));
	for (int i = len - 1; i > pos; i--) {
		line->str[i] = line->str[i - str_len];
	}
	char *ptr = line->str + pos;
	while (*str != '\0') {
		*ptr++ = *str++;
	}
	line->str[len] = '\0';
	line->len = len;
}
