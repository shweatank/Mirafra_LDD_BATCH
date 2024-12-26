/*
 * NAME : PODDUTURI MANASA REDDY
 * KM_ID : KM54BESD03
 * DATE :
 * FILE_NAME :
 * SAMPLE_INPUT :
 * SAMPLE_OUTPUT :
 *
 * */


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>

#define DEVICE_FILE "/dev/gpio_device"
#define PORT 8080
#define IP "192.168.0.179"
int client_request(char *);
int main()
{

	char message[20];
	int fd;
	char flag_str[2];  // Buffer to store the string value of the flag
	int flag = 0;

	// Open device file
	fd = open(DEVICE_FILE, O_RDONLY);
	if (fd < 0) {
		perror("Open device file");
		return 0;
	}
	printf("Waiting for interrupt...\n");

	// Read the flag from the device
	if (read(fd, flag_str, sizeof(flag_str)) < 0) {
		perror("Read");
		close(fd);
		return 0;
	}

	// Convert the string to an integer
	flag = atoi(flag_str);	// Convert "1" or "0" to integer 1 or 0
	printf("%d\n",flag);


	// Check the flag and take action if interrupt triggered
	if (flag == 1) {
		printf("Interrupt triggered! Choose LED action:\n");
		printf("1. Turn LED ON\n");
		printf("2. Turn LED OFF\n");
		printf("Enter your choice (1/2): ");
		int choice;
		scanf("%d", &choice);

		// Handle user input with switch case
		switch (choice) {
			case 1:
				strcpy(message, "on");
				break;
			case 2:
				strcpy(message, "off");
				break;
			default:
				printf("Invalid choice.\n");
				return 0;
		}
		client_request(message);
	}
	// Optionally, add a small delay to avoid busy waiting
	sleep(1);  // Sleep for 1 second before checking again

	close(fd);
}
// Create socket
int client_request(char *msg)
{
	int sfd, len;
	struct sockaddr_in client_id;
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0) {
		perror("Socket");
		close(sfd);
		return 0;
	}

	// Set up server address structure
	client_id.sin_family = AF_INET;
	client_id.sin_port = htons(PORT);  // Port from command line argument
	client_id.sin_addr.s_addr = inet_addr(IP);  // IP address from command line argument
	len = sizeof(client_id);

	// Connect to the server
	if (connect(sfd, (struct sockaddr*)&client_id, len) < 0) {
		perror("Connect");
		close(sfd);
		return 0;
	}
	write(sfd,msg,strlen(msg)+1);

}

