#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include "tty.h"
#include "editor.h"
#include "util.h"
#include "log.h"

#ifdef _WIN32

#include <io.h>
#include <windows.h>
static DWORD orig_in_mode;
static DWORD orig_out_mode;

#else

#include <sys/ioctl.h>
#include <termios.h>

#include <unistd.h>

#endif

private struct termios old_termios, new_termios;

/* output buffer, index and size */
private unsigned char obuf[OBUFSIZE];
private int obufp = 0;

public void tty_raw_mode()
{
	tcgetattr(0, &old_termios);

	new_termios = old_termios;

	/* raw CR/NL etc input handling, but keep ISTRIP if we're on a 7-bit line */
	new_termios.c_iflag &= ~(IGNBRK | BRKINT | IGNPAR | PARMRK
						  | INPCK | INLCR | IGNCR | ICRNL);

	/* raw CR/NR etc output handling */
	new_termios.c_oflag &=
	    ~(OPOST | ONLCR | OLCUC | OCRNL | ONOCR | ONLRET);


	/* No signal handling, no echo etc */
	new_termios.c_lflag &= ~(ISIG | ICANON | XCASE | ECHO | ECHOE | ECHOK
						  | ECHONL | NOFLSH | TOSTOP | ECHOCTL |
						  ECHOPRT | ECHOKE | FLUSHO | PENDIN | IEXTEN);

	/* one character, no timeout */
	new_termios.c_cc[VMIN] = 1;
	new_termios.c_cc[VTIME] = 0;

	tcsetattr(0, TCSADRAIN, &new_termios);
}

public void tty_cooked_mode()
{
	tcsetattr(0, TCSADRAIN, &old_termios);
}

public char tty_get_char(int *remaining)
{
	char c;
	static char buf[32];
	static int pending = 0;
	static int index = 0;
	int count = 0;

	if (pending) {
		pending--;
		c = buf[++index];
	} else {
		count = read(STDIN_FILENO, buf, sizeof(buf));
		c = buf[index = 0];
		pending = count - 1;
	}
	*remaining = pending;
	return c;
}

#ifdef _WIN32
public int tty_window_size(int* rows, int* cols)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hStdout, &csbi)) {
		*cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		*rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		return 0;
	}
	return -1;
}
#else

public int tty_cursor_pos_get(int* rows, int* cols)
{
	char buf[32];
	size_t i = 0;

	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
		return -1;

	while (i < sizeof(buf) - 1) {
		if (read(STDIN_FILENO, &buf[i], 1) != 1)
			break;
		if (buf[i] == 'R')
			break;
		i++;
	}
	buf[i] = '\0';

	if (buf[0] != '\x1b' || buf[1] != '[') {
		return -1;
	}
	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) {
		return -1;
	}
	return 0;
}

public int tty_window_size(int* rows, int* cols)
{
	struct winsize ws;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
			return -1;
		}
		return tty_cursor_pos_get(rows, cols);
	} else {
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
}
#endif

/*
 * write string to output buffer
 */
public void tty_put_string(bool flush_now, char *format, ...)
{
	int n = 0;
	size_t size = 0;
	char *str = NULL;
	va_list ap;

	/* Determine required size */

	va_start(ap, format);
	n = vsnprintf(NULL, 0, format, ap);
	va_end(ap);

	if (n < 0) {
		return;
	}

	/* One extra byte for '\0' */

	size = (size_t) n + 1;
	str = malloc(size);
	if (str == NULL) {
		return; 
	}

	va_start(ap, format);
	n = vsnprintf(str, size, format, ap);
	va_end(ap);

	char *ptr = str;
	while (*ptr) {
		tty_put_char(*ptr++);
	}
	if (flush_now) {
		tty_flush();
	}
	if (str) {
		free(str);
	}
}

public void tty_put_char(char c)
{
	if (obufp == OBUFSIZE) {
		tty_flush();
	}
	obuf[obufp++] = c;
}

public void tty_flush()
{
	if (obufp) {
		if (write(editor.tty_in, obuf, obufp) == 0) {
			printf("SOMETHING WENT WRONG WHEN WRITING INTO TERMOUT\n");
			die(0);
		}
		obufp = 0;
	}
}

public void tty_clear()
{
	system("clear");
}

public void tty_clear_eol()
{
	tty_put_string(true, "%s", "\e[K");
}

public void tty_cursor_move(cursor_pos_t pos)
{
	tty_put_string(true, "\e[%d;%dH", pos.row, pos.col);
}

public void tty_cursor_hide()
{
	tty_put_string(true, "%s", "\e[?25l");
}

public void tty_cursor_show()
{
	tty_put_string(true, "%s", "\e[?25h");
}

public void tty_cursor_store()
{
	tty_put_string(true, "%s", "\e7");
}

public void tty_cursor_restore()
{
	tty_put_string(true, "%s", "\e8");
}

public void tty_cursor_line_next()
{
	tty_put_string(true, "%s", "\e[B");
}

public void tty_cursor_line_prev()
{
	tty_put_string(true, "%s", "\e[A");
}

public void tty_cursor_char_next()
{
	tty_put_string(true, "%s", "\e[C");
}

public void tty_cursor_char_prev()
{
	tty_put_string(true, "%s", "\e[D");
}
