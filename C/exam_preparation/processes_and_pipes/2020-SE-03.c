#include <unistd.h>
#include <sys/wait.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>

int wrapped_open(const char* filename, int flags, mode_t* modes) {
    int result = modes == NULL ? open(filename, flags) : open(filename, flags, &modes);

    if (result == -1) {
        err(2, "Invalid open!");
    }

    return result;
}

int wrapped_pipe(int fildes[2]) {
    int result = pipe(fildes);

    if (result == -1) {
        err(4, "Cannot pipe!");
    }

    return result;
}

pid_t wrapped_fork(void) {
    pid_t pid = fork();

    if(pid == -1) {
        err(5, "Cannot fork");
    }

    return pid;
}

ssize_t wrapped_read(int fd, void* buff, size_t read_size) {
    ssize_t readed_bytes_count = read(fd, buff, read_size);

    if (readed_bytes_count == -1) {
        err(3, "Cannot read!");
    }

    return readed_bytes_count;
}

ssize_t wrapped_write(int fd, const void* buf, size_t bytes_to_write) {
    ssize_t write_size = write(fd, buf, bytes_to_write);

    if (write_size == -1) {
        err(7, "Cannot write!");
    }

    return write_size;
}

off_t wrapped_lseek(int fd, off_t offset, int whence) {
    off_t result = lseek(fd, offset, whence);

    if (result == -1) {
        err(8, "Cannot lseek!");
    }

    return result;
}

typedef struct {
    char filename[8];
    uint32_t offset;
    uint32_t length;
} triplet;

int main(int argc, char* argv[]) {

        if (argc != 2) {
        err(1, "Invalid args count!");
        }

    int input_file = wrapped_open(argv[1], O_RDONLY, NULL);

    triplet info;
    int8_t children_count = 0;

    int pd[8][2];

    while (wrapped_read(input_file, &info, sizeof(info))) {
        wrapped_pipe(pd[children_count]);

        pid_t pid = wrapped_fork();

        if (pid == 0) {
            close(pd[children_count][0]);

            int fd = wrapped_open(info.filename, O_RDONLY, NULL);
            wrapped_lseek(fd, info.offset * sizeof(uint16_t), SEEK_SET);

            uint16_t result = 0;

            for (uint32_t i = 0; i < info.length; i++){
                uint16_t temp;
                wrapped_read(fd, &temp, sizeof(temp));
                result ^= temp;
            }

            wrapped_write(pd[children_count][1], &result, sizeof(result));
            return children_count;
        }
        else {
            close(pd[children_count][1]);
        }

        children_count++;
    }

    uint16_t result = 0;

    for (int i = 0; i < children_count; i++) {
        int status;
        wait(&status);

        if (!WIFEXITED(status)) {
            err(6, "Child did not exit!");
        }

        int current_pipe_index = WEXITSTATUS(status);

        int16_t current_child_result;
        wrapped_read(pd[current_pipe_index][0], &current_child_result, sizeof(current_child_result));

        result ^= current_child_result;
    }

    char output[1024];
    int len = snprintf(output, sizeof(output), "result: %04x\n", result);
    write(STDOUT_FILENO, output, len);

    return 0;
}
