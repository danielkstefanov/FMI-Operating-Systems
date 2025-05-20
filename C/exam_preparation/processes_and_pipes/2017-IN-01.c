#include <unistd.h>
#include <err.h>
#include <sys/wait.h>

int wrapped_wait(int* status) {
    int child_pid = wait(status);

    if (child_pid == -1) {
        err(4, "Cannot wait!");
    }

    return child_pid;
}

int wrapped_pipe(int fd[2])
{
    int res = pipe(fd);

    if (res == -1) {
        err(2, "Invalid pipe!");
    }

    return res;
}

int wrapped_dup2(int old, int new)
{
    int fd = dup2(old, new);

    if (fd == -1) {
        err(3, "Cannot dup!");
    }

    return fd;
}

int main(void) {

    int fd1[2];
    pipe(fd1);
    pid_t pid = fork();

    if (pid == 0) {
        close(fd1[0]);

        wrapped_dup2(fd1[1],1);

        execlp("cut", "cut", "-d", ":", "-f", "7","/etc/passwd", NULL);
        err(1, "Invalid command");
    }
    
    close(fd1[1]);

    int status;
    wrapped_wait(&status);

    int fd2[2];
    pipe(fd2);
    pid = fork();

    if (pid == 0) {
        close(fd2[0]);
        wrapped_dup2(fd1[0], 0);
        wrapped_dup2(fd2[1], 1);
        execlp("sort", "sort", NULL);
        err(5, "Invalid command!");
    }
    
    close(fd2[1]);

    wrapped_wait(&status);

    int fd3[2];
    pipe(fd3);
    pid = fork();

    if (pid == 0) {
        close(fd3[0]);
        wrapped_dup2(fd2[0], 0);
        wrapped_dup2(fd3[1], 1);
        execlp("uniq", "uniq", "-c", NULL);
        err(5, "Invalid command!");
    }
    
    close(fd3[1]);

    wrapped_wait(&status);

    wrapped_dup2(fd3[0],0);
    execlp("sort", "sort", "-n", NULL);
    err(5, "Invalid command!");
        return 0;
}
