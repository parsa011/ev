#include "tty.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

struct termios old_termios, new_termios;

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

public char tty_get_char()
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

	return c;
}
