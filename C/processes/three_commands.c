#include <err.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        err(1, "Invalid args count: Expected three");
    }

    pid_t parent = getpid();

    for (int i = 1; i <= 3; i++)
    {
        if (getpid() == parent)
        {
            pid_t child = fork();

            if (child == -1)
            {
                err(2, "Cannot fork");
            }
            else if (child == 0)
            {
                if (execlp(argv[i], argv[i], NULL) < 0)
                {
                    err(3, "Cannot exec %s!", argv[i]);
                }

                err(127, "Command not exist");
            }
        }
    }

    if (getpid() == parent)
    {
        pid_t processes[3];
        int statuses[3];

        for (int i = 0; i < 3; i++)
        {
            processes[i] = wait(&statuses[i]);
        }

        for (int i = 0; i < 3; i++)
        {
            printf("process: %d with status: %d \n", processes[i], statuses[i]);
        }
    }

    return 0;
}
