/*
 * NAME : PODDUTURI MANASA REDDY
 * KM_ID : KM54BESD03
 * DATE :
 * FILE_NAME :
 * SAMPLE_INPUT :
 * SAMPLE_OUTPUT :
 *
 * */

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

// Log levels
typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} LogLevel;

// Macro to simplify logging
#define LOG(level, format, ...) \
    do { \
        log_message(level,__FILE__, __FUNCTION__, __LINE__, format, ##VA_ARGS_); \
    } while (0)

// Function to convert log level to string
const char* log_level_to_string(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_INFO:  return "INFO";
        case LOG_LEVEL_WARN:  return "WARN";
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

// Function to log messages
void log_message(LogLevel level, const char *file, const char *function, int line, const char *format, ...) {
    // Get current time
    time_t raw_time;
    struct tm *time_info;
    char time_buffer[20];
    
    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", time_info);

    // Print log metadata
    fprintf(stderr, "%s [%s] %s:%d in %s: ", 
            time_buffer, 
            log_level_to_string(level), 
            file, 
            line, 
            function);

    // Print formatted message
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, "\n");
}

int main() {
    int value = 42;

    // Example usage of the logging macro
    LOG(LOG_LEVEL_DEBUG, "This is a debug message. Value = %d", value);
    LOG(LOG_LEVEL_INFO, "Starting the application...");
    LOG(LOG_LEVEL_WARN, "Warning: Disk space low.");
    LOG(LOG_LEVEL_ERROR, "Error occurred in processing.");
    LOG(LOG_LEVEL_FATAL, "Fatal error: system crash imminent!");

    return 0;
}
