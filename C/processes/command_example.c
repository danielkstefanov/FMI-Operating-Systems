#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        errx(1, "Invalid args count: Expected one");
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        err(2, "Cannot fork");
    }
    else if (pid == 0)
    {
        if (execlp(argv[1], argv[1], NULL) < 0)
        {
            err(3, "Cannot exec %s!", argv[1]);
        }

        err(127, "Command not exist");
    }
    else
    {
        int status;
        wait(&status);

        if (status == 0)
        {
            write(1, "Success\n", 9);
        }
    }

    return 0;
}
