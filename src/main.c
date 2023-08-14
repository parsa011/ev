#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include "editor.h"
#include "tty.h"
#include "util.h"
#include "log.h"

public void die(int code)
{
	exit(code);
}

void handle_signals()
{
	signal(SIGWINCH, editor_change_size);
}

int main(int argc, char **argv)
{
	editor_init();
	log("this is a simple log :))\r\n");
	if (argc > 1)
		editor_file_open(argv[1]);
	editor_run();
	editor_close();
	return 0;
}
