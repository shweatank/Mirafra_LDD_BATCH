#include <linux/module.h>        // For module macros and functions
#include <linux/kernel.h>        // For kernel-level logging (e.g., printk)
#include <linux/fs.h>            // For file operations structure
#include <linux/uaccess.h>       // For user-space and kernel-space memory access
#include <linux/io.h>            // For MMIO (Memory-Mapped I/O) functions
#include <linux/cdev.h>          // For character device structure
#include <linux/device.h>        // For creating device nodes

// Device and driver definitions
#define DEVICE_NAME "uart_driver" // Name of the device
#define CLASS_NAME "uart_class"   // Name of the class for device creation

// Base address and offsets for UART hardware
#define UART_BASE_ADDR  0x7e201000  // Example base address for UART MMIO (update as per platform)This is the base physical address of the UART device.
#define UART_REG_OFFSET 0x100     // MMIO region size for UART registers
//UART_REG_OFFSET tells the kernel to map a 256-byte memory region starting at UART_BASE_ADDR.
// UART register offsets
#define UART_DATA       0x00  // Offset for UART data register
#define UART_STATUS     0x04 // Offset for UART status register
#define UART_CONTROL    0x30  // Offset for UART control register

// UART status flags
#define UART_STATUS_RX_READY   (1 << 0)  // Flag indicating data is ready to be read
#define UART_STATUS_TX_READY   (1 << 1)  // Flag indicating transmitter is ready

// Device-related variables
static int major_number;                // Major number assigned to the device
static struct class *uart_class = NULL; // Device class
static struct device *uart_device = NULL; // Device structure
static void __iomem *uart_base = NULL;  // Memory-mapped base address for UART registers

// Function to handle opening the device
static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "UART Driver: Device opened.\n"); // Log device open event
    return 0; // Return success
}

// Function to handle closing the device
static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "UART Driver: Device closed.\n"); // Log device close event
    return 0; // Return success
}

// Function to handle reading from the device
static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    char uart_char;                // Variable to store read character
    unsigned int status;           // Variable to store UART status

    // Check if data is ready to be read
    status = readl(uart_base + UART_STATUS);
    if (!(status & UART_STATUS_RX_READY)) { // If RX_READY flag is not set checks whether the Receiver Ready flag is set in the status register.
        return -EAGAIN; // Return error indicating no data available
    }

    // Read a character from UART data register
    uart_char = readl(uart_base + UART_DATA) & 0xFF;

    // Copy the character to user-space buffer
    if (copy_to_user(buffer, &uart_char, 1)) { // Check for copy failure
        return -EFAULT; // Return error if copy fails
    }

    printk(KERN_INFO "UART Driver: Read character '%c'.\n", uart_char); // Log the read character
    return 1; // Indicate one byte has been read
}

// Function to handle writing to the device
static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    char uart_char;                // Variable to store character to write
    unsigned int status;           // Variable to store UART status

    if (len == 0) {                // If no data to write
        return 0;                  // Return 0 indicating nothing was written
    }

    // Copy the first byte of data from user space to kernel space
    if (copy_from_user(&uart_char, buffer, 1)) { // Check for copy failure
        return -EFAULT; // Return error if copy fails
    }

    // Check if the transmitter is ready to accept data
    status = readl(uart_base + UART_STATUS);//unsigned int readl(void __iomem *addr);
    if (!(status & UART_STATUS_TX_READY)) { // If TX_READY flag is not set
        return -EAGAIN; // Return error indicating transmitter is busy
    }

    // Write the character to UART data register
    writel(uart_char, uart_base + UART_DATA);//void writel(u32 value, void __iomem *addr);
    printk(KERN_INFO "UART Driver: Sent character '%c'.\n", uart_char); // Log the sent character
    return 1; // Indicate one byte has been written
}

// File operations structure
static struct file_operations fops = {
    .open = dev_open,       // Map open to dev_open
    .release = dev_release, // Map release to dev_release
    .read = dev_read,       // Map read to dev_read
    .write = dev_write,     // Map write to dev_write
};

// Function to initialize the driver
static int __init uart_driver_init(void) {
    printk(KERN_INFO "UART Driver: Initializing UART driver.\n"); // Log driver initialization

    // Map the UART MMIO region
    uart_base = ioremap(UART_BASE_ADDR, UART_REG_OFFSET);//This maps the physical address 0x4000C000 to a virtual address (uart_base).Now, the driver can use uart_base to safely access UART registers in kernel space.

    if (!uart_base) { // Check for remap failure
        printk(KERN_ALERT "UART Driver: Failed to remap UART base address.\n");
        return -ENOMEM; // Return error if remap fails
    }

    // Register a character device and allocate a major number
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) { // Check for registration failure
        printk(KERN_ALERT "UART Driver: Failed to register a major number.\n");
        return major_number; // Return error code
    }
    printk(KERN_INFO "UART Driver: Registered with major number %d.\n", major_number);

    // Create a device class
    uart_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(uart_class)) { // Check for class creation failure
        unregister_chrdev(major_number, DEVICE_NAME); // Cleanup on failure
        printk(KERN_ALERT "UART Driver: Failed to register device class.\n");
        return PTR_ERR(uart_class); // Return error code
    }

    // Create the device
    uart_device = device_create(uart_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(uart_device)) { // Check for device creation failure
        class_destroy(uart_class); // Cleanup on failure
        unregister_chrdev(major_number, DEVICE_NAME); // Cleanup
        printk(KERN_ALERT "UART Driver: Failed to create the device.\n");
        return PTR_ERR(uart_device); // Return error code
    }

    printk(KERN_INFO "UART Driver: Device created successfully.\n"); // Log success
    return 0; // Indicate successful initialization
}

// Function to clean up the driver
static void __exit uart_driver_exit(void) {
    iounmap(uart_base); // Unmap the UART MMIO region
    device_destroy(uart_class, MKDEV(major_number, 0)); // Destroy the device
    class_destroy(uart_class); // Destroy the device class
    unregister_chrdev(major_number, DEVICE_NAME); // Unregister the character device
    printk(KERN_INFO "UART Driver: Exiting and cleaning up.\n"); // Log cleanup
}

// Register initialization and cleanup functions
module_init(uart_driver_init);
module_exit(uart_driver_exit);

// Module metadata
MODULE_LICENSE("GPL");               // License of the module
MODULE_AUTHOR("Prashant");           // Author information
MODULE_DESCRIPTION("A simple UART device driver."); // Description of the module
MODULE_VERSION("1.0");               // Module version

