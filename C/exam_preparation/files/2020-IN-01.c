#include <err.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>

typedef struct {
    uint32_t magic;
    uint8_t header_version;
    uint8_t data_version;
    uint16_t count;
    uint32_t res1;
    uint32_t res2;
} header;

typedef struct {
    uint16_t offset;
    uint8_t original;
    uint8_t new;
} first_version;

typedef struct {
    uint32_t offset;
    uint16_t original;
    uint16_t new;
} second_version;

ssize_t wrapped_write(int fd, const void* buf, size_t bytes_to_write) {
    ssize_t write_size = write(fd, buf, bytes_to_write);

    if (write_size == -1) {
        err(7, "Cannot write!");
    }

    return write_size;
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

ssize_t wrapped_read(int filedes, void* obj, size_t read_size) {
    ssize_t result = read(filedes, obj, read_size);

    if (result == -1) {
        err(6, "Cannot read!");
    }

    return result;
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

void copy_file(int fd1, int fd2) {
    wrapped_lseek(fd1, 0, SEEK_SET);
    wrapped_lseek(fd2, 0, SEEK_SET);
    char ch;

    while(wrapped_read(fd1, &ch, sizeof(ch))){
        wrapped_write(fd2, &ch, sizeof(ch));
    }

    wrapped_lseek(fd1, 0, SEEK_SET);
    wrapped_lseek(fd2, 0, SEEK_SET);
}

int main(int argc, char* argv[]) {

        if (argc != 4) {
            errx(1, "Invalid args count!");
        }

        int patch = wrapped_open(argv[1], O_RDONLY, NULL);
        int source = wrapped_open(argv[2], O_RDONLY, NULL);

        mode_t mode = 0666;
        int dest = wrapped_open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, &mode);

    header header_data;
    wrapped_read(patch, &header_data, sizeof(header_data));

    if (header_data.magic != 0xEFBEADDE || header_data.header_version != 0x01 || (header_data.data_version != 0x01 && header_data.data_version != 0x00)) {
        errx(2, "Invalid file format");
    }

    copy_file(source, dest);
    first_version first_data;
    second_version second_data;
    for (uint16_t i = 0; i < header_data.count; i++) {
        if (header_data.data_version == 0x00){
            wrapped_read(patch, &first_data, sizeof(first_version));
            wrapped_lseek(source, first_data.offset, SEEK_SET);

            uint8_t current;
            wrapped_read(source, &current, sizeof(current));

            if (current != first_data.original) {
                errx(2, "Invalid file data 1");
            }

            wrapped_lseek(dest, first_data.offset, SEEK_SET);
            wrapped_write(dest, &first_data.new, sizeof(first_data.new));
        }
        else {
            wrapped_read(patch, &second_data, sizeof(second_version));
            wrapped_lseek(source, second_data.offset, SEEK_SET);

            uint16_t current;
            wrapped_read(source, &current, sizeof(current));

            if (current != second_data.original) {
                errx(2, "Invalid file data 2");
            }

            wrapped_lseek(dest, second_data.offset, SEEK_SET);
            wrapped_write(dest, &second_data.new, sizeof(second_data.new));
        }
    }

    close(patch);
    close(source);
    close(dest);

    return 0;
}
