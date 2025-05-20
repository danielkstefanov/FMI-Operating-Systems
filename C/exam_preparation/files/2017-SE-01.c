#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <fcntl.h>

int wrapped_open(const char *filename, int flags, mode_t *mode)
{
    int fd = mode == NULL ? open(filename, flags) : open(filename, flags, *mode);

    if (fd < 0)
    {
        err(2, "Cannot open file %s!", filename);
    }

    return fd;
}

int wrapped_read(int fd, void *data, size_t size)
{
    int read_size = read(fd, data, size);

    if (read_size < 0)
    {
        err(3, "Cannot read from file descriptor %d", fd);
    }

    return read_size;
}

int wrapped_write(int fd, const void *data, size_t size)
{
    int write_size = write(fd, data, size);

    if (write_size < 0)
    {
        err(4, "Cannot write in file descriptor %d!", fd);
    }

    return write_size;
}

typedef struct
{
    uint16_t offset;
    uint8_t original;
    uint8_t modified;
} patch_info;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        errx(1, "Invalid args count: Expected 3");
    }

    int original = wrapped_open(argv[1], O_RDONLY, NULL);
    int copy = wrapped_open(argv[2], O_RDONLY, NULL);
    // mode_t modes = S_IRUSR | S_IWUSR;
    mode_t modes = 0777;
    int difference = wrapped_open(argv[3], O_RDWR | O_CREAT | O_TRUNC, &modes);

    patch_info data;
    data.offset = 0;
    while (wrapped_read(original, &data.original, sizeof(data.original)) == sizeof(data.original) && wrapped_read(copy, &data.modified, sizeof(data.modified)) == sizeof(data.modified))
    {
        if (data.original != data.modified)
        {
            wrapped_write(difference, &data, sizeof(data));
        }

        data.offset++;
    }

    close(original);
    close(copy);
    close(difference);
    return 0;
}
