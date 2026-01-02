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
	/*
	 * this is for test, if we passed any arg to program, it will counted as file name for now
	 * we will open it in current buffer of editor
	 * also we always use this in main()
	 * because it will load file lines into buffer , if file does not exists, it will
	 * create new line for current and first line of buffer
	 */
	if (argv[1])
		buffer_file_open(argv[1]);
	else
		buffer_append_line(line_init("", 0));
	editor_run();
	editor_close();
	return 0;
}
