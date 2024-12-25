#include <wiringPi.h>
#include <wiringPiSerial.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int serialPort;
    char receivedChar;
    char sendString[] = "Hello, this is a test string!";  // String to send

    // Initialize wiringPi
    wiringPiSetup();

    // Open the serial port (ttyAMA0 or ttyS0 depending on your setup)
    serialPort = serialOpen("/dev/serial0", 9600);  // You can also try "/dev/ttyAMA0" or "/dev/ttyS0"

    if (serialPort < 0) {
        printf("Unable to open serial port\n");
        return -1;
    }

    printf("Serial Port Opened\n");

    // Loopback Test: Send the string and immediately check if it's received
    while (1) {
        // Send the string using serialPuts
        serialPuts(serialPort, sendString);

        // Wait for the data to be transmitted and received
        usleep(100000);  // Sleep for 100ms

        // Check if any data is available
        if (serialDataAvail(serialPort)) {
            // Read the received characters until there's no more data
            printf("Received: ");
            while (serialDataAvail(serialPort)) {
                receivedChar = serialGetchar(serialPort);  // Read a character
                printf("%c", receivedChar);  // Print the received character
            }
            printf("\n");
        } else {
            printf("No data received\n");
        }

        usleep(500000);  // Sleep for 500ms before sending the next string
    }

    return 0;
}
