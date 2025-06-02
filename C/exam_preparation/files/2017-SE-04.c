#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int wrapped_read(int fd, void *buffer, int size)
{
    int read_size = read(fd, buffer, size);

    if (read_size < 0)
    {
        err(5, "Cannot read from fd %d", fd);
    }

    return read_size;
}

int wrapped_open(const char* pathname, int flags, mode_t* mode) {
    int fildes = mode == NULL ? open(pathname, flags): open(pathname, flags, &mode);

    if(fildes < 0) {
        err(1, "Cannot open!");
    }

    return fildes;
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

int main(int argc, char* argv[]) {

    char current_symbol;

    if (argc == 1) {
        while(wrapped_read(0, &current_symbol, sizeof(current_symbol))){
            wrapped_write(1, &current_symbol, sizeof(current_symbol));
        }
    }
    else {
        for (int i = 1; i < argc; i++) {

            int read_descriptor = 0;

            if(strcmp(argv[i], "-")) {
                read_descriptor = wrapped_open(argv[i], O_RDONLY, NULL);
            }

            while(wrapped_read(read_descriptor, &current_symbol, sizeof(current_symbol))){
                wrapped_write(1, &current_symbol, sizeof(current_symbol));
            }
        }
    }

    return 0;
}
