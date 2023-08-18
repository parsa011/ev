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

public uint8_t string_len_to_offset(char *str, int len)
{
	int tot = 0;
	for (int i = 0; i < len; i++) {
		if (*(str + i) == '\t')
			tot += TAB_SIZE;
		else
			tot++;
	}
	return tot;
}
