#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>

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

int wrapped_fstat(int fd, struct stat* info) {
    int result = fstat(fd, info);

    if (result == -1) {
        err(3, "Cannot stat fd %d", fd);
    }

    return result;
}

const int ARR_MAX_LENGTH = 524288;
char array_delimeter[] = ", ";
char array_end[] = "};\n";

int main(int argc, char* argv[]) {

        if (argc != 3){
        errx(1, "Invalid argc count: Expected two!");
        }

    int input_file_descriptor = wrapped_open(argv[1], O_RDONLY, NULL);
    struct stat input_file_info;

    wrapped_fstat(input_file_descriptor, &input_file_info);

    if ((long int) input_file_info.st_size > (long int) sizeof(uint16_t) * ARR_MAX_LENGTH) {
        errx(7, "Input file too big!");
    }

    uint32_t arr_size = input_file_info.st_size / sizeof(uint16_t);

    mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR;
    int output_file_descriptor = wrapped_open(argv[2], O_RDWR | O_CREAT | O_TRUNC, &mode);

    char str1[] = "#include <stdint.h>\n\n";
    char str2[] = "uint32_t arrN = ";
    char str3[] = "const uint16_t arr[] = {";

    wrapped_write(output_file_descriptor, (void*)&str1, strlen(str1));
    wrapped_write(output_file_descriptor, (void*)&str2, strlen(str2));
    char output[1024];
    int len = snprintf(output, sizeof(output), "%d;\n", arr_size);
    wrapped_write(output_file_descriptor, (void*)&output, len);
    wrapped_write(output_file_descriptor, (void*)&str3, strlen(str3));

    if (arr_size != 0) {
        uint16_t current_number;
        wrapped_read(input_file_descriptor, &current_number, sizeof(current_number));

        len = snprintf(output, sizeof(output), "%d", current_number);
        wrapped_write(output_file_descriptor, (void*)&output, len);

        while (wrapped_read(input_file_descriptor, &current_number, sizeof(current_number))) {
            wrapped_write(output_file_descriptor, (void*) &array_delimeter, sizeof(array_delimeter) - 1);
            len = snprintf(output, sizeof(output), "%d", current_number);
            wrapped_write(output_file_descriptor, (void*)&output, len);

        }
    }
    
    wrapped_write(output_file_descriptor, (void*) &array_end, sizeof(array_end) - 1);

    close(input_file_descriptor);
    close(output_file_descriptor);
    return 0;
}
