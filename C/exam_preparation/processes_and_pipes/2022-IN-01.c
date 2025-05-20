#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int wrapped_read(int fd, void *buffer, int size)
{
    int read_size = read(fd, buffer, size);

    if (read_size < 0)
    {
        err(5, "Cannot read from file %d", fd);
    }

    return read_size;
}

int wrapped_write(int fd, const void *buffer, int size)
{
    int write_size = write(fd, buffer, size);

    if (write_size < 0)
    {
        err(6, "Cannot write in fd: %d!", fd);
    }

    return write_size;
}

char ready_parent_message[] = "ready parent\0";
char ready_child_message[] = "ready child\0";

int main(int argc, char* argv[]) {

    if (argc != 3) {
        err(1, "Invalid args count: Expected two");
    }

        int N = atoi(argv[1]);
        int D = atoi(argv[2]);

    int pc[2];
    int cp[2];
    pipe(pc);
    pipe(cp);

    pid_t pid = fork();

        for (int i=0; i< N; i++) {
        if(pid == 0) {
            close(pc[1]);
            close(cp[0]);

            char received[13];
            received[12] = '\0';
            wrapped_read(pc[0], &received, 13);

            if (strcmp(received, ready_parent_message) != 0) {
                err(2, "Invalid message");
            }

            wrapped_write(0, "DONG\n", 5);
            wrapped_write(cp[1], &ready_child_message, 12);
        }
        else {
            close(pc[0]);
            close(cp[1]);

            wrapped_write(0, "DING\n", 5);
            wrapped_write(pc[1], &ready_parent_message, 13);

            char received[12];
            received[11] = '\0';
            wrapped_read(cp[0], &received, 12);

            if (strcmp(received, ready_child_message) != 0) {
                err(2, "Invalid message");
            }

            sleep(D);
        }
    }
        return 0;
}
