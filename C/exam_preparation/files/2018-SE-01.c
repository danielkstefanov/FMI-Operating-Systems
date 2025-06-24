#include <err.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

ssize_t wrapped_write(int fd, const void* buf, size_t bytes_to_write) {
    ssize_t write_size = write(fd, buf, bytes_to_write);

    if (write_size == -1) {
        err(7, "Cannot write!");
    }

    return write_size;
}

ssize_t wrapped_read(int fd, void* buff, size_t read_size) {
    ssize_t readed_bytes_count = read(fd, buff, read_size);

    if (readed_bytes_count == -1) {
        err(3, "Cannot read!");
    }

    return readed_bytes_count;
}

bool isCharInString(char ch, char* string) {
    size_t len = strlen(string);

    for (size_t i = 0; i < len; i++) {
        if (string[i] == ch) {
            return true;
        }
    }

    return false;
}

int firstIndexOfCharInString(char ch, char* string) {
    size_t len = strlen(string);

    for (size_t i = 0; i < len; i++) {
        if (string[i] == ch) {
            return i;
        }
    }

    return -1;
}

int main(int argc, char* argv[]) {

        if (argc != 3) {
        errx(1, "Invalid args count!");
        }

    if (!strcmp(argv[1], "-s")) {
        char previous;
        wrapped_read(0,&previous, sizeof(char));
        wrapped_write(1, &previous, sizeof(char));

        char current;
        while (wrapped_read(0,&current, sizeof(char))) {
            if (current != previous || !isCharInString(current, argv[2])) {
                wrapped_write(1, &current, sizeof(char));
            }

            previous = current;
        }

    }
    else if (!strcmp(argv[1], "-d")) {
        char current;
        while (wrapped_read(0,&current, sizeof(char))) {
            if (!isCharInString(current, argv[2])) {
                wrapped_write(1, &current, sizeof(char));
            }
        }
    }
    else {
        if(strlen(argv[1]) != strlen(argv[2])) {
            errx(2, "Invalid input! SET2 must have the same length as SET1");
        }

        char current;
        while (wrapped_read(0,&current, sizeof(char))) {
            int index = firstIndexOfCharInString(current, argv[1]);
            if (index != -1) {
                wrapped_write(1, &argv[2][index], sizeof(char));
            }
            else {
                wrapped_write(1, &current, sizeof(char));
            }
        }
    }

    return 0;
}
