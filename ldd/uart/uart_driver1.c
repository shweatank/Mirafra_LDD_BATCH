#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/serial.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#define UART_PORT "/dev/ttys0" /* write the driver name */

// File operation function prototypes
static int uart_open(struct inode *inode, struct file *file);
static int uart_release(struct inode *inode, struct file *file);
static ssize_t uart_read(struct file *file, char __user *buf, size_t len, loff_t *offset);
static ssize_t uart_write(struct file *file, const char __user *buf, size_t len, loff_t *offset);

// Define UART file operations
static struct file_operations uart_fops = {
    .owner = THIS_MODULE,
    .open = uart_open,
    .release = uart_release,
    .read = uart_read,
    .write = uart_write,
};

// Global file pointer for UART device
static struct file *uart_file = NULL;

// UART Open function - Opens the UART device
static int uart_open(struct inode *inode, struct file *file)
{
    // Open the UART device
    uart_file = filp_open(UART_PORT, O_RDWR | O_NOCTTY | O_NONBLOCK, 0);
    if (IS_ERR(uart_file)) {
        pr_err("Failed to open UART device %s\n", UART_PORT);
        return PTR_ERR(uart_file);  // Return the error code
    }

    pr_info("UART device opened successfully\n");
    return 0;
}

// UART Release function - Closes the UART device
static int uart_release(struct inode *inode, struct file *file)
{
    if (uart_file && !IS_ERR(uart_file)) {
        filp_close(uart_file, NULL);
        pr_info("UART device closed\n");
    }
    return 0;
}

// UART Read function - Reads data from UART device
static ssize_t uart_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    ssize_t ret;

    if (!uart_file || IS_ERR(uart_file)) {
        pr_err("UART device is not opened\n");
        return -ENODEV;  // No such device
    }

    ret = kernel_read(uart_file, buf, len, offset);
    if (ret < 0) {
        pr_err("Failed to read from UART device\n");
        return ret;  // Return read error
    }

    pr_info("Read %zd bytes from UART device\n", ret);
    return ret;
}

// UART Write function - Writes data to UART device
static ssize_t uart_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
    ssize_t ret;

    if (!uart_file || IS_ERR(uart_file)) {
        pr_err("UART device is not opened\n");
        return -ENODEV;  // No such device
    }

    ret = kernel_write(uart_file, buf, len, offset);
    if (ret < 0) {
        pr_err("Failed to write to UART device\n");
        return ret;  // Return write error
    }

    pr_info("Written %zd bytes to UART device\n", ret);
    return ret;
}

// Module Init function - Registers the character device
static int __init uart_init(void)
{
    int ret;
    pr_info("UART Module Loaded\n");

    // Register the character device with file operations
    ret = register_chrdev(0, "ttys0", &uart_fops);
    if (ret < 0) {
        pr_err("Failed to register UART driver\n");
        return ret;
    }

    pr_info("UART driver registered successfully %d \n",ret);
    return 0;
}

// Module Exit function - Unregisters the character device
static void __exit uart_exit(void)
{
    pr_info("UART Module Unloaded\n");

    // Unregister the character device
    unregister_chrdev(0, "uart_driver");
}

module_init(uart_init);
module_exit(uart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("A Simple UART Driver Module");

