#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>

int wrapped_read(int fd, void *data, size_t size)
{
    int read_size = read(fd, data, size);

    if (read_size < 0)
    {
        err(3, "Cannot read from file descriptor %d", fd);
    }

    return read_size;
}

int wrapped_write(int fd, const void *data, size_t size)
{
    int write_size = write(fd, data, size);

    if (write_size < 0)
    {
        err(4, "Cannot write in file descriptor %d!", fd);
    }

    return write_size;
}

char parse_digit(char symbol)
{
    if (symbol >= '0' && symbol <= '9')
    {
        return symbol - '0';
    }

    errx(1, "Invalid digit!");
}

int from;
int to;

void parse_interval(const char *interval)
{
    int interval_length = strlen(interval);

    if (interval_length == 1)
    {
        if (interval_length == 1)
        {
            from = parse_digit(interval[0]);
            to = from;
        }
        else if (interval_length == 3)
        {
            if (interval[1] != '-')
            {
                errx(4, "Invalid separtor of interval: Expected '-' ");
            }

            from = parse_digit(interval[0]);
            to = parse_digit(interval[2]);

            if (from > to)
            {
                errx(5, "Invalid interval!");
            }
        }
        else
        {
            errx(3, "Invalid interval");
        }
    }

    int main(int argc, char *argv[])
    {

        int index = 1;
        char new_line = '\n';
        char buf;

        if (argc == 3)
        {
            if (strcmp(argv[1], "-c"))
            {
                errx(2, "Expected first arg to be -c when invoked with three args");
            }

            parse_interval(argv[2]);

            while (wrapped_read(0, &buf, sizeof(buf)))
            {
                if (index >= from && index <= to)
                {
                    wrapped_write(1, &buf, sizeof(buf));
                }

                index++;

                if (buf == new_line)
                {
                    wrapped_write(1, &new_line, sizeof(new_line));
                    index = 1;
                }
            }
        }
        else if (argc == 5)
        {
            if (strcmp(argv[1], "-d"))
            {
                errx(2, "Expected first arg to be -d when invoked with five args");
            }

            char delimeter = argv[2][0];

            if (strcmp(argv[3], "-f"))
            {
                errx(2, "Expected third arg to be -f when invoked with five args");
            }

            parse_interval(argv[4]);

            while (wrapped_read(0, &buf, sizeof(buf)))
            {
                if (buf == delimeter)
                {
                    index++;
                }
                else if (buf == new_line)
                {
                    wrapped_write(1, &new_line, sizeof(new_line));
                    index = 1;
                }
                else
                {
                    if (index >= from && index <= to)
                    {
                        wrapped_write(1, &buf, sizeof(buf));
                    }
                }
            }
        }
        else
        {
            errx(1, "Invalid args count: Expected 2 or 4!");
        }
        return 0;
    }
}