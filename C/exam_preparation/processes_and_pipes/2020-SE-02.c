#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

int wrapped_write(int fd, const void *buffer, int size)
{
    int write_size = write(fd, buffer, size);

    if (write_size < 0)
    {
        err(8, "Cannot write in fd: %d!", fd);
    }

    return write_size;
}

int wrapped_pipe(int filedes[2]) {
    int result = pipe(filedes);

    if (result == -1){
        err(2, "Cannot pipe!");
    }

    return result;
}

pid_t wrapped_fork(void) {
    pid_t pid = fork();

    if(pid == -1) {
        err(3, "Cannot fork!");
    }

    return pid;
}

int wrapped_dup2(int filedes, int filedes2) {
    int result = dup2(filedes,filedes2);

    if (result == -1) {
        err(4, "Cannot duplicate filedes!");
    }

    return result;
}

ssize_t wrapped_read(int filedes, void* obj, size_t read_size) {
    ssize_t result = read(filedes, obj, read_size);

    if (result == -1) {
        err(6, "Cannot read!");
    }

    return result;
}

int wrapped_open(const char* path, int flags, mode_t* mode) {
    int filedes = mode == NULL ? open(path, flags) : open(path, flags, *mode);

    if(filedes == -1) {
        err(7, "Cannot open!");
    }

    return filedes;
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        errx(1, "Invalid args count: Expected two");
        }

    int pd[2];
    wrapped_pipe(pd);
    pid_t pid = wrapped_fork();

    if (pid == 0) {
        close(pd[0]);
        wrapped_dup2(pd[1], 1);
        execlp("cat", "cat", argv[1], NULL);
        err(5, "Cannot execlp!");
    }
    else {
        close(pd[1]);
    }

    mode_t mode = 0666;
    int output_filedes = wrapped_open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, &mode);

    int8_t current_byte;
    bool is_escaped = false;

    while(wrapped_read(pd[0], &current_byte, sizeof(current_byte))) {
        if(current_byte == 0x7D) {
            is_escaped = true;
            continue;
        }

        if (current_byte == 0x55) {
            continue;
        }

        if (is_escaped) {
            current_byte = current_byte ^ 0x20;
        }

        wrapped_write(output_filedes, &current_byte, sizeof(current_byte));
        is_escaped = false;
    }

        return 0;
}
