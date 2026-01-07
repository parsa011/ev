#ifndef _UTIL_H
# define _UTIL_H

#include  <stdint.h>
#include "basic.h"

/*
 *	make string with given format and returns it pointer
 *	also this code is from printf manual page :)
 */
char *make_fmt_string(const char *fmt, ...);

uint8_t offset_to_col(char *str, int str_len, int len);
uint8_t col_to_offset(char *str, int str_len, uint8_t col);

int int_digits_count(int n);

#endif
