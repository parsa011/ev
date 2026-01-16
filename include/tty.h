#ifndef _TTY_H
# define _TTY_H

#include <termios.h>
#include "basic.h"

/* 
 * 	write a character to the output buffer.  If it becomes
 * 	full (obufp == OBUFSIZE), call tty_flush()
 */
#define OBUFSIZE 512

void tty_raw_mode();

void tty_cooked_mode();

char tty_get_char(int *pending);

int tty_window_size(int *rows, int *cols);

int tty_cursor_pos_get(int *rows, int *cols);

/*
 *	Write given string into out put buffer using ttyputc()
 *	sometimes we need to write output immediatly , so we will pass
 *	'true' for flush_now to autocall for tty_flush()
 */
void tty_put_string(bool flush_now, char *s, ...);

/*
 *	add given given into out put array
 *	call tty_flush() when we reached to OBUFSIZE size
 */
void tty_put_char(char c);

/*
 *	Print output to terminal by calling write()
 */
void tty_flush();

void tty_clear();

void tty_clear_eol();

void tty_cursor_move(cursor_pos_t pos);

void tty_cursor_hide();

void tty_cursor_show();

void tty_cursor_store();

void tty_cursor_restore();

void tty_cursor_line_next();

void tty_cursor_line_prev();

void tty_cursor_char_next();

void tty_cursor_char_prev();

#endif
