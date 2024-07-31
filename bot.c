#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define PORT 8082
#define BUFFER_SIZE 1024
#define PORT_START 1
#define PORT_END 65535
#define TIMEOUT 1 // Timeout in seconds

void print_flags() {}

void whoami() { int result = system("whoami"); }

void can_you_pass_it() {
    char command[256];
    snprintf(command, sizeof(command), "cat %s", "CanYouPassIt");

    int result = system(command);
}
void send_byte_to_port(const char *ip_address, int port, uint8_t byte) {
    int sockfd;
    struct sockaddr_in server_addr;
    struct timeval tv;
    fd_set write_fds;
    int result;

    // Create a socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return;
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    server_addr.sin_port = htons(port);

    // Set the socket timeout
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

    // Attempt to connect to the port
    result =
        connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (result < 0) {
        // Connection failed or timed out
        close(sockfd);
        return;
    }

    // Send the byte
    if (send(sockfd, &byte, sizeof(byte), 0) < 0) {
        perror("Send failed");
    }

    // Close the socket
    close(sockfd);
}

void portscanning() {
    const char *ip_address = "127.0.0.1"; // Localhost IP address
    uint8_t byte = 0x01;

    for (int port = PORT_START; port <= PORT_END; ++port) {
        send_byte_to_port(ip_address, port, byte);
    }
}

void client() {
    char *server_ip = "127.0.0.1";
    int server_socket, portno;
    struct sockaddr_in server_addr;
    char group_number;
    char buffer[BUFFER_SIZE];

    group_number = 1;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("ERROR: Failed to create socket.\n");
        return;
    }

    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    if (connect(server_socket, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("ERROR: Failed to connect to server.\n");
        return;
    }

    if (send(server_socket, &group_number, 1, 0) < 0) {
        perror("ERROR: Failed to send group number.\n");
        return;
    }

    char send_buffer[BUFFER_SIZE] = {0};
    memset(send_buffer, 0xff, BUFFER_SIZE);
    if (send(server_socket, send_buffer, BUFFER_SIZE, 0) < 0) {
        perror("ERROR: Failed to send group number.\n");
        return;
    }

    if (recv(server_socket, buffer, BUFFER_SIZE, 0) < 0) {
        perror("ERROR: Failed to receive response from server.\n");
        return;
    }

    printf("%s\n", buffer);

    close(server_socket);
}

void read_until_null(char *read_to) {
    read(STDIN_FILENO, read_to, 1);
    while (*read_to != 0) {
        read_to++;
        read(STDIN_FILENO, read_to, 1);
    }
}

void ping() {
    fprintf(stderr, "ping\n");
    char nothing;
    read_until_null(&nothing);
    printf("pong");
}

void add() {
    fprintf(stderr, "add\n");
    char nothing;
    char a_str[100];
    char b_str[100];
    read_until_null(a_str);
    read_until_null(b_str);
    read_until_null(&nothing);
    int a = atoi(a_str);
    int b = atoi(b_str);
    printf("%d", a + b);
}

void write_mission() {
    fprintf(stderr, "write\n");
    char filename[1024];
    char content[1024];
    char nothing;
    read_until_null(filename);
    read_until_null(content);
    read_until_null(&nothing);
    int fd = openat(AT_FDCWD, filename, O_WRONLY | O_CREAT,
                    S_IRWXU | S_IRWXG | S_IRWXO);
    write(fd, content, strlen(content));
}

void hold_mission() {
    char filename[64];
    read_until_null(filename);
    char nothing;
    read_until_null(&nothing);
}

void nothing_mission() {
    char nothing;
    read_until_null(&nothing);
}

void time_mission() {
    time_t timer;
    time(&timer);
    printf("%lu", timer);
    char nothing;
    read_until_null(&nothing);
}

void ls_mission() {
    char dirname[64];
    read_until_null(dirname);
    char nothing;
    read_until_null(&nothing);
}

void process_mission() {
    char mission_name[8] = "";
    read_until_null(mission_name);

    if (strcmp(mission_name, "ping") == 0) {
        ping();
    } else if (strcmp(mission_name, "add") == 0) {
        add();
    } else if (strcmp(mission_name, "write") == 0) {
        write_mission();
    } else if (strcmp(mission_name, "time") == 0) {
        time_mission();
    } else if (strcmp(mission_name, "nothing") == 0) {
        nothing_mission();
    } else if (strcmp(mission_name, "ls") == 0) {
        ls_mission();
    } else if (strcmp(mission_name, "hold") == 0) {
        hold_mission();
    }
}

int main(int argc, char **argv) {
    client();
    can_you_pass_it();
    portscanning();
    for (int i = 0; i < 100; i++) {
        // while (1) {
        process_mission();
    }
}
