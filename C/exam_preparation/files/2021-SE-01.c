#include <err.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

int wrapped_open(const char *filename, int flags, mode_t *modes)
{
    int result = modes == NULL ? open(filename, flags) : open(filename, flags, *modes);

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

int main(int argc, char* argv[]) {

        if (argc != 3) {
        errx(1, "Invalid args count!");
        }

    int src = wrapped_open(argv[1], O_RDONLY, NULL);

//    mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR;
    mode_t mode = 0777;
    int dest = wrapped_open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, &mode);

    uint8_t current;
    while (wrapped_read(src, &current, sizeof(current))) {

        uint16_t to_write = 0;
        uint8_t mask = 128;
        int shift_counter = 15;
        while(mask >= 1) {

            uint16_t to_write_mask = 1;

            if (current & mask) {
                to_write_mask <<= shift_counter;
                to_write |= to_write_mask;
                shift_counter -= 2;
            }
            else {
                shift_counter--;
                to_write_mask <<= shift_counter;
                to_write |= to_write_mask;
                shift_counter--;
            }

            mask >>= 1;
        }

        uint8_t* newBytePtr = (uint8_t*)&to_write;

        wrapped_write(dest, &newBytePtr[1], sizeof(uint8_t));
        wrapped_write(dest, &newBytePtr[0], sizeof(uint8_t));
    }

    close(src);
    close(dest);
    return 0;
}
