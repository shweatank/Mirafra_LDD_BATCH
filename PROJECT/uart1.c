#include <linux/kernel.h>// Kernel-level logging functions
#include <linux/module.h>// Required for writing kernel modules
#include <linux/fs.h>// File system related functions and structures
#include <linux/uaccess.h>// For user-space access functions like copy_to_user, copy_from_user
#include <linux/init.h> // Required for module initialization functions
#include <linux/gpio.h>  // GPIO functions (not used in the code but included)
#include <linux/serial.h>// Serial port related functions (not used here, but could be useful)
#include <linux/errno.h>// Error codes
#include <linux/cdev.h>// Character device management

#define UART_PORT "/dev/ttys0"  // UART device file path, adjust as needed

// File operation function prototypes
static int uart_open(struct inode *inode, struct file *file);
static int uart_release(struct inode *inode, struct file *file);
static ssize_t uart_read(struct file *file, char __user *buf, size_t len, loff_t *offset);
static ssize_t uart_write(struct file *file, const char __user *buf, size_t len, loff_t *offset);

// Define UART file operations structure
static struct file_operations uart_fops = {
    .owner = THIS_MODULE,            // Indicates the owner of the module
    .open = uart_open,               // File open function
    .release = uart_release,         // File release (close) function
    .read = uart_read,               // File read function
    .write = uart_write,             // File write function
};

// Global variable for storing the file pointer to the UART device
static struct file *uart_file = NULL; // File pointer for UART device (defined in fs.h header)

// UART Open function - Opens the UART device and initializes the file pointer
static int uart_open(struct inode *inode, struct file *file)
{
    // Attempt to open the UART device for reading and writing (non-blocking mode)
    uart_file = filp_open(UART_PORT, O_RDWR | O_NOCTTY | O_NONBLOCK, 0);
    if (IS_ERR(uart_file)) {
        pr_err("Failed to open UART device %s\n", UART_PORT);
        return PTR_ERR(uart_file);  // Return error code if opening fails
    }

    pr_info("UART device opened successfully\n");
    return 0;  // Return 0 if successful
}

// UART Release function - Closes the UART device
static int uart_release(struct inode *inode, struct file *file)
{
    if (uart_file && !IS_ERR(uart_file)) {
        filp_close(uart_file, NULL);  // Close the UART device
        pr_info("UART device closed\n");
    }
    return 0;  // Return 0 on success
}

// UART Read function - Reads data from the UART device
static ssize_t uart_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    ssize_t ret;

    // Check if UART device is open
    if (!uart_file || IS_ERR(uart_file)) {
        pr_err("UART device is not opened\n");
        return -ENODEV;  // Return error if UART device is not open
    }

    // Read data from UART into user-space buffer
    ret = kernel_read(uart_file, buf, len, offset);
    if (ret < 0) {
        pr_err("Failed to read from UART device\n");
        return ret;  // Return error if reading fails
    }

    pr_info("Read %zd bytes from UART device\n", ret);  // Log how many bytes were read
    return ret;  // Return the number of bytes read
}

// UART Write function - Writes data to the UART device
static ssize_t uart_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
    ssize_t ret;

    // Check if UART device is open
    if (!uart_file || IS_ERR(uart_file)) {
        pr_err("UART device is not opened\n");
        return -ENODEV;  // Return error if UART device is not open
    }

    // Write data from user-space buffer to UART
    ret = kernel_write(uart_file, buf, len, offset);
    if (ret < 0) {
        pr_err("Failed to write to UART device\n");
        return ret;  // Return error if writing fails
    }

    pr_info("Written %zd bytes to UART device\n", ret);  // Log how many bytes were written
    return ret;  // Return the number of bytes written
}

// Module Init function - Registers the character device and prepares the UART driver
static int __init uart_init(void)
{
    int ret;
    pr_info("UART Module Loaded\n");

    // Register the character device with the system, providing the file operations
    ret = register_chrdev(0, "ttys0", &uart_fops);
    if (ret < 0) {
        pr_err("Failed to register UART driver\n");
        return ret;  // Return error if registration fails
    }

    pr_info("UART driver registered successfully %d \n", ret);
    return 0;  // Return 0 if successful
}

// Module Exit function - Unregisters the character device and cleans up
static void __exit uart_exit(void)
{
    pr_info("UART Module Unloaded\n");

    // Unregister the character device driver
    unregister_chrdev(0, "uart_driver");
}

module_init(uart_init);  // Specify the init function for module load
module_exit(uart_exit);  // Specify the exit function for module unload

MODULE_LICENSE("GPL");  // License type, used by the kernel
MODULE_AUTHOR("Sujitha");  // Author of the module
MODULE_DESCRIPTION("A Simple UART Driver Module");  // Short description of the module
