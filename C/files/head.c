/*
	Реализирайте команда head без опции (т.е. винаги да извежда
	на стандартния изход само първите 10 реда от съдържанието на
	файл подаден като първи параматър)
*/

#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {

   if (argc != 2) {
       errx(1, "Expected only one arg");
   }

    int rows_count = 0;
    char ch;
    int read_size;

    int src = open(argv[1], O_RDONLY);

    if (src == -1) {
       err(2, "Cannot open file of reading");
    }

    while ((read_size = read(src, &ch, sizeof(ch))) > 0) {
        if(ch == '\n') {
            rows_count++;
        }

        write(1, &ch, sizeof(ch));

        if(rows_count == 10) {
           break;
        }
    }

    if (read_size == -1) {
        err(3, "Cannot read from file");
    }

    close(src);

    return 0;
}
