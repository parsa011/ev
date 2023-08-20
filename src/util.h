#ifndef _UTIL_H
# define _UTIL_H

#include  <stdint.h>
#include "basic.h"

/*
 *	make string with given format and returns it pointer
 *	also this code is from printf manual page :)
 */
public char *make_fmt_string(const char *fmt, ...);

public uint8_t offset_to_col(char *str, int len);

public uint8_t col_to_offset(char *str, uint8_t col);

#endif
