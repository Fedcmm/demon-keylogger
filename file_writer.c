#include "file_writer.h"

#define NUM_CHARS 64

int keepWriting = 1;

void writer_handler(int sig) {
    keepWriting = 0;
    syslog(LOG_INFO, "Writer stopped by signal %s", strsignal(sig));
}

void file_writer(int read_pipe, int file_desc) {
    ssize_t bytesToWrite;
    char str[NUM_CHARS];
    int numWritten = 10;

    struct sigaction int_handler;
    int_handler.sa_handler = &writer_handler;
    sigaction(SIGINT, &int_handler, NULL);

    while (keepWriting) {
        bytesToWrite = read(read_pipe, str, sizeof(char) * NUM_CHARS);
        if (bytesToWrite == 0) {
            syslog(LOG_INFO, "Read EOF from pipe");
            break;
        }

        if (write(file_desc, str, bytesToWrite) == -1) {
            syslog(LOG_CRIT, "write: %m");
            break;
        }

        if (--numWritten == 0) {
            write(file_desc, "\n", sizeof("\n"));
            numWritten = 10;
        }
    }

    write(file_desc, "\n\n", sizeof("\n\n"));
    syslog(LOG_INFO, "Writer finished, closing read end of pipe");
    close(read_pipe);
}