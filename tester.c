#include <unistd.h>
int main() {
    char first[] = "ping\0";
    write(STDOUT_FILENO, first, sizeof(first));
    char second[] = "add\00012\0003\0";
    write(STDOUT_FILENO, second, sizeof(second));
    char third[] = "write\0testing.test\0abcef\0";
    write(STDOUT_FILENO, third, sizeof(third));
    char ls[] = "ls\0thisdir\0";
    write(STDOUT_FILENO, ls, sizeof(ls));
    char nothing[] = "nothing\0";
    write(STDOUT_FILENO, nothing, sizeof(nothing));
    char time[] = "time\0";
    write(STDOUT_FILENO, time, sizeof(time));
    return 0;
}
