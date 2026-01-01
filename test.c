#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>

static int log_master_fd = -1;

/* ---------- logger ---------- */

void log_msg2(const char *fmt, ...)
{
    if (log_master_fd < 0)
        return;

    char buf[1024];

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    write(log_master_fd, buf, strlen(buf));
    write(log_master_fd, "\r\n", 2);
}

/* ---------- init logger ---------- */

void start_logger(void)
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

	pid_t pid = fork();
	if (pid == 0) {
		execlp(
				"xterm",
				"xterm",
				"-T", "Editor Logger",
				"-e", "cat", slave_name,
				NULL
			  );
		_exit(1);
	}
}

/* ---------- fake editor ---------- */

int main(void)
{
    start_logger();

    log_msg("Logger started");
    log_msg("Editor PID: %d", getpid());

    for (int i = 1; i <= 5; i++) {
        log_msg("Event %d occurred", i);
        sleep(1);
    }

    log_msg("Done. Press Ctrl+C to exit.");

    while (1)
        pause();
}

