#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        perror("exactly three arguements must be provided");
        return EXIT_FAILURE;
    }

    int port = atoi(argv[2]);
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

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    server_addr.sin_port = htons(port); // set the port on which to listen
                                        
    int n;
    unsigned int len;
    
    char msg[50] = "Hello from the client ";
    strcat(msg, argv[3]);

    // buffer to store the received messages
    char buffer[1024];
		
	sendto(socket_fd, (const char *)msg, strlen(msg),
		MSG_CONFIRM, (const struct sockaddr *) &server_addr,
			sizeof(server_addr));
			
	n = recvfrom(socket_fd, (char *)buffer, 1024,
				MSG_WAITALL, (struct sockaddr *) &server_addr,
				&len);
	buffer[n] = '\0';
    printf("From the server: %s\n", buffer);

    close(socket_fd);

    return EXIT_SUCCESS;
}
