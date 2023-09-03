#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "editor.h"

public char *make_fmt_string(const char *fmt, ...)
{
	int n = 0;
	size_t size = 0;
	char *p = NULL;
	va_list ap;

	/* Determine required size */

	va_start(ap, fmt);
	n = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	if (n < 0)
		return NULL;

	/* One extra byte for '\0' */

	size = (size_t) n + 1;
	p = malloc(size);
	if (p == NULL)
		return NULL;

	va_start(ap, fmt);
	n = vsnprintf(p, size, fmt, ap);
	va_end(ap);

	if (n < 0) {
		free(p);
		return NULL;
	}

	return p;
}

public uint8_t offset_to_col(char *str, int str_len, int len)
{
	int col = 1;
	if (str_len == 0)
		return col;
	for (int i = 0; i < len; i++) {
		if (*(str + i) == '\t')
			col += TAB_SIZE;
		else
			col++;
	}
	return col;
}

public uint8_t col_to_offset(char *str, int str_len, uint8_t col)
{
	uint8_t current_col = 1;
	int offset = 0;
	if (str_len == 0)
		return offset;
	while (current_col < col) {
		if (*(str + offset) == '\t') {
			current_col += TAB_SIZE;
		} else
			current_col++;
		offset++;
	}
	return offset;
}
