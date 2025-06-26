#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>

off_t wrapped_lseek(int fd, off_t offset, int whence)
{
    off_t ls = lseek(fd, offset, whence);

    if (ls < 0)
    {
        err(errno, "Failed to lseek in %d", fd);
    }

    return ls;
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

int wrapped_open(const char *filename, int flags, mode_t *mode)
{
    int fd = (mode == NULL ? open(filename, flags) : open(filename, flags, *mode));

    if (fd < 0)
    {
        err(2, "Cannot open file %s!", filename);
    }

    return fd;
}

int wrapped_write(int fd, const void *buffer, int size)
{
    int write_size = write(fd, buffer, size);

    if (write_size < 0)
    {
        err(6, "Cannot write in fd: %d!", fd);
    }

    return write_size;
}

int wrapped_read(int fd, void *buffer, int size)
{
    int read_size = read(fd, buffer, size);

    if (read_size < 0)
    {
        err(5, "Cannot read from file %d", fd);
    }

    return read_size;
}

typedef struct
{
    uint16_t offset;
    uint8_t length;
    uint8_t separator;
} idx_info;

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        errx(1, "Invalid args count: Expected 4!");
    }

    int first_dat = wrapped_open(argv[1], O_RDONLY, NULL);
    int first_idx = wrapped_open(argv[2], O_RDONLY, NULL);

    int first_idx_size = get_file_size(first_idx);

    if (first_idx_size % sizeof(idx_info))
    {
        err(3, "Invalid idx file %s!", argv[2]);
    }

    // mode_t mode = S_IRUSR | S_IWUSR;
    mode_t mode = 0777;

    int second_dat = wrapped_open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, &mode);
    int second_idx = wrapped_open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, &mode);

    idx_info current_info;

    uint16_t current_offset = 0;
    char ch;

    while (wrapped_read(first_idx, &current_info, sizeof(current_info)) == sizeof(current_info))
    {
        wrapped_lseek(first_dat, current_info.offset, SEEK_SET);
        wrapped_read(first_dat, &ch, sizeof(ch));

        if (ch >= 'A' && ch <= 'Z')
        {
            wrapped_write(second_dat, &ch, sizeof(ch));

            for (uint8_t i = 0; i < current_info.length - 1; i++)
            {
                wrapped_read(first_dat, (void *)&ch, sizeof(ch));
                wrapped_write(second_dat, (const void *)&ch, sizeof(ch));
            }

            idx_info info_to_save = {current_offset, current_info.length, 0x00};
            current_offset += current_info.length;
            wrapped_write(second_idx, (const void *)&info_to_save, sizeof(info_to_save));
        }
    }

    close(first_dat);
    close(first_idx);
    close(second_dat);
    close(second_idx);

    return 0;
}
