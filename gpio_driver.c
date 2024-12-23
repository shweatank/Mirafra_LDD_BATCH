#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define GPIO_LED 532  // GPIO pin for the LED
#define DEVICE_NAME "gpio_driver"
#define CLASS_NAME "gpio_driver_class"

static dev_t dev = 0;           // Device number
static struct class *dev_class;
static struct cdev gpio_cdev;
static char command[4];         // Buffer for "on"/"off" command

// Prototypes
static int gpio_device_open(struct inode *inode, struct file *file);
static int gpio_device_close(struct inode *inode, struct file *file);
static ssize_t gpio_device_write(struct file *file, const char __user *buf, size_t len, loff_t *offset);

// File operations structure
static struct file_operations fops = {
    .open = gpio_device_open,
    .release = gpio_device_close,
    .write = gpio_device_write,
};

// Device open function
static int gpio_device_open(struct inode *inode, struct file *file)
{
    pr_info("GPIO Driver Device opened\n");
    return 0;
}

// Device close function
static int gpio_device_close(struct inode *inode, struct file *file)
{
    pr_info("GPIO Driver Device closed\n");
    return 0;
}

// Device write function
static ssize_t gpio_device_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
    if (copy_from_user(command, buf, len)) {
        return -EFAULT;
    }

    // Control the LED based on the received command
    if (strncmp(command, "on", 2) == 0) {
        gpio_set_value(GPIO_LED, 1);  // Turn LED on
        pr_info("LED turned ON\n");
    } else if (strncmp(command, "off", 3) == 0) {
        gpio_set_value(GPIO_LED, 0);  // Turn LED off
        pr_info("LED turned OFF\n");
    } else {
        pr_info("Invalid command: %s\n", command);
    }

    return len;
}

// Module initialization
static int __init gpio_driver_init(void)
{
    // Allocate device number
    if (alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) < 0) {
        pr_err("Cannot allocate major number\n");
        return -1;
    }

    pr_info("Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev));

    // Create cdev structure and add device
    cdev_init(&gpio_cdev, &fops);
    if (cdev_add(&gpio_cdev, dev, 1) < 0) {
        pr_err("Cannot add the device to the system\n");
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    // Create device class and device
    if (IS_ERR(dev_class = class_create(CLASS_NAME)) ||
        IS_ERR(device_create(dev_class, NULL, dev, NULL, DEVICE_NAME))) {
        pr_err("Cannot create device\n");
        cdev_del(&gpio_cdev);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    // Request GPIO for LED
    if (gpio_request(GPIO_LED, "gpio_led") < 0) {
        pr_err("Failed to request GPIO for LED\n");
        device_destroy(dev_class, dev);
        class_destroy(dev_class);
        cdev_del(&gpio_cdev);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    gpio_direction_output(GPIO_LED, 0);  // Initialize LED as off
    pr_info("GPIO Driver Initialized\n");
    return 0;
}

// Module exit
static void __exit gpio_driver_exit(void)
{
    gpio_free(GPIO_LED);
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&gpio_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("GPIO Driver Removed\n");

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sushma");
MODULE_DESCRIPTION("GPIO Driver for LED Control");
//MODULE_VERSION("1.0");

