#include <err.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>

const char message[] = "Word not found!\n";

int get_file_size(int fd)
{
    struct stat sb;

    if (fstat(fd, &sb) < 0)
    {
        err(4, "Cannot stat file desc %d!", fd);
    }

    return sb.st_size;
}

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

bool binary_search(char *search_word, int fd)
{
    int left = 0;
    int right = get_file_size(fd);

    while (left < right)
    {
        int mid = left + (right - left) / 2;

        char current;
        while (wrapped_read(fd, &current, sizeof(current)))
        {
            if (current == '\0')
            {
                break;
            }
        }

        char word[64];
        int word_len = 0;
        while (wrapped_read(fd, &current, sizeof(current)))
        {

            if (current == '\n' || current == '\0')
            {
                break;
            }

            word[word_len++] = current;
        }

        word[word_len] = '\0';

        int res = strcmp(word, search_word);

        if (res == 0)
        {
            while (wrapped_read(fd, &current, sizeof(current)))
            {

                if (current == '\0')
                {
                    break;
                }

                wrapped_write(0, &current, sizeof(current));
            }

            return true;
        }
        else if (res < 0)
        {
            right = mid;
        }
        else
        {
            left = mid;
        }
    }

    return false;
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        errx(1, "Invalid args count");
    }

    if (strlen(argv[1]) > 63)
    {
        wrapped_write(0, message, strlen(message));
        return 0;
    }

    int dictionary = wrapped_open(argv[2], O_RDONLY, NULL);

    bool found = binary_search(argv[1], dictionary);

    if (!found)
    {
        wrapped_write(0, message, strlen(message));
    }

    close(dictionary);

    return 0;
}
