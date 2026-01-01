#ifndef _LOG_H
# define _LOG_H

#include "basic.h"
#include "tty.h"
#include "file.h"

void start_logger();
void log_msg(const char *fmt, ...);
void close_logger();

#endif
