#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>

int wrapped_open(const char *filename, int flags, mode_t *modes)
{
    int fd = modes == NULL ? open(filename, flags) : open(filename, flags, *modes);

    if (fd == -1)
    {
        err(2, "Cannot open file %s!", filename);
    }

    return fd;
}

int wrapped_lseek(int fd, off_t offset, int whence)
{
    off_t res = lseek(fd, offset, whence);

    if (res < 0)
    {
        err(6, "Cannot lseek in file descriptor %d", fd);
    }

    return res;
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

int wrapped_write(int fd, const void *buf, size_t bytes)
{
    int write_size = write(fd, buf, bytes);

    if (write_size < 0)
    {
        err(5, "Cannot write in file descriptor %d!", fd);
    }

    return write_size;
}

uint8_t message_separator = 0x55;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        errx(1, "Invalid args count: Expected two");
    }

    //mode_t modes = S_IRUSR | S_WRUSR;
    mode_t modes = 0777;

    int stream = wrapped_open(argv[1], O_RDONLY, NULL);
    int messages = wrapped_open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, &modes);

    uint8_t symbol;

    while (wrapped_read(stream, &symbol, sizeof(symbol)) != sizeof(symbol))
    {
        if (symbol != message_separator)
        {
            continue;
        }

        uint8_t nested_message_offset = 0;

        uint8_t message_data[256];
        message_data[0] = message_separator;

        uint8_t N;
        wrapped_read(stream, &N, sizeof(N));
        message_data[1] = N;

        if (N < 3)
        {
            errx(4, "N should be < 3");
        }

        uint8_t accumulated_checksum = message_separator;
        accumulated_checksum ^= N;

        for (int i = 2; i < N - 1; i++)
        {
            wrapped_read(stream, &message_data[i], sizeof(uint8_t));
            accumulated_checksum ^= message_data[i];

            if (message_data[i] == message_separator && nested_message_offset == 0)
            {
                nested_message_offset = i;
            }
        }

        uint8_t correct_checksum;
        wrapped_read(stream, &correct_checksum, sizeof(correct_checksum));

        if (accumulated_checksum == correct_checksum)
        {
            message_data[N-1] = correct_checksum;
            wrapped_write(messages, &message_data, sizeof(uint8_t) * N);
        }

        if (nested_message_offset != 0)
        {
            wrapped_lseek(stream, -N - nested_message_offset, SEEK_CUR);
        }
    }

    close(stream);
    close(messages);

    return 0;
}
