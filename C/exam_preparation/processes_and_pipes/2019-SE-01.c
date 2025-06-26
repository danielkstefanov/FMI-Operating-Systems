#include <err.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>

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

pid_t wrapped_fork(void)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        err(5, "Cannot fork");
    }

    return pid;
}

pid_t wrapped_wait(int *status)
{
    pid_t child_pid = wait(status);

    if (child_pid == -1)
    {
        err(3, "Error in wait!");
    }
    return child_pid;
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        errx(1, "Expected at least two args");
    }

    int max_seconds = atoi(argv[1]);

    if (max_seconds == 0)
    {
        errx(1, "Invalid input!");
    }

    if (max_seconds < 1 || max_seconds > 9)
    {
        errx(1, "Invalid secods limit!");
    }

    mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR;
    int log_file = wrapped_open("run.log", O_CREAT | O_TRUNC | O_WRONLY, &mode);

    bool was_last_invalid = false;

    while (true)
    {
        pid_t pid = wrapped_fork();
        time_t start = time(NULL);
        if (pid == 0)
        {
            execvp(argv[2], argv + 2);
            err(2, "Cannot execlp!");
        }

        int status;
        wrapped_wait(&status);
        time_t end = time(NULL);

        int exit_code;

        if (!WIFEXITED(status))
        {
            exit_code = 129;
        }
        else
        {
            exit_code = WEXITSTATUS(status);
        }
        
        char start_str[32];
        char end_str[32];
        char code_str[32];

        snprintf(start_str, sizeof(start_str), "%ld", start);
        snprintf(end_str, sizeof(end_str), "%ld", end);
        snprintf(code_str, sizeof(code_str), "%d", exit_code);

        wrapped_write(log_file, start_str, strlen(start_str));
        wrapped_write(log_file, " ", 1);
        wrapped_write(log_file, end_str, strlen(end_str));
        wrapped_write(log_file, " ", 1);
        wrapped_write(log_file, code_str, strlen(code_str));
        wrapped_write(log_file, "\n", 1);

        bool is_invalid = (end - start > max_seconds) && exit_code != 0;

        if (is_invalid && was_last_invalid)
        {
            break;
        }

        was_last_invalid = is_invalid;
    }

    close(log_file);
    return 0;
}
