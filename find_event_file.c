#include "find_event_file.h"

#define INPUT_DIR "/dev/input/"

int is_char_device(const struct dirent *file) {
    struct stat filestat;
    char file_path[512];
    int err;

    snprintf(file_path, sizeof(file_path), "%s%s", INPUT_DIR, file->d_name);

    err = stat(file_path, &filestat);
    if (err)
        return 0;

    // Check if the file supports character events
    return S_ISCHR(filestat.st_mode);
}


char *get_keyboard_event_file(void) {
    char *keyboard_file = NULL;
    int num, i;
    struct dirent **event_files;
    char file_path[512];

    num = scandir(INPUT_DIR, &event_files, &is_char_device, &alphasort);
    if (num < 0)
        return NULL;

    for (i = 0; i < num; ++i) {
        int fd;
        int32_t event_bitmap = 0;
        int32_t kbd_bitmap = KEY_A | KEY_B | KEY_C | KEY_Z;

        snprintf(file_path, sizeof(file_path), "%s%s", INPUT_DIR, event_files[i]->d_name);

        if ((fd = open(file_path, O_RDONLY)) == -1) {
            continue;
        }

        // Get all the features supported by the device
        ioctl(fd, EVIOCGBIT(0, sizeof(event_bitmap)), &event_bitmap);
        if ((EV_KEY & event_bitmap) == EV_KEY) {
            // The device supports key events

            // Get the features supported that are related to key events
            ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(event_bitmap)), &event_bitmap);
            if ((kbd_bitmap & event_bitmap) == kbd_bitmap) {
                // The device supports A, B, C, Z keys, so it probably is a keyboard

                keyboard_file = strdup(file_path);
                close(fd);
                break;
            }
        }

        close(fd);
    }

    // Cleanup scandir results
    for (i = 0; i < num; ++i) {
        free(event_files[i]);
    }
    free(event_files);

    return keyboard_file;
}

