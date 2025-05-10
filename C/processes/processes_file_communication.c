#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int wrapped_open(const char *filename, int flags, mode_t *modes)
{
    int fd = modes == NULL ? open(filename, flags) : open(filename, flags, *modes);

    if (fd == -1)
    {
        err(2, "Cannot open file %s!", filename);
    }

    return fd;
}

int wrapped_read(int fd, void *data, size_t bytes)
{
    int read_size = read(fd, data, bytes);

    if (read_size < 0)
    {
        err(3, "Cannot read from file descriptor %d!", fd);
    }

    return read_size;
}

int wrapped_write(int fd, const void *data, size_t bytes)
{
    int written_bytes_count = write(fd, data, bytes);

    if (written_bytes_count < 0)
    {
        err(1, "Cannot write in file descriptor %d!", fd);
    }

    return written_bytes_count;
}

char space_symbol = ' ';

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        errx(1, "Invalid arguments count: Expected one");
    }

    pid_t child = fork();

    if (child == -1)
    {
        err(2, "Cannot fork");
    }
    else if (child == 0)
    {
        mode_t modes = S_IRUSR | S_IWUSR;
        int fd = wrapped_open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, &modes);

        const char *text = "foobar";
        wrapped_write(fd, text, strlen(text));
    }
    else
    {
        int status;
        wait(&status);

        int fd = wrapped_open(argv[1], O_RDONLY, NULL);
        char symbol;
        while (wrapped_read(fd, &symbol, sizeof(symbol)) > 0)
        {
            wrapped_write(1, &symbol, sizeof(symbol));
            wrapped_write(1, &space_symbol, sizeof(space_symbol));
        }
    }

    return 0;
}
