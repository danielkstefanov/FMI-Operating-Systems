#include <err.h>
#include <unistd.h>
#include <fcntl.h>

char named_pipe[] = "../foo/named_pipe_for_2020_SE_01";

int wrapped_dup2(int old, int new) {
    int result = dup2(old, new);

    if (result == -1) {
        err(3, "Cannot duplicate filedescriptor");
    }

    return result;
}

int wrapped_open(const char *filename, int flags)
{
    int fd = open(filename, flags);

    if (fd < 0)
    {
        err(2, "Cannot open file %s!", filename);
    }

    return fd;
}

int main(int argc, char* argv[]) {
        if (argc != 2) {
        errx(1, "Expected one arg only!");
        }

        int fd = wrapped_open(named_pipe, O_RDONLY);
    wrapped_dup2(fd, 0);
    execl(argv[1], argv[1], NULL);
        return 0;
}
