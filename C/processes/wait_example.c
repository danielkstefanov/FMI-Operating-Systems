#include <sys/wait.h>
#include <unistd.h>
#include <err.h>

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        err(1, "Cannot fork");
    }

    if (pid == 0)
    {
        for (int i = 0; i < 100; i++)
        {
            write(1, "Child\n", 7);
        }
    }
    else
    {
        int status;
        wait(&status);
        for (int i = 0; i < 100; i++)
        {
            write(1, "Father\n", 8);
        }
    }

    return 0;
}
