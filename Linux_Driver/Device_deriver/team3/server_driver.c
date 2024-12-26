/*
 * NAME : PODDUTURI MANASA REDDY
 * KM_ID : KM54BESD03
 * DATE :
 * FILE_NAME :
 * SAMPLE_INPUT :
 * SAMPLE_OUTPUT :
 *
 * */



#include <linux/init.h>       // For module initialization and cleanup macros
#include <linux/module.h>     // For module macros and functions
#include <linux/fs.h>         // For file operations structure
#include <linux/cdev.h>       // For character device structure
#include <linux/gpio.h>       // For GPIO control
#include <linux/uaccess.h>    // For copy_from_user function
#include <linux/device.h>     // For device creation

#define GPIO_LED 532          // GPIO pin number for the LED
#define DEVICE_NAME "gpio_driver"  // Name of the device
#define CLASS_NAME "gpio_driver_class" // Name of the device class

static dev_t dev = 0;         // Device number (major and minor)
static struct class *dev_class;  // Pointer to device class
static struct cdev gpio_cdev;    // Character device structure
static char command[4];          // Buffer to store "on"/"off" commands

// Prototypes for file operations
static int gpio_device_open(struct inode *inode, struct file *file);
static int gpio_device_close(struct inode *inode, struct file *file);
static ssize_t gpio_device_write(struct file *file, const char __user *buf, size_t len, loff_t *offset);

// File operations structure to link to driver functions
static struct file_operations fops = {
    .open = gpio_device_open,      // Function to handle device open
    .release = gpio_device_close,  // Function to handle device close
    .write = gpio_device_write,    // Function to handle writing to device
};

// Device open function
static int gpio_device_open(struct inode *inode, struct file *file)
{
    pr_info("GPIO Driver Device opened\n"); // Log when the device is opened
    return 0; // Success
}

// Device close function
static int gpio_device_close(struct inode *inode, struct file *file)
{
    pr_info("GPIO Driver Device closed\n"); // Log when the device is closed
    return 0; // Success
}

// Device write function
static ssize_t gpio_device_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
    if (copy_from_user(command, buf, len)) { // Copy data from user space to kernel space
        return -EFAULT; // Return error if copy fails
    }

    // Control the LED based on the received command
    if (strncmp(command, "on", 2) == 0) { // If command is "on"
        gpio_set_value(GPIO_LED, 1);      // Turn the LED on
        pr_info("LED turned ON\n");
    } else if (strncmp(command, "off", 3) == 0) { // If command is "off"
        gpio_set_value(GPIO_LED, 0);      // Turn the LED off
        pr_info("LED turned OFF\n");
    } else {
        pr_info("Invalid command: %s\n", command); // Log invalid command
    }

    return len; // Return the number of bytes written
}

// Module initialization function
static int __init gpio_driver_init(void)
{
    // Allocate a device number
    if (alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) < 0) {
        pr_err("Cannot allocate major number\n");
        return -1; // Return error if allocation fails
    }

    pr_info("Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev)); // Log device number

    // Initialize and add the character device
    cdev_init(&gpio_cdev, &fops); // Initialize cdev structure
    if (cdev_add(&gpio_cdev, dev, 1) < 0) { // Add the device
        pr_err("Cannot add the device to the system\n");
        unregister_chrdev_region(dev, 1); // Clean up on error
        return -1;
    }

    // Create device class and device
    if (IS_ERR(dev_class = class_create(THIS_MODULE, CLASS_NAME)) ||
        IS_ERR(device_create(dev_class, NULL, dev, NULL, DEVICE_NAME))) {
        pr_err("Cannot create device\n");
        cdev_del(&gpio_cdev);            // Remove cdev
        unregister_chrdev_region(dev, 1); // Unregister device number
        return -1; // Return error
    }

    // Request the GPIO pin for the LED
    if (gpio_request(GPIO_LED, "gpio_led") < 0) { // Request GPIO pin
        pr_err("Failed to request GPIO for LED\n");
        device_destroy(dev_class, dev); // Destroy device
        class_destroy(dev_class);      // Destroy class
        cdev_del(&gpio_cdev);          // Remove cdev
        unregister_chrdev_region(dev, 1); // Unregister device number
        return -1; // Return error
    }

    gpio_direction_output(GPIO_LED, 0); // Set GPIO as output and turn LED off initially
    pr_info("GPIO Driver Initialized\n"); // Log success
    return 0; // Success
}

// Module exit function
static void __exit gpio_driver_exit(void)
{
    gpio_free(GPIO_LED);               // Free the GPIO pin
    device_destroy(dev_class, dev);    // Destroy the device
    class_destroy(dev_class);          // Destroy the device class
    cdev_del(&gpio_cdev);              // Remove the character device
    unregister_chrdev_region(dev, 1);  // Unregister the device number
    pr_info("GPIO Driver Removed\n");  // Log removal
}

// Macros to specify module initialization and exit functions
module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");                 // License type
MODULE_AUTHOR("Sushma");               // Author of the module
MODULE_DESCRIPTION("GPIO Driver for LED Control"); // Description of the module
MODULE_VERSION("1.0");                 // Module version
