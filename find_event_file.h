#ifndef DEMON_KEYLOGGER_FIND_EVENT_FILE_H
#define DEMON_KEYLOGGER_FIND_EVENT_FILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

/**
 * Tries to find the filepath of a connected keyboard
 *
 * @return the filepath of keyboard, or NULL if none could be detected
 */
char *get_keyboard_event_file(void);

#endif //DEMON_KEYLOGGER_FIND_EVENT_FILE_H
