#ifndef _TTY_H
# define _TTY_H

#include <termios.h>
#include "basic.h"

/* 
 * 	write a character to the output buffer.  If it becomes
 * 	full (obufp == OBUFSIZE), call tty_flush()
 */
#define OBUFSIZE 512

public void tty_raw_mode();

public void tty_cooked_mode();

public char tty_get_char(int *pending);

public int tty_window_size(int *rows, int *cols);

public int tty_cursor_pos_get(int *rows, int *cols);

/*
 *	Write given string into out put buffer using ttyputc()
 *	sometimes we need to write output immediatly , so we will pass
 *	'true' for flush_now to autocall for tty_flush()
 */
public void tty_put_string(bool flush_now, char *s, ...);

/*
 *	add given given into out put array
 *	call tty_flush() when we reached to OBUFSIZE size
 */
public void tty_put_char(char c);

/*
 *	Print output to terminal by calling write()
 */
public void tty_flush();

public void tty_clear();

public void tty_cursor_hide();

public void tty_cursor_show();

public void tty_cursor_store();

public void tty_cursor_restore();

public void tty_cursor_line_next();

public void tty_cursor_line_prev();

public void tty_cursor_char_next();

public void tty_cursor_char_prev();

#endif
