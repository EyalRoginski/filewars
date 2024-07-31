#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8082
#define BUFFER_SIZE 1024

void print_flags() {}

void challenge() {
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

void process_mission() {
    char mission_name[8] = "";
    read_until_null(mission_name);

    if (strcmp(mission_name, "ping") == 0) {
        ping();
    } else if (strcmp(mission_name, "add") == 0) {
        add();
    }
}

int main(int argc, char **argv) {
    challenge();
    for (int i = 0; i < 100; i++) {
        process_mission();
    }
}
