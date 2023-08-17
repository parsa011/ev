#ifndef _LOG_H
# define _LOG_H

#include "tty.h"
#include "file.h"

#define _LOG_FORMAT(format)  \
	"[%s:%u] %s(): " format "\r\n", file_name(__FILE__), __LINE__, __FUNCTION__

#define log(format, ...)     \
	tty_put_string(true, _LOG_FORMAT(format), ##__VA_ARGS__)

#endif
