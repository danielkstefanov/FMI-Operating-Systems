#include <err.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        errx(1, "Expected only one arg!");
    }

    if (execlp("ls", "ls", argv[1], NULL) < 0)
    {
        err(2, "Cannot list");
    }

    return 0;
}