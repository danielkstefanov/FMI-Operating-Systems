#include <stdbool.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <string.h>

pid_t wrapped_wait(int* status){
    pid_t child_pid = wait(status);

    if (child_pid == -1) {
        err(3, "Error in wait!");
    }
    return child_pid;
}

int wrapped_write(int fd, const void *data, size_t size)
{
    int write_size = write(fd, data, size);

    if (write_size < 0)
    {
        err(4, "Cannot write in file descriptor %d!", fd);
    }

    return write_size;
}

int wrapped_read(int fd, void* data, size_t size)
{
    int read_size = read(fd, data, size);

    if(read_size == -1) {
        err(1, "Cannot read from file descriptor %d!", fd);
    }

    return read_size;
}

char EXIT_COMMAND[] = "exit";

int main(void) {

    while (true) {

        wrapped_write(1, "command > ", 10);
        char command[128];
        int read_size = wrapped_read(0, command, sizeof(command) - 1);
        command[read_size] = '\0';

        if (command[read_size - 1] == '\n') {
            command[read_size - 1] = '\0';
        }

        pid_t pid = fork();

        if (pid == 0) {
            if (strcmp(command, EXIT_COMMAND) == 0) {
               return 1;
            }

            execlp(command, command, NULL);
            err(2, "Invalid command!");
        }

        int status;
        wrapped_wait(&status);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
            return 0;
        }
    }

        return 0;
}
