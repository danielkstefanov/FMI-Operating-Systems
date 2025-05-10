#include <unistd.h>
#include <err.h>

int main(void)
{
    if (execlp("/bin/date", "date", (char *)NULL) < 0)
    {
        err(1, "Cannot invoke date");
    }

    return 0;
}