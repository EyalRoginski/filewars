#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 8082
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    char *server_ip = "127.0.0.1";
    int server_socket, portno;
    struct sockaddr_in server_addr;
    char group_number;
    char buffer[BUFFER_SIZE];


    group_number = 1;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("ERROR: Failed to create socket.\n");
        exit(1);
    }

    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR: Failed to connect to server.\n");
        exit(1);
    }


    if (send(server_socket, &group_number, 1, 0) < 0) {
        perror("ERROR: Failed to send group number.\n");
        exit(1);
    }

    char send_buffer[BUFFER_SIZE] =  {0};
    memset(send_buffer, 0xff, BUFFER_SIZE);
    if (send(server_socket, send_buffer, BUFFER_SIZE, 0) < 0) {
        perror("ERROR: Failed to send group number.\n");
        exit(1);
    }



    if (recv(server_socket, buffer, BUFFER_SIZE, 0) < 0) {
        perror("ERROR: Failed to receive response from server.\n");
        exit(1);
    }

    printf("%s\n", buffer);

    close(server_socket);

    return 0;
}