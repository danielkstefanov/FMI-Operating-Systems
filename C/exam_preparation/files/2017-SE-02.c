#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

int wrapped_read(int fd, void *data, size_t bytes)
{
    int read_size = read(fd, data, bytes);

    if (read_size < 0)
    {
        err(3, "Cannot read from file descriptor %d!", fd);
    }

    return read_size;
}

int wrapped_write(int fd, const void *data, size_t bytes)
{
    int written_bytes_count = write(fd, data, bytes);

    if (written_bytes_count < 0)
    {
        err(1, "Cannot write in file descriptor %d!", fd);
    }

    return written_bytes_count;
}

void print_number(int fd, int number)
{
    char row_buffer[32];
    int row_number_length = snprintf(row_buffer, sizeof(row_buffer), "%d", number);

    if (wrapped_write(fd, row_buffer, row_number_length) != row_number_length)
    {
        err(2, "Cannot write row number %d!", number);
    }
}

int wrapped_open(const char *filename, int flags, mode_t *modes)
{
    int fd = modes == NULL ? open(filename, flags) : open(filename, flags, *modes);

    if (fd == -1)
    {
        err(2, "Cannot open file %s!", filename);
    }

    return fd;
}

char space_symbol = ' ';
char new_line_symbol = '\n';

int main(int argc, char *argv[])
{
    bool to_print_rows = argc >= 2 && !strcmp(argv[1], "-n");
    bool is_new_row = true;
    int current_row = 1;

    if (argc == 1 || (argc == 2 && to_print_rows))
    {
        char symbol;

        while (wrapped_read(0, &symbol, sizeof(symbol)) > 0)
        {

            if (to_print_rows && is_new_row)
            {
                print_number(1, current_row);
                wrapped_write(1, &space_symbol, sizeof(space_symbol));
                is_new_row = false;
            }

            if (symbol == '\n')
            {
                current_row++;
                wrapped_write(1, &new_line_symbol, sizeof(new_line_symbol));
                is_new_row = true;
            }
            else
            {
                wrapped_write(1, &symbol, sizeof(symbol));
            }
        }
    }
    else
    {
        int start_index = to_print_rows ? 2 : 1;
        for (int i = start_index; i < argc; i++)
        {
            int fd = (!strcmp(argv[i], "-")) ? 0 : wrapped_open(argv[i], O_RDONLY, NULL);

            char symbol;

            while (wrapped_read(fd, &symbol, sizeof(symbol)) > 0)
            {

                if (to_print_rows && is_new_row)
                {
                    print_number(1, current_row);
                    wrapped_write(1, &space_symbol, sizeof(space_symbol));
                    is_new_row = false;
                }

                if (symbol == '\n')
                {
                    current_row++;
                    wrapped_write(1, &new_line_symbol, sizeof(new_line_symbol));
                    is_new_row = true;
                }
                else
                {
                    wrapped_write(1, &symbol, sizeof(symbol));
                }
            }
        }

        return 0;
    }
}
