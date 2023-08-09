#ifndef _TTY_H
# define _TTY_H

#include "basic.h"
#include <termios.h>

public void tty_raw_mode();

public void tty_cooked_mode();

public char tty_get_char(int *pending);

public int tty_window_size(int *rows, int *cols);

public int tty_cursor_pos_get(int *rows, int *cols);

public void tty_clear();

public void tty_cursor_hide();

public void tty_cursor_show();

public void tty_cursor_store();

public void tty_cursor_restore();

public void tty_cursor_line_next();

public void tty_cursor_line_prev();

public void tty_cursor_char_next();

public void tty_cursor_prev_char();

#endif
