/*
 * NAME : PODDUTURI MANASA REDDY
 * KM_ID : KM54BESD03
 * DATE :
 * FILE_NAME :
 * SAMPLE_INPUT :
 * SAMPLE_OUTPUT :
 *
 * */


#include <stdio.h>    // Standard I/O library
#include <string.h>   // String handling functions
#include <unistd.h>   // POSIX API for system calls
#include <sys/socket.h> // For socket programming
#include <netinet/in.h> // Structures for internet addresses
#include <arpa/inet.h>  // For inet_addr() and inet_ntoa() functions
#include <fcntl.h>     // File control options

#define DEVICE_FILE "/dev/gpio_device" // Device file path for GPIO control
#define PORT 8080  // Port number for the server

int main(int argc, char **argv) {
    // Check if server IP address is provided as a command-line argument
    if (argc != 2) {
        printf("usage: ./server server_ip_address\n");
        return 0; // Exit if incorrect usage
    }

    int sfd, new_sfd, len; // File descriptors for socket and new connection
    struct sockaddr_in server_id, client_id; // Structures for server and client addresses
    char buffer[20]; // Buffer to store incoming messages
    int fd; // File descriptor for the device file
    socklen_t addr_len = sizeof(client_id); // Size of client address structure

    // Open the GPIO device file for writing (to control LED)
    fd = open(DEVICE_FILE, O_WRONLY);
    if (fd < 0) { // Check if device file opened successfully
        perror("Open device file");
        return 0; // Exit on failure
    }

    // Create a socket (IPv4, TCP)
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) { // Check if socket was created successfully
        perror("Socket");
        close(fd); // Close device file
        return 0; // Exit on failure
    }

    // Set up the server address structure
    server_id.sin_family = AF_INET; // Use IPv4
    server_id.sin_port = htons(PORT); // Convert port number to network byte order
    server_id.sin_addr.s_addr = inet_addr(argv[1]); // Set IP address from command-line argument
    len = sizeof(server_id); // Length of server address structure

    // Bind the socket to the server address and port
    if (bind(sfd, (struct sockaddr*)&server_id, len) < 0) {
        perror("Bind"); // Print error if bind fails
        close(fd); // Close device file
        close(sfd); // Close socket
        return 0; // Exit on failure
    }

    // Listen for incoming client connections
    if (listen(sfd, 5) < 0) { // Max 5 pending connections
        perror("Listen"); // Print error if listen fails
        close(fd); // Close device file
        close(sfd); // Close socket
        return 0; // Exit on failure
    }

    printf("Server listening on %s:%d...\n", argv[1], PORT); // Indicate server is ready

    // Accept a connection from a client
    new_sfd = accept(sfd, (struct sockaddr*)&client_id, &addr_len);
    if (new_sfd < 0) { // Check if accept was successful
        perror("Accept");
        close(fd); // Close device file
        close(sfd); // Close socket
        return 0; // Exit on failure
    }

    printf("Client connected: %s\n", inet_ntoa(client_id.sin_addr)); // Print client IP address

    // Receive a command from the client
    if (recv(new_sfd, buffer, sizeof(buffer), 0) < 0) {
        perror("Receive"); // Print error if receive fails
        close(fd); // Close device file
        close(new_sfd); // Close client socket
        close(sfd); // Close server socket
        return 0; // Exit on failure
    }

    // Process the received command and control the LED accordingly
    if (strncmp(buffer, "on", 2) == 0) { // Check if command is "on"
        write(fd, "on", 2); // Write "on" to device file to turn LED on
        printf("Received 'on' command. Turning LED ON.\n");
    } else if (strncmp(buffer, "off", 3) == 0) { // Check if command is "off"
        write(fd, "off", 3); // Write "off" to device file to turn LED off
        printf("Received 'off' command. Turning LED OFF.\n");
    } else { // Handle invalid commands
        printf("Invalid command received: %s\n", buffer);
    }

    // Close all open file descriptors
    close(new_sfd); // Close client connection
    close(sfd); // Close server socket
    close(fd); // Close device file

    return 0; // Exit successfully
}

