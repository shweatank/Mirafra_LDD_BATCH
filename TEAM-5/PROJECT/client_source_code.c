/* sudo code 
 *
 * creating the socket
 *
 * connect to the server
 *
 * receive data 
 *
 * perform operation
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/delay.h>

#define SERVER_IP "192.168.0.104"  // Server IP address
#define PORT 6543                  // Port for communication
#define BUFFER_SIZE 512
#define GPIO_PIN 529               // GPIO pin for the LED

static struct socket *sock;            // Socket structure for communication
static struct timer_list data_timer;   // Timer to periodically check for server data

// Function to initialize GPIO for the LED
static int init_gpio(void) {
    int ret;

    // Request GPIO pin for LED
    ret = gpio_request(GPIO_PIN, "LED_GPIO");
    if (ret) {
        printk(KERN_ERR "Failed to request GPIO pin %d\n", GPIO_PIN);
        return ret;
    }

    // Set GPIO pin as output
    ret = gpio_direction_output(GPIO_PIN, 0);
    if (ret) {
        printk(KERN_ERR "Failed to set GPIO pin %d as output\n", GPIO_PIN);
        gpio_free(GPIO_PIN);
        return ret;
    }

    printk(KERN_INFO "GPIO pin %d initialized for LED control\n", GPIO_PIN);
    return 0;
}

// Function to clean up GPIO resources
static void cleanup_gpio(void) {
    gpio_free(GPIO_PIN);  // Free GPIO pin used for LED
    printk(KERN_INFO "GPIO pin %d cleaned up\n", GPIO_PIN);
}

// Function to blink the LED (this simulates the GPIO interrupt handler)
static void blink_led(void) {
    gpio_set_value(GPIO_PIN, 1); // Turn LED on
    msleep(1000);                // Keep LED on for 1 second
    gpio_set_value(GPIO_PIN, 0); // Turn LED off
    printk(KERN_INFO "LED blinked\n");
}

// Function to simulate GPIO interrupt handler when server sends data
static void gpio_irq_handler(void) {
    printk(KERN_INFO "Simulated GPIO interrupt triggered\n");
    blink_led();  // Call blink_led to simulate GPIO interrupt action
}

/**
* It provides a mechanism to handle data along with ancillary (control) information

struct msghdr {
    void         *msg_name;       // Optional address (e.g., destination or source)
    socklen_t     msg_namelen;    // Length of the address in msg_name
    struct iovec *msg_iov;        // Pointer to an array of iovec structures
    int           msg_iovlen;     // Number of elements in the iovec array
    void         *msg_control;    // Pointer to ancillary data (control messages)
    size_t        msg_controllen; // Length of ancillary data
    int           msg_flags;      // Flags received during a recvmsg() call
};
*/


// Function to receive data from the server and simulate the GPIO interrupt
static void receive_data(struct timer_list *t) {
    struct msghdr msg;
    struct kvec iov;
    char buffer[BUFFER_SIZE];
    int ret;

    memset(&msg, 0, sizeof(msg));      // Clear message header
    memset(buffer, 0, sizeof(buffer)); // Clear data buffer

    // Prepare the buffer for receiving data
    iov.iov_base = buffer;      //starting address of the buffer
    iov.iov_len = BUFFER_SIZE;  //length of the buffer

    // Receive data from the server
    ret = kernel_recvmsg(sock, &msg, &iov, 1, BUFFER_SIZE, 0);
    if (ret < 0) {
        printk(KERN_ERR "Error receiving data: %d\n", ret);
    } else if (ret == 0) {
        printk(KERN_INFO "Connection closed by server\n");
    } else {
        printk(KERN_INFO "Received data: %s\n", buffer);
        // Simulate GPIO interrupt handler when data is received
        gpio_irq_handler();
    }

    // Re-start the timer to continue checking for data periodically
    mod_timer(&data_timer, jiffies + msecs_to_jiffies(1000)); // 1-second interval
}

// Function to initialize the socket and connect to the server
static int init_socket(void) {
    struct sockaddr_in server_addr;
    int ret;
/**
* &init_net is a global variable that represents the default (or initial) network namespace in the Linux kernel.
* Create a socket for communication
*/
    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
    if (ret < 0) {
        printk(KERN_ERR "Socket creation failed: %d\n", ret);
        return ret;
    }

    // Configure server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IP address from string to binary format
    ret = in4_pton(SERVER_IP, -1, (u8 *)&server_addr.sin_addr.s_addr, -1, NULL);
    if (ret <= 0) {
        printk(KERN_ERR "Invalid server IP address\n");
        sock_release(sock);
        return -EINVAL;
    }

    // Connect to the server
    ret = kernel_connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr), 0);
    if (ret < 0) {
        printk(KERN_ERR "Connection failed: %d\n", ret);
        sock_release(sock);
        return ret;
    }

    printk(KERN_INFO "Connected to server at %s:%d\n", SERVER_IP, PORT);
    return 0;
}

// Function to set up the timer for periodic data reception
static void setup_timer(void) {
    // Initialize the timer to call receive_data function
    timer_setup(&data_timer, receive_data, 0);
    // Start the timer to call receive_data every 1 second
    mod_timer(&data_timer, jiffies + msecs_to_jiffies(1000)); 
}

// Module initialization function
static int __init kernel_client_init(void) {
    int ret;

    printk(KERN_INFO "Initializing kernel client module with simulated GPIO interrupt and timer\n");

    // Initialize GPIO for LED control
    ret = init_gpio();
    if (ret) {
        return ret; // Return error if GPIO initialization fails
    }

    // Initialize socket and connect to the server
    ret = init_socket();
    if (ret) {
        cleanup_gpio();  // Clean up GPIO if socket initialization fails
        return ret;
    }

    // Set up the timer to receive data periodically from the server
    setup_timer();

    return 0;
}

// Module cleanup function
static void __exit kernel_client_exit(void) {
    // Stop the timer and clean up resources
    del_timer(&data_timer);
    sock_release(sock);  // Release the socket connection
    cleanup_gpio();      // Free GPIO resources

    printk(KERN_INFO "Kernel client module unloaded\n");
}

module_init(kernel_client_init);   // Module initialization entry point
module_exit(kernel_client_exit);   // Module cleanup entry point

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Team 5");
MODULE_DESCRIPTION("Kernel module for client socket communication with simulated GPIO interrupt control and timer");
