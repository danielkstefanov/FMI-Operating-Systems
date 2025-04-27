#include <unistd.h>
#include <fcntl.h>
#include <err.h>

int main(int argc, char* argv[]) {

    if (argc != 3) {
        errx(1, "Expected two args only");
    }

    int first_file;

    if ((first_file = open(argv[1], O_RDWR)) == -1) {
        err(2, "Cannot open file");
    }

    int second_file;

    if ((second_file = open(argv[2], O_RDWR)) == -1) {
        err(2, "Cannot open file");
    }

    int temp_file;

    if ((temp_file = open("temp_file_49283", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR)) == -1) {
        err(3, "Cannot create temp file");
    }

    char ch;
    int read_size;

    while ((read_size = read(first_file, &ch, sizeof(ch))) > 0) {

        if (read_size == 0) {
            break;
        }

        write(temp_file, &ch, read_size);
    }

    if (read_size == -1) {
        err(5, "Error while reading from file");
    }

    close(first_file);

    if ((first_file = open(argv[1], O_RDWR | O_TRUNC)) == -1) {
        err(2, "Cannot open file");
    }

    while ((read_size = read(second_file, &ch, sizeof(ch))) > 0) {
        if (read_size == 0) {
            break;
        }

        write(first_file, &ch, read_size);
    }

    if (read_size == -1) {
        err(5, "Error while reading from file");
    }

    if (lseek(temp_file, 0, SEEK_SET) == -1) {
        err(6, "Cannot position in temp file");
    }

    close(second_file);

    if ((second_file = open(argv[2], O_RDWR | O_TRUNC)) == -1) {
        err(2, "Cannot open file");
    }

    while ((read_size = read(temp_file, &ch, sizeof(ch))) > 0) {
        if (read_size == 0) {
            break;
        }

        write(second_file, &ch, read_size);
    }

    if (read_size == -1) {
        err(5, "Error while reading from file");
    }

    close(first_file);
    close(second_file);
    close(temp_file);

    return 0;
}
