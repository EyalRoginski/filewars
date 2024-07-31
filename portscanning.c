#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define PORT_START 1
#define PORT_END 65535
#define TIMEOUT 1 // Timeout in seconds

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
    result = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
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

int main() {
    const char *ip_address = "127.0.0.1"; // Localhost IP address
    uint8_t byte = 0x01;

    for (int port = PORT_START; port <= PORT_END; ++port) {
        send_byte_to_port(ip_address, port, byte);
    }

    return 0;
}
