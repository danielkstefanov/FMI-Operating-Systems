#include <unistd.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>

char INTERVAL_SYMBOL = 0x20;
char NEW_LINE_SYMBOL = 0x0A;

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

int main(int argc, char* argv[]) {
        if (argc > 2) {
        err(1, "Invalid args count! Expected two!");
        }

        char command[5] = "echo";

    if (argc == 2) {
            if (strlen(argv[1]) > 4) {
            err(2, "Invalid command");
            }

        strcpy(command, (const char*) argv[1]);
    }

    char first_parameter[5];
    char second_parameter[5];

    int index = 0;
    char* to_read = first_parameter;
    char current_symbol;

    while (wrapped_read(0, &current_symbol, sizeof(char))) {

        if (index == 5) {
            err(3, "Parameter must be not more than 4 symbols!");
        }

        to_read[index] = current_symbol;

        if (current_symbol == INTERVAL_SYMBOL || current_symbol == NEW_LINE_SYMBOL)
        {
            to_read[index] = '\0';

            if (to_read == second_parameter) {
                to_read = first_parameter;

                pid_t pid = fork();

                if (pid == 0) {
                    execlp(command, command, first_parameter, second_parameter, NULL);
                    err(6, "Invalid execlp!");
                }

                int status;
                wrapped_wait(&status);

                if (!WIFEXITED(status)) {
                    err(5, "Cannot exec!");
                }

                if (WEXITSTATUS(status) != 0) {
                    err(4, "Exited with invalid status!");
                }
            }
            else {
                to_read = second_parameter;
            }

            index = 0;
            continue;
        }

        index++;
    }

    if (to_read == second_parameter) {

            pid_t pid = fork();

            if (pid == 0) {
                execlp(command, command, first_parameter, NULL);
            }

            int status;
            wrapped_wait(&status);

            if (!WIFEXITED(status)) {
                err(5, "Cannot exec!");
            }

            if (WEXITSTATUS(status) != 0) {
                err(4, "Exited with invalid status!");
            }
    }


    return 0;
}
