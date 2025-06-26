#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>

int wrapped_open(const char *filename, int flags, mode_t *modes)
{
    int result = modes == NULL ? open(filename, flags) : open(filename, flags, &modes);

    if (result == -1)
    {
        err(2, "Invalid open!");
    }

    return result;
}

ssize_t wrapped_read(int fd, void *buff, size_t read_size)
{
    ssize_t readed_bytes_count = read(fd, buff, read_size);

    if (readed_bytes_count == -1)
    {
        err(3, "Cannot read!");
    }

    return readed_bytes_count;
}

ssize_t wrapped_write(int fd, const void *buf, size_t bytes_to_write)
{
    ssize_t write_size = write(fd, buf, bytes_to_write);

    if (write_size == -1)
    {
        err(7, "Cannot write!");
    }

    return write_size;
}

int get_file_size(int fd)
{
    struct stat sb;

    if (fstat(fd, &sb) < 0)
    {
        err(4, "Cannot stat file desc %d!", fd);
    }

    return sb.st_size;
}

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        errx(1, "Invalid args count!");
    }

    int signals = wrapped_open(argv[1], O_RDONLY, NULL);
    int data = wrapped_open(argv[2], O_RDONLY, NULL);
    mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR;
    int result = wrapped_open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, &mode);

    if (get_file_size(signals) * 8 != get_file_size(data) / 2)
    {
        errx(2, "Invalid files format!");
    }

    uint8_t current;
    uint16_t to_write;
    while (wrapped_read(signals, &current, sizeof(current)))
    {
        uint8_t mask = 128;

        while (mask >= 1)
        {
            wrapped_read(data, &to_write, sizeof(to_write));

            if (mask & current)
            {
                wrapped_write(result, &to_write, sizeof(to_write));
            }

            mask >>= 1;
        }
    }

    close(signals);
    close(data);
    close(result);

    return 0;
}
