#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t magic;
    uint32_t count;
} data_header;

typedef struct {
    uint32_t magic1;
    uint16_t magic2;
    uint16_t reserved;
    uint64_t count;
} comp_header;

typedef struct {
    uint16_t type;
    uint16_t reserved1;
    uint16_t reserved2;
    uint16_t reserved3;
    uint32_t offset1;
    uint32_t offset2;
} data_block;

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

int get_file_size(int fd)
{
    struct stat sb;

    if (fstat(fd, &sb) < 0)
    {
        err(4, "Cannot stat file desc %d!", fd);
    }

    return sb.st_size;
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

void swap(int fd1, uint64_t num1, uint64_t num2, uint32_t offset1, uint32_t offset2) {
    uint64_t tmp = num1;
    num1 = num2;
    num2 = tmp;

    lseek(fd1, offset1 * sizeof(uint64_t), SEEK_SET);
    wrapped_write(fd1, &num1, sizeof(num1));

    wrapped_lseek(fd1, offset2 * sizeof(uint64_t), SEEK_SET);
    wrapped_write(fd1, &num2, sizeof(num2));
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        errx(1, "Invalid args count!");
    }

    int data = wrapped_open(argv[1], O_RDONLY, NULL);
    data_header data_info;
    wrapped_read(data, &data_info, sizeof(data_header));
    int data_file_size = get_file_size(data);

    if (data_info.magic != 0x21796F4A || (long unsigned int)data_file_size != sizeof(data_header) + data_info.count * sizeof(uint64_t)) {
        errx(2, "Invalid data file format!");
    }

    int comparator = wrapped_open(argv[2], O_RDWR, NULL);
    comp_header comp_info;
    wrapped_read(comparator, &comp_info, sizeof(comp_header));
    int comp_file_size = get_file_size(comparator);

    if (comp_info.magic1 != 0xAFBC7A37 || comp_info.magic2 != 0x1C27 || (long unsigned int)comp_file_size != sizeof(comp_header) + comp_info.count * sizeof(data_block)) {
        errx(3, "Invalid comp file format!");
    }

    data_block current_block;
    while (wrapped_read(comparator, &current_block, sizeof(data_block))) {

        if ((current_block.type != 0 && current_block.type != 1) || current_block.reserved1 != 0 || current_block.reserved2 != 0 || current_block.reserved3 != 0) {
            errx(4, "Invalid comparator data format!");
        }

        if (current_block.offset1 > data_info.count || current_block.offset2 > data_info.count) {
            errx(5, "Invalid offset!");
        }

        uint64_t left;
        wrapped_lseek(data, sizeof(data_header) + sizeof(uint64_t) * current_block.offset1, SEEK_SET);
        wrapped_read(data, &left, sizeof(left));

        uint64_t right;
        wrapped_lseek(data, sizeof(data_header) + sizeof(uint64_t) * current_block.offset2, SEEK_SET);
        wrapped_read(data, &right, sizeof(right));

        if((current_block.type == 0 && left > right) || (current_block.type == 1 && left < right)) {
            swap(data, left, right, current_block.offset1, current_block.offset2);
        }
    }

    close(data);
    close(comparator);

    return 0;
}
