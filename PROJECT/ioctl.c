#include <linux/module.h>       // For module_init, module_exit, and module-related functions
#include <linux/kernel.h>       // For kernel information logging (pr_info, pr_err)
#include <linux/fs.h>           // For file operations structure and functions
#include <linux/uaccess.h>      // For copy_to_user and copy_from_user
#include <linux/cdev.h>         // For character device registration
#include <linux/device.h>       // For device and class creation

#define DEVICE_NAME "ioctl_device"  // Name of the device
#define CLASS_NAME "ioctl_class"    // Name of the device class

// IOCTL Commands
#define IOCTL_GET_DATA _IOR('i', 1, int)  // IOCTL command to get data
#define IOCTL_SET_DATA _IOW('i', 2, int)  // IOCTL command to set data

static int device_open = 0;    // Tracks whether the device is currently open
static int ioctl_data = 0;     // Placeholder to store data for IOCTL operations

// ** File operation: Open the device **
static int dev_open(struct inode *inodep, struct file *filep)
{
    if (device_open)           // If the device is already open, return busy error
        return -EBUSY;

    device_open++;             // Increment the open count
    pr_info("Device opened\n");// Log device open
    return 0;                  // Success
}

// ** File operation: Release/close the device **
static int dev_release(struct inode *inodep, struct file *filep)
{
    device_open--;             // Decrement the open count
    pr_info("Device closed\n");// Log device close
    return 0;                  // Success
}

// ** File operation: Handle IOCTL commands **
static long dev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    int temp;
    switch (cmd) 
    {
    case IOCTL_GET_DATA:       // Handle IOCTL_GET_DATA command
        // Copy data from kernel space to user space
        if (copy_to_user((int __user *)arg, &ioctl_data, sizeof(ioctl_data))) {
            return -EFAULT;    // Return fault if copy fails
        }
        pr_info("IOCTL_GET_DATA: Data sent to user: %d\n", ioctl_data);
        break;

    case IOCTL_SET_DATA:       // Handle IOCTL_SET_DATA command
        // Copy data from user space to kernel space
        if (copy_from_user(&temp, (int __user *)arg, sizeof(temp))) {
            return -EFAULT;    // Return fault if copy fails
        }
        ioctl_data = temp;     // Update the ioctl_data variable
        pr_info("IOCTL_SET_DATA: Data received from user: %d\n", ioctl_data);
        break;

    default:                   // Handle invalid commands
        return -EINVAL;        // Return invalid argument error
    }
    return 0;                  // Success
}

// File operations structure, linking the functions to their respective operations
static struct file_operations fops = {
    .owner = THIS_MODULE,      // Owner of this module
    .open = dev_open,          // Open function
    .release = dev_release,    // Release function
    .unlocked_ioctl = dev_ioctl, // IOCTL function
};

static int major_number;       // Stores the major number of the device
static struct class *ioctl_class; // Pointer to device class
static struct device *ioctl_device; // Pointer to the created device

// ** Module initialization function **
static int __init ioctl_driver_init(void)
{
    // Register the character device and get a major number
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {    // Check if registration failed
        pr_err("Failed to register a major number\n");
        return major_number;  // Return the error code
    }
    pr_info("Registered with major number %d\n", major_number);

    // Create a device class
    ioctl_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(ioctl_class)) { // Check if class creation failed
        unregister_chrdev(major_number, DEVICE_NAME);
        pr_err("Failed to register device class\n");
        return PTR_ERR(ioctl_class); // Return the error code
    }
    pr_info("Device class registered\n");

    // Create the device in /dev/
    ioctl_device = device_create(ioctl_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(ioctl_device)) { // Check if device creation failed
        class_destroy(ioctl_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        pr_err("Failed to create the device\n");
        return PTR_ERR(ioctl_device); // Return the error code
    }
    pr_info("Device created successfully\n");
    return 0; // Initialization successful
}

// ** Module exit function **
static void __exit ioctl_driver_exit(void)
{
    device_destroy(ioctl_class, MKDEV(major_number, 0)); // Remove the device from /dev/
    class_unregister(ioctl_class);                      // Unregister the device class
    class_destroy(ioctl_class);                         // Destroy the device class
    unregister_chrdev(major_number, DEVICE_NAME);       // Unregister the character device
    pr_info("IOCTL driver unloaded\n");                 // Log driver unload
}

module_init(ioctl_driver_init); // Register the initialization function
module_exit(ioctl_driver_exit); // Register the exit function

MODULE_LICENSE("GPL");           // Specify the license (GPL)
MODULE_AUTHOR("ashritha");      // Author information
MODULE_DESCRIPTION("A simple IOCTL Linux driver"); // Short description of the module
MODULE_VERSION("1.0");           // Module version


