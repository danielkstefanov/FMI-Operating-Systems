#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>

char TEMP_FILE_NAME[] = "/tmp/tmp_file_nameXXXXXX";
const char TERMINATING_ZERO = '\0';

int wrapped_unlink(const char *file_name)
{
    int res = unlink(file_name);

    if (res == -1)
    {
        err(1, "Cannot unlink file!");
    }

    return res;
}

void wrapped_err(int error_code, const char *error_message)
{
    wrapped_unlink(TEMP_FILE_NAME);
    err(error_code, error_message);
}

off_t wrapped_lseek(int fd, off_t offset, int whence)
{
    off_t ls = lseek(fd, offset, whence);

    if (ls < 0)
    {
        wrapped_err(errno, "Failed to lseek");
    }

    return ls;
}

int get_file_size(int fd)
{
    struct stat sb;

    if (fstat(fd, &sb) < 0)
    {
        wrapped_err(4, "Cannot stat file");
    }

    return sb.st_size;
}

int wrapped_open(const char *filename, int flags, mode_t *mode)
{
    int fd = (mode == NULL ? open(filename, flags) : open(filename, flags, *mode));

    if (fd < 0)
    {
        wrapped_err(2, "Cannot open file!");
    }

    return fd;
}

int wrapped_write(int fd, const void *buffer, int size)
{
    int write_size = write(fd, buffer, size);

    if (write_size < 0)
    {
        wrapped_err(6, "Cannot write!");
    }

    return write_size;
}

int wrapped_read(int fd, void *buffer, int size)
{
    int read_size = read(fd, buffer, size);

    if (read_size < 0)
    {
        wrapped_err(5, "Cannot read");
    }

    return read_size;
}

void copy_file(int fd1, int fd2)
{
    wrapped_lseek(fd1, 0, SEEK_SET);
    wrapped_lseek(fd2, 0, SEEK_SET);
    char ch;

    while (wrapped_read(fd1, &ch, sizeof(ch)))
    {
        wrapped_write(fd2, &ch, sizeof(ch));
    }
}

typedef struct
{
    uint64_t data[64];
} record;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        errx(1, "Expected only one arg!");
    }

    int input_file = wrapped_open(argv[1], O_RDWR, NULL);
    int input_file_size = get_file_size(input_file);

    if (input_file_size % sizeof(record))
    {
        errx(1, "Invalid file format!");
    }

    int temp_file = mkstemp(TEMP_FILE_NAME);
    if (temp_file == -1)
    {
        err(2, "Failed to open temp file!");
    }

    for (int i = 0; i < input_file_size; i++)
    {
        wrapped_write(temp_file, &TERMINATING_ZERO, sizeof(char));
    }

    record current;
    int record_index = 0;

    while (wrapped_read(input_file, &current, sizeof(current)))
    {

        wrapped_lseek(temp_file, record_index * sizeof(record), SEEK_SET);
        wrapped_write(temp_file, &current, sizeof(current));

        if (current.data[0] == 0)
        {
            break;
        }

        wrapped_lseek(input_file, current.data[0] * sizeof(record), SEEK_SET);
        record_index = current.data[0];
    }

    copy_file(temp_file, input_file);

    wrapped_unlink(TEMP_FILE_NAME);
    close(input_file);
    close(temp_file);
    return 0;
}