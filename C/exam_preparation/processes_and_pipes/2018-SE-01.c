#include <unistd.h>
#include <sys/wait.h>
#include <err.h>

pid_t wrapped_wait(int* status) {
    int result = wait(status);

    if(result == -1) {
        err(2, "Cannot wait!");
    }

    return result;
}

int wrapped_dup2(int old, int new) {
    int result = dup2(old, new);

    if (result == -1) {
        err(3, "Cannot duplicate filedescriptor");
    }

    return result;
}

pid_t wrapped_fork(void) {
    pid_t pid = fork();

    if(pid == -1) {
        err(4, "Cannot fork!");
    }

    return pid;
}

int main(int argc, char* argv[]) {

        if (argc != 2) {
            errx(1, "Invalid arguments count");
        }

        int pd1[2];
        pipe(pd1);

    pid_t first_pid = wrapped_fork();

    if(first_pid == 0) {
        close(pd1[0]);
        wrapped_dup2(pd1[1], 1);
        execlp("find", "find", argv[1], "-mindepth", "1", "-type", "f", "-printf", "%f %T@\n", NULL);
        err(2, "Invalid execlp");
    }
    else {
        close(pd1[1]);
    }

    int status;
    wrapped_wait(&status);

    int pd2[2];
    pipe(pd2);
    pid_t second_pid = wrapped_fork();

    if(second_pid == 0) {
        close(pd2[0]);
        wrapped_dup2(pd1[0], 0);
        wrapped_dup2(pd2[1], 1);
        execlp("sort", "sort", "-k2", "-rn", NULL);
        err(5, "Cannot execlp");
    }
    else {
        close(pd2[1]);
        close(pd1[0]);
    }

    wrapped_wait(&status);

    int pd3[2];
    pipe(pd3);

    pid_t third_pid = wrapped_fork();

    if (third_pid == 0) {
        close(pd3[0]);
        wrapped_dup2(pd3[1], 1);
        wrapped_dup2(pd2[0], 0);
        execlp("head", "head", "-n1", NULL);
        err(6, "Cannot execlp");
    }
    else {
        close(pd3[1]);
        close(pd2[0]);
    }

    wrapped_wait(&status);

    wrapped_dup2(pd3[0], 0);
    execlp("cut", "cut", "-d ", "-f", "1", NULL);
    err(7, "Cannot execlp");
}