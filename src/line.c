#include <stdlib.h>
#include <string.h>
#include "line.h"

public line_t *line_init(char *str, int len)
{
	line_t *line = (line_t *) malloc(sizeof(line_t));
	line->str = (char *) malloc((len + 1) * sizeof(char));
	strncpy(line->str, str, len);
	line->len = len;
	return line;
}

public void line_insert_string(line_t *line, char *str, uint16_t pos)
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

public void line_delete_char(line_t *line, uint16_t pos)
{
	if (pos < 0)
		return;
	for (int i = pos; i < line->len - 1; i++) {
		line->str[i] = line->str[i + 1];
	}
	line->len--;
	line->str = (char *) realloc(line->str, (line->len + 1) * sizeof(char));
	line->str[line->len] = '\0';
}
