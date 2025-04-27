#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
         errx(1 , "Expected only one arg");
    }

    int src = open(argv[1], O_RDONLY);

    if (src == -1) {
         err(1, "Cannot open file for reading");
     }

    char ch;
    int read_size;

    int chars_count = 0;
    int rows_count = 0;
    int words_count = 0;

    bool in_word = false;

    while ((read_size = read(src, &ch, sizeof(ch))) > 0) {

        chars_count++;

        if (ch == '\n') {
            rows_count++;

            if (in_word) {
                words_count++;
                in_word = false;
            }
        }
        else if (ch == ' ' && in_word) {
            words_count++;
            in_word = false;
        }
        else {
            in_word = true;
        }
    }

    if (read_size == -1) {
        errx(2, "Cannot read from file");
    }

    printf("%d %d %d %s \n", rows_count, words_count, chars_count, argv[1]);
    return 0;
}
   