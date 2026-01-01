#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

#include "log.h"

private int log_master_fd = -1;
private pid_t pid = -1;

void logger_clear();

public void start_logger()
{
	log_master_fd = posix_openpt(O_RDWR | O_NOCTTY);
	if (log_master_fd < 0) {
		perror("posix_openpt");
		exit(1);
	}

	grantpt(log_master_fd);
	unlockpt(log_master_fd);

	char *slave_name = ptsname(log_master_fd);
	if (!slave_name) {
		perror("ptsname");
		exit(1);
	}

	pid = fork();
	if (pid == 0) {
		execlp("xterm", "xterm", "-T", "Editor Logger", "-e", "cat", slave_name, NULL);
		_exit(1);
	}

}

public void log_msg(const char *fmt, ...)
{
	logger_clear();
	char buf[1024];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	write(log_master_fd, buf, strlen(buf));
	write(log_master_fd, "\r\n", 2);
}


public void close_logger()
{
	if (pid > 0) {
		kill(pid, SIGTERM);
		pid = -1;
	}
}

void logger_clear()
{
	const char *clear = "\033[2J\033[H";
	write(log_master_fd, clear, strlen(clear));
}
