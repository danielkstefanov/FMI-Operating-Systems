#include <fcntl.h>
#include <err.h>
#include <unistd.h>

char named_pipe[] = "named_pipe_for_2020_SE_01";

int wrapped_dup2(int old, int new) {
    int result = dup2(old, new);

    if (result == -1) {
        err(3, "Cannot duplicate filedescriptor");
    }

    return result;
}

int wrapped_open(const char *filename, int flags, mode_t *mode)
{
    int fd = (mode == NULL ? open(filename, flags) : open(filename, flags, *mode));

    if (fd < 0)
    {
        err(2, "Cannot open file %s!", filename);
    }

    return fd;
}

int main(int argc, char* argv[]) {

        if (argc != 2) {
        errx(1, "Invalid args count! Expected one!");
        }

    int fd = wrapped_open(named_pipe, O_WRONLY, NULL);
    wrapped_dup2(fd, 1);
        execlp("cat", "cat", argv[1], NULL);
        err(2, "Cannot exec");
        return 0;
}
