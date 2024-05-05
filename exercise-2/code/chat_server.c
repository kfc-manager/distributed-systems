#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        perror("port of the server must be provided");
        return EXIT_FAILURE;
    }

    if (argc > 2) {
        perror("only one argument is allowed");
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    if (port > 20020 || port < 20001) {
        perror("port must be in range of 20001-20020");
        return EXIT_FAILURE;
    }

    // create the socket file descriptor, AF_INET means we want to use IPv4 addresses 
    // and SOCK_DGRAM means we want to use Datagram (a connectionless communication)
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socket_fd < 0) {
        perror("socket file descriptor could not be created");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // listen on all network interfaces of the machine
    server_addr.sin_port = htons(port); // set the port on which to listen

    // bind the socket
    if (bind(socket_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("socket could not be bound");
        return EXIT_FAILURE;
    }

    // buffer to store the received messages
    char buffer[1024];

    unsigned int len = sizeof(client_addr);
    int n;
    time_t current_time;
    struct tm *tm_info;

    while (1) {
        // receive the message and prepare buffer
        n = recvfrom(socket_fd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&client_addr, &len);
        buffer[n] = '\0';

        // get the current time and parse the timestamp into a string
        current_time = time(NULL);
        tm_info = localtime(&current_time);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

        // print the timestamp and the message
        printf("[%s] ", timestamp);
        unsigned int ip = client_addr.sin_addr.s_addr;
        printf("[Origin: %u.%u.%u.%u:%u] ", ip & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, (ip >> 24) & 0xFF, client_addr.sin_port);
        printf("%s\n", buffer);

        char* ack = "Acknowledged";

        // send acknowledge to the client
        sendto(socket_fd, (const char *)ack, strlen(ack), MSG_CONFIRM, (const struct sockaddr *)&client_addr, len);
    }

    close(socket_fd);

    return EXIT_SUCCESS;
}
