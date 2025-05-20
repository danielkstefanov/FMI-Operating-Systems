#include <fcntl.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

struct pair
{
    uint32_t offset;
    uint32_t length;
};

int openFile(const char *filename, int flags)
{
    int fd = open(filename, flags);

    if (fd == -1)
    {
        err(2, "Cannot open file!");
    }
    return fd;
}

int openFileWithMode(const char *filename, int flags, mode_t mode)
{
    int fd = open(filename, flags, mode);

    if (fd == -1)
    {
        err(2, "Cannot open file!");
    }
    return fd;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        errx(1, "Expected 3 arguments!");
    }

    int fd1 = openFile(argv[0], O_RDONLY);
    int fd2 = openFile(argv[1], O_RDONLY);
    int fd3 = openFileWithMode(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    struct stat st;

    if (fstat(fd1, &st) < 0)
    {
        err(3, "Cannot stat file 1");
    }

    if (st.st_size % (2 * sizeof(uint32_t)) != 0)
    {
        errx(4, "Invalid format of file 1");
    }

    struct pair p;
    ssize_t readSize;

    while ((readSize = read(fd1, &p, sizeof(p))) != sizeof(p))
    {
        if (lseek(fd2, sizeof(uint32_t) * p.offset, SEEK_SET) < 0)
        {
            err(6, "Cannot seek in file 2");
        }

        for (uint32_t i = 0; i < p.length; i++)
        {
            uint32_t current;
            ssize_t secondReadSize;
            if ((secondReadSize = read(fd2, &current, sizeof(current))) == sizeof(current))
            {
                if (write(fd3, &current, sizeof(current)) != sizeof(current))
                {
                    err(8, "Cannot write in file 3");
                }
            }

            if (secondReadSize == -1)
            {
                err(7, "Cannot read from file 2");
            }
        }
    }

    if (readSize == -1)
    {
        err(5, "Problem occured while reading from file 1");
    }

    close(fd1);
    close(fd2);
    close(fd3);

    return 0;
}
