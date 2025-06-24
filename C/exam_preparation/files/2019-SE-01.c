#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

typedef struct {
    uint32_t id;
    uint16_t temp1;
    uint16_t temp2;
    uint32_t start;
    uint32_t end;
} user_info;

typedef struct {
    uint32_t user_id;
    uint32_t duration;
} session;

ssize_t wrapped_write(int fd, const void* buf, size_t bytes_to_write) {
    ssize_t write_size = write(fd, buf, bytes_to_write);

    if (write_size == -1) {
        err(7, "Cannot write!");
    }

    return write_size;
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

int wrapped_fstat(int fd, struct stat* info) {
    int result = fstat(fd, info);

    if (result == -1) {
        err(3, "Cannot stat fd %d", fd);
    }

    return result;
}

ssize_t wrapped_read(int filedes, void* obj, size_t read_size) {
    ssize_t result = read(filedes, obj, read_size);

    if (result == -1) {
        err(6, "Cannot read!");
    }

    return result;
}

double calc_dispertion(user_info* sessions, uint32_t nums_count, double average)
{
    double sum = 0;

    for (uint32_t i = 0; i < nums_count; i++) {
        int duration = sessions[i].end - sessions[i].start;

        double diff = duration - average;
        sum += (diff * diff);
    }

    return (sum / nums_count);
}

int contains(session* sessions, int count, uint32_t id) {
    for(int i = 0; i < count; i++) {
        if(sessions[i].user_id == id) {
            return i;
        }
    }

    return -1;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        errx(1, "Invalid args count");
    }

    int fd = wrapped_open(argv[1], O_RDONLY, NULL);

    struct stat sb;
    wrapped_fstat(fd, &sb);

    if (sb.st_size % (sizeof(uint32_t) * 4) != 0) {
        errx(2, "Invalid file format");
    }

    user_info all_sessions_info[16384];
    int sessions_count = sb.st_size / (sizeof(uint32_t) * 4);

    wrapped_read(fd, &all_sessions_info, sizeof(user_info) * sessions_count);

    int sum = 0;

    for(int i = 0; i < sessions_count; i++){
        double duration = all_sessions_info[i].end - all_sessions_info[i].start;

        if (duration < 0) {
            errx(3,"Invalid data");
        }

        sum += duration;
    }

    double average = sum / sessions_count;
    double dispersion = calc_dispertion(all_sessions_info, sessions_count,average);

    session best_sessions[2048];
    int unique_users_count = 0;

    for (int i = 0; i < sessions_count; i++) {

        double duration = all_sessions_info[i].end - all_sessions_info[i].start;

        if (duration * duration > dispersion) {
            int index = contains(best_sessions, unique_users_count, all_sessions_info[i].id);

            if (index != -1) {
                if (duration > best_sessions[index].duration) {
                    best_sessions[index].duration = duration;
                }
            }
            else {
                best_sessions[unique_users_count] = (session){ all_sessions_info[i].id, duration };
                unique_users_count++;
            }
        }
    }

    mode_t mode = 0666;
    int output = wrapped_open("best_sessions.dat", O_CREAT | O_TRUNC | O_WRONLY, &mode);
    for (int i = 0; i < unique_users_count; i++) {
        wrapped_write(output, &best_sessions[i].user_id, sizeof(uint32_t));
        wrapped_write(output, &best_sessions[i].duration, sizeof(uint32_t));
    }

    close(fd);
    return 0;
}
