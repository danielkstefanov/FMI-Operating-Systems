// копирайте съдържанието на файл1 във файл2

#include <fcntl.h>
#include <err.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "Expected two args.");
    }

    int src;
    int dest;
    char currentSymbol;

    src=open(argv[1], O_RDONLY);

    if (src == -1) {
        err(2, "Cannot open file for reading.");
    }

    dest=open(argv[2], O_WRONLY | O_CREAT , 0666);

    ssize_t readSize;

    while((readSize=read(src, &currentSymbol, sizeof(currentSymbol)))>0) {
        write(dest, &currentSymbol, readSize);
    }

    if (readSize == -1) {
        errx(3, "Cannot read");
    }

    close(src);
    close(dest);

    return 0;
}