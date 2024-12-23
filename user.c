#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define DEVICE_FILE "/dev/gpio_driver"
#define PORT 8080  // Server port

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: ./server server_ip_address\n");
        return 0;
    }

    int sfd, new_sfd, len;
    struct sockaddr_in server_id, client_id;
    char buffer[20];
    int fd;
    socklen_t addr_len = sizeof(client_id);

    // Open device file
    fd = open(DEVICE_FILE, O_WRONLY);  // Open device for writing (to control LED)
    if (fd < 0) {
        perror("Open device file");
        return 0;
    }

    // Create socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        perror("Socket");
        close(fd);
        return 0;
    }

    // Set up server address structure
    server_id.sin_family = AF_INET;
    server_id.sin_port = htons(PORT);
    server_id.sin_addr.s_addr = inet_addr(argv[1]);  // IP address from command line argument
    len = sizeof(server_id);

    // Bind the socket
    if (bind(sfd, (struct sockaddr*)&server_id, len) < 0) {
        perror("Bind");
        close(fd);
        close(sfd);
        return 0;
    }

    // Listen for incoming connections
    if (listen(sfd, 5) < 0) {
        perror("Listen");
        close(fd);
        close(sfd);
        return 0;
    }

    printf("Server listening on %s:%d...\n", argv[1], PORT);

    // Accept incoming client connection
    new_sfd = accept(sfd, (struct sockaddr*)&client_id, &addr_len);
    if (new_sfd < 0) {
        perror("Accept");
        close(fd);
        close(sfd);
        return 0;
    }

    printf("Client connected: %s\n", inet_ntoa(client_id.sin_addr));

    // Receive command from client
    if (recv(new_sfd, buffer, sizeof(buffer), 0) < 0) {
        perror("Receive");
        close(fd);
        close(new_sfd);
        close(sfd);
        return 0;
    }

    // Check received message and perform LED control
    if (strncmp(buffer, "on", 2) == 0) {
        write(fd, "on", 2);  // Write to device to turn LED on
        printf("Received 'on' command. Turning LED ON.\n");
    } else if (strncmp(buffer, "off", 3) == 0) {
        write(fd, "off", 3);  // Write to device to turn LED off
        printf("Received 'off' command. Turning LED OFF.\n");
    } else {
        printf("Invalid command received: %s\n", buffer);
    }

    // Close the socket and device file
    close(new_sfd);
    close(sfd);
    close(fd);

    return 0;
}
