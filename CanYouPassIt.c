#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char command[256];
    snprintf(command, sizeof(command), "cat %s", "CanYouPassIt");

    int result = system(command);
    return 0;    
}
