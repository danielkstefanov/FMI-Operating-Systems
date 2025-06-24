#include <err.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

typedef struct {
    uint32_t temp1;
    uint16_t count;
    uint16_t temp2;
    uint64_t temp3;
} header;

typedef struct {
    uint16_t post_start;
    uint16_t post_count;

    uint16_t pre_start;
    uint16_t pre_count;

    uint16_t in_start;
    uint16_t in_count;

    uint16_t suf_start;
    uint16_t suf_count;
} complect;

int wrapped_open(const char *filename, int flags, mode_t *mode)
{
    int fd = (mode == NULL ? open(filename, flags) : open(filename, flags, *mode));

    if (fd < 0)
    {
        err(2, "Cannot open file %s!", filename);
    }

    return fd;
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

ssize_t wrapped_read(int fd, void* buff, size_t read_size) {
    ssize_t readed_bytes_count = read(fd, buff, read_size);

    if (readed_bytes_count == -1) {
        err(3, "Cannot read!");
    }

    return readed_bytes_count;
}

ssize_t wrapped_write(int fd, const void* buf, size_t bytes_to_write) {
    ssize_t write_size = write(fd, buf, bytes_to_write);

    if (write_size == -1) {
        err(7, "Cannot write!");
    }

    return write_size;
}

int main(int argc, char* argv[]) {

        if (argc != 7) {
        errx(1, "Invalid argc count!");
        }

        int affix_fd = wrapped_open(argv[1], O_RDONLY, NULL);
        int postfix_fd = wrapped_open(argv[2], O_RDONLY, NULL);
        int prefix_fd = wrapped_open(argv[3], O_RDONLY, NULL);
        int infix_fd = wrapped_open(argv[4], O_RDONLY, NULL);
        int suffix_fd = wrapped_open(argv[5], O_RDONLY, NULL);

        mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR;
        int crucifixus_fd = wrapped_open(argv[6], O_WRONLY | O_CREAT | O_TRUNC, &mode);

    header affix_header;
    wrapped_read(affix_fd, &affix_header, sizeof(header));

    complect current_complect;
    while(wrapped_read(affix_fd, &current_complect, sizeof(complect))) {
        wrapped_lseek(postfix_fd, 16 + (current_complect.post_start * sizeof(uint32_t)), SEEK_SET);
       for (int j = 0; j < current_complect.post_count; j++) {
           uint32_t el;
           wrapped_read(postfix_fd, (void*)&el, sizeof(el));
           wrapped_write(crucifixus_fd, (void*)&el, sizeof(el));
       }

       wrapped_lseek(prefix_fd, 16+ (current_complect.pre_start * sizeof(uint8_t)), SEEK_SET);
       for (int j = 0; j < current_complect.pre_count; j++) {
           uint8_t el;
           wrapped_read(prefix_fd, (void*)&el, sizeof(el));
           wrapped_write(crucifixus_fd, (void*)&el, sizeof(el));
       }

       wrapped_lseek(infix_fd, 16+ (current_complect.in_start * sizeof(uint16_t)), SEEK_SET);
       for (int j = 0; j < current_complect.in_count; j++) {
           uint16_t el;
           wrapped_read(infix_fd, (void*)&el, sizeof(el));
           wrapped_write(crucifixus_fd, (void*)&el, sizeof(el));
       }

       wrapped_lseek(suffix_fd, 16 + (current_complect.suf_start * sizeof(uint64_t)), SEEK_SET);
       for (int j = 0; j < current_complect.suf_count; j++) {
           uint64_t el;
           wrapped_read(suffix_fd, (void*)&el, sizeof(el));
           wrapped_write(crucifixus_fd, (void*)&el, sizeof(el));
       }

    }

    close(affix_fd);
    close(postfix_fd);
    close(prefix_fd);
    close(infix_fd);
    close(postfix_fd);
    close(crucifixus_fd);

    return 0;
}
