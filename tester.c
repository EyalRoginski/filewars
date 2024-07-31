#include <unistd.h>
int main() {
    char first[] = "ping\0";
    write(STDOUT_FILENO, first, sizeof(first));
    char second[] = "add\00012\0003\0";
    write(STDOUT_FILENO, second, sizeof(second));
    return 0;
}
