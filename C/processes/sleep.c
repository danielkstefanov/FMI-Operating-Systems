#include <unistd.h>
#include <err.h>

int main(void) {

        if (execlp("sleep", "sleep", "60", NULL) < 0) {
        err(1, "Cannot invoke sleep");
        }

        return 0;
}
