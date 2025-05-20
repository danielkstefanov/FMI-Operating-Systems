#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int wrapped_read(int fd, void *data, size_t size)
{
    int read_size = read(fd, data, size);

    if (read_size < 0)
    {
        err(3, "Cannot read from file descriptor %d", fd);
    }

    return read_size;
}

off_t wrapped_lseek(int fd, off_t offset, int whence)
{
    off_t ls = lseek(fd, offset, whence);

    if (ls < 0)
    {
        err(errno, "Failed to lseek in %d", fd);
    }

    return ls;
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

int wrapped_open(const char *filename, int flags, mode_t *modes)
{
    int fd = modes ? open(filename, flags, *modes) : open(filename, flags);

    if (fd < 0)
    {
        err(2, "Cannot open file %s!", filename);
    }

    return fd;
}

int get_file_size(int fd)
{
    struct stat sb;

    if (fstat(fd, &sb) < 0)
    {
        err(3, "Cannot stat file desc %d!", fd);
    }

    return sb.st_size;
}

void copy_file(int src_fd, int dest_fd)
{
    char ch;

    while (wrapped_read(src_fd, &ch, sizeof(ch)))
    {
        wrapped_write(dest_fd, &ch, sizeof(ch));
    }
}

typedef struct
{
    uint16_t offset;
    uint8_t original;
    uint8_t new;
} patch_info;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        errx(1, "Invalid args count: Expected three");
    }

    int patch = wrapped_open(argv[1], O_RDONLY, NULL);

    if (get_file_size(patch) % sizeof(patch_info) != 0)
    {
        err(4, "Invalid patch file format!");
    }

    int original = wrapped_open(argv[2], O_RDONLY, NULL);
    int original_filesize = get_file_size(original);

    // mode_t modes = S_IRUSR | S_WRUSR;
    mode_t modes = 0777;

    int new = wrapped_open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, &modes);

    copy_file(original, new);

    patch_info current_change;

    uint8_t symbol;
    while (wrapped_read(patch, &current_change, sizeof(current_change)))
    {
        if (current_change.offset > original_filesize)
        {
            err(6, "Invalid offset!");
        }

        wrapped_lseek(original, current_change.offset, SEEK_SET);
        wrapped_read(original, &symbol, sizeof(symbol));

        printf("%d\n", symbol);

        if (symbol != current_change.original)
        {
            err(7, "Invalid patch file!");
        }

        wrapped_lseek(new, current_change.offset, SEEK_SET);
        wrapped_write(new, &current_change.new, sizeof(current_change.new));
    }

    close(original);
    close(patch);
    close(new);
    return 0;
}
