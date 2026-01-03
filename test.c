#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>



/
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

