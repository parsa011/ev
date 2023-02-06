#ifndef _TTY_H
# define _TTY_H

#include "basic.h"
#include <termios.h>

public void tty_raw_mode();

public void tty_cooked_mode();

public char tty_get_char();

#endif
