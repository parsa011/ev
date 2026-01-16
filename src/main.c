#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "editor.h"
#include "tty.h"
#include "key.h"
#include "util.h"
#include "log.h"

void die(int code)
{
	exit(code);
}

void handle_signals()
{
	signal(SIGWINCH, editor_change_size);
	signal(SIGSEGV, editor_close);
}

int main(int argc, char **argv)
{
	handle_signals();
	editor_init();
	if (argv[1])
		buffer_file_open(argv[1]);
	else
		buffer_append_line(line_init("", 0));
	editor_run();
	editor_close();
	return 0;
}
