#include <linux/module.h>      // For module programming
#include <linux/kernel.h>      // For kernel functions like pr_info
#include <linux/fs.h>          // For file system operations
#include <linux/uaccess.h>     // For copy_to_user and copy_from_user functions
#include <linux/cdev.h>        // For character device operations
#include <linux/device.h>      // For device and class creation

#define DEVICE_NAME "ioctl_device"  // Name of the device
#define CLASS_NAME "ioctl_class"    // Name of the device class

// IOCTL Commands: Defines two commands for IOCTL
#define IOCTL_GET_DATA _IOR('i', 1, int)   // Command to get data from the driver
#define IOCTL_SET_DATA _IOW('i', 2, int)   // Command to set data to the driver

static int device_open = 0;      // Flag to keep track of whether the device is already open
static int ioctl_data = 0;       // Variable to store data that can be manipulated via IOCTL

// Function to handle the open operation of the device
static int dev_open(struct inode *inodep, struct file *filep)
{
    if (device_open)  // Check if the device is already opened
        return -EBUSY; // Return error if the device is busy (already opened)

    device_open++; // Increment device open count
    pr_info("Device opened\n");  // Print log indicating the device is opened
    return 0;  // Return success
}

// Function to handle the close operation of the device
static int dev_release(struct inode *inodep, struct file *filep)
{
    device_open--;  // Decrement device open count when closed
    pr_info("Device closed\n");  // Print log indicating the device is closed
    return 0;  // Return success
}

// Function to handle IOCTL commands
static long dev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    int temp;  // Temporary variable to store data

    // Handle different IOCTL commands
    switch (cmd) {
    case IOCTL_GET_DATA:
        // Copy data from kernel space to user space
        if (copy_to_user((int __user *)arg, &ioctl_data, sizeof(ioctl_data))) {
            return -EFAULT;  // Return error if copying fails
        }
        pr_info("IOCTL_GET_DATA: Data sent to user: %d\n", ioctl_data);  // Print log for data sent
        break;
        
    case IOCTL_SET_DATA:
        // Copy data from user space to kernel space
        if (copy_from_user(&temp, (int __user *)arg, sizeof(temp))) {
            return -EFAULT;  // Return error if copying fails
        }
        ioctl_data = temp;  // Set the value of ioctl_data
        pr_info("IOCTL_SET_DATA: Data received from user: %d\n", ioctl_data);  // Print log for data received
        break;
        
    default:
        return -EINVAL;  // Return error for unknown IOCTL commands
    }

    return 0;  // Return success
}

// Structure to define the file operations for the device
static struct file_operations fops = {
    .owner = THIS_MODULE,              // Owner of the file operations
    .open = dev_open,                  // Open function
    .release = dev_release,            // Release (close) function
    .unlocked_ioctl = dev_ioctl,       // IOCTL function
};

// Major number for device registration
static int major_number;
// Device class and device structure
static struct class *ioctl_class;
static struct device *ioctl_device;

// Initialization function for the driver
static int __init ioctl_driver_init(void)
{
    // Register the character device with the kernel
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        pr_err("Failed to register a major number\n");  // Log error if registration fails
        return major_number;  // Return error code
    }
    pr_info("Registered with major number %d\n", major_number);  // Log success

    // Create a device class
    ioctl_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(ioctl_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);  // Unregister if class creation fails
        pr_err("Failed to register device class\n");
        return PTR_ERR(ioctl_class);  // Return error code
    }
    pr_info("Device class registered\n");  // Log success

    // Create a device associated with the class
    ioctl_device = device_create(ioctl_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(ioctl_device)) {
        class_destroy(ioctl_class);  // Destroy class if device creation fails
        unregister_chrdev(major_number, DEVICE_NAME);  // Unregister the device
        pr_err("Failed to create the device\n");
        return PTR_ERR(ioctl_device);  // Return error code
    }
    pr_info("Device created successfully\n");  // Log success
    return 0;  // Return success
}

// Exit function to clean up resources when the driver is unloaded
static void __exit ioctl_driver_exit(void)
{
    // Destroy the device and class, and unregister the device
    device_destroy(ioctl_class, MKDEV(major_number, 0));
    class_unregister(ioctl_class);
    class_destroy(ioctl_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    pr_info("IOCTL driver unloaded\n");  // Log message when the driver is unloaded
}

// Register the initialization and exit functions
module_init(ioctl_driver_init);
module_exit(ioctl_driver_exit);

// Module information
MODULE_LICENSE("GPL");                // License type
MODULE_AUTHOR("Sujitha");           // Author information
MODULE_DESCRIPTION("A simple IOCTL Linux driver");  // Description of the driver
MODULE_VERSION("1.0");                // Module version

