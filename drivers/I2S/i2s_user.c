/*
 * audio_play.c - User-space program to send audio data to I2S virtual driver
 * 
 * This program opens a /dev/i2s_audio device node and writes audio data
 * (e.g., from a .wav file) to the virtual I2S driver for simulated playback.
 * 
 * Compilation:
 *   gcc -o audio_play audio_play.c
 * 
 * Usage:
 *   ./audio_play <audio_file.wav>
 */

#include <stdio.h>      // For printf, perror
#include <fcntl.h>      // For open system call
#include <unistd.h>     // For write, close system calls
#include <stdlib.h>     // For malloc, free
#include <sys/stat.h>   // For file status
#include <string.h>     // For memset

#define DEVICE_PATH "/dev/i2s_audio" // Path to the I2S virtual driver
#define BUFFER_SIZE 1024            // Buffer size for reading audio data in chunks

int main(int argc, char *argv[])
{
    int dev_fd, audio_fd;           // File descriptors for the device and audio file
    ssize_t bytes_read, bytes_written; // Variables for reading and writing data
    char buffer[BUFFER_SIZE];       // Buffer to hold chunks of audio data

    /* --- Check for correct usage --- */
    if (argc != 2) {
        printf("Usage: %s <audio_file.wav>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* --- Open the I2S virtual device --- */
    dev_fd = open(DEVICE_PATH, O_WRONLY);
    if (dev_fd < 0) {
        perror("Failed to open I2S device");
        return EXIT_FAILURE;
    }

    printf("Opened I2S device at %s successfully.\n", DEVICE_PATH);

    /* --- Open the audio file --- */
    audio_fd = open(argv[1], O_RDONLY);
    if (audio_fd < 0) {
        perror("Failed to open audio file");
        close(dev_fd);
        return EXIT_FAILURE;
    }

    printf("Opened audio file: %s\n", argv[1]);

    /* --- Read and write audio data in chunks --- */
    while ((bytes_read = read(audio_fd, buffer, BUFFER_SIZE)) > 0) {
        /* Write the chunk of audio data to the I2S device */
        bytes_written = write(dev_fd, buffer, bytes_read);
        if (bytes_written < 0) {
            perror("Failed to write to I2S device");
            break;
        }
        printf("Sent %zd bytes of audio data to I2S device.\n", bytes_written);
    }

    if (bytes_read < 0) {
        perror("Failed to read audio file");
    }

    /* --- Cleanup: Close file descriptors --- */
    close(audio_fd);
    close(dev_fd);

    printf("Audio playback simulation complete.\n");

    return EXIT_SUCCESS;
}
