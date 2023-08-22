#include <stdlib.h>
#include <string.h>
#include "line.h"
#include "commands/commands.h"

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

public void line_append_string(line_t *line, char *str)
{
	int str_len = strlen(str);
	int dest_len = strlen(line->str);
	int len = str_len + dest_len + 1;
	line->str = (char *) realloc(line->str, (len + 1) * sizeof(char));
	strcat(line->str, str);
	line->len = len;
}

public void line_delete(bool go_next)
{
	buffer_t *buf = editor_buffer();
	line_t *line = buf->current_line;
	if (go_next) {
		if (L_LINK_NEXT(line)) {
			next_line_command(NULL);
			buf->current_line = L_LINK_NEXT(line);
		}
	} else {
		if (L_LINK_PREV(line)) {
			prev_line_command(NULL);
			buf->current_line = L_LINK_PREV(line);
		}
	}
	L_LINK_REMOVE(line);
	line_free(line);
	buf->line_count--;
	buf->render = true;
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

public void line_delete_range(line_t *line, uint16_t start, uint16_t end)
{
	if (end > line->len || end < 0 || start >line->len || start < 0 )
		return;
	int remove_len = end - start;
	for (int i = start; i < end; i++) {
		line->str[i] = line->str[i + remove_len];
	}
	line->len -= remove_len;
	line->str = (char *) realloc(line->str, (line->len + 1) * sizeof(char));
	line->str[line->len] = '\0';
}

public void line_free(line_t *line)
{
	if (line) {
		if (line->str)
			free(line->str);
		free(line);
	}
}
