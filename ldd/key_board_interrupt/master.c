//#include <stdio.h>
#include <linux/string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <linux/stdlib.h>
#include <arpa/inet.h>

#define buffer_size 1024
#define port 6543

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int adderlen = sizeof(address);
    char buffer[buffer_size] = {0};
    char response[256];

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Failed to create endpoint\n");
        exit(EXIT_FAILURE);
    }

    // Configure the server address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Failed to bind\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening
    if (listen(server_fd, 3) < 0) {
        perror("Failed to listen\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Accept a client connection
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&adderlen)) < 0) {
        perror("Failed to accept the connection\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Communication loop
    while (1) {
        int val = read(new_socket, buffer, buffer_size);
        if (val > 0) {
//            printf("Received: %s\n", buffer);
  //          printf("Enter the data to send:\n");
            fgets(response, sizeof(response), stdin);
            response[strcspn(response, "\n")] = 0; // Remove newline character
            send(new_socket, response, strlen(response), 0);
            memset(buffer, 0, buffer_size);
        } else if (val == 0) {
    //        printf("Client disconnected\n");
            break;
        }
    }

    // Close sockets
    close(new_socket);
    close(server_fd);
    return 0;
}

