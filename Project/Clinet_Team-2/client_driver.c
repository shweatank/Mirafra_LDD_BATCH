/*
 * NAME : PODDUTURI MANASA REDDY
 * KM_ID : KM54BESD03
 * DATE :
 * FILE_NAME :
 * SAMPLE_INPUT :
 * SAMPLE_OUTPUT :
 *
 * */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define GPIO_BUTTON 535  // GPIO pin for the button
#define DEVICE_NAME "gpio_device"
#define CLASS_NAME "gpio_class"

static int irq_number;       // IRQ number for the button GPIO
static dev_t dev = 0;        // Device number
static struct class *dev_class;
static struct cdev gpio_cdev;

static int flag = 0;         // Flag to indicate interrupt (0 or 1)
static char flag_status[2];  // Buffer to store the flag as a string

// Prototypes
static irqreturn_t button_irq_handler(int irq, void *dev_id);
static int gpio_device_open(struct inode *inode, struct file *file);
static int gpio_device_close(struct inode *inode, struct file *file);
static ssize_t gpio_device_read(struct file *file, char __user *buf, size_t len, loff_t *offset);

// IRQ handler for button press
static irqreturn_t button_irq_handler(int irq, void *dev_id)
{
    flag = !flag;  // Set the flag when interrupt is triggered
    pr_info("Interrupt triggered: flag set to %d\n", flag);
    return IRQ_HANDLED;
}

// Device open function
static int gpio_device_open(struct inode *inode, struct file *file)
{
    pr_info("GPIO Device opened\n");
    return 0;
}

// Device close function
static int gpio_device_close(struct inode *inode, struct file *file)
{
    pr_info("GPIO Device closed\n");
    return 0;
}

// Device read function - Used to send the flag status (on or off command)
static ssize_t gpio_device_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    snprintf(flag_status, sizeof(flag_status), "%d", flag);  // Convert flag to string ("1" or "0")

    if (copy_to_user(buf, flag_status, sizeof(flag_status))) {
        return -EFAULT;
    }

    flag = 0;  // Reset the flag after reading
    return sizeof(flag_status);
}

// File operations structure
static struct file_operations fops = {
    .open = gpio_device_open,
    .release = gpio_device_close,
    .read = gpio_device_read,
};

// Module initialization function
static int __init gpio_driver_init(void)
{
    // Allocate device number
    if ((alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME)) < 0) {
        pr_err("Cannot allocate major number\n");
        return -1;
    }

    pr_info("Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev));

    // Create cdev structure
    cdev_init(&gpio_cdev, &fops);

    // Add character device to the system
    if ((cdev_add(&gpio_cdev, dev, 1)) < 0) {
        pr_err("Cannot add the device to the system\n");
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    // Create struct class
    if (IS_ERR(dev_class = class_create(CLASS_NAME))) {
        pr_err("Cannot create the struct class\n");
        cdev_del(&gpio_cdev);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    // Create device
    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, DEVICE_NAME))) {
        pr_err("Cannot create the Device\n");
        class_destroy(dev_class);
        cdev_del(&gpio_cdev);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    // Request GPIO for button
    if (gpio_request(GPIO_BUTTON, "gpio_button") < 0) {
        pr_err("Failed to request GPIO for button\n");
        device_destroy(dev_class, dev);
        class_destroy(dev_class);
        cdev_del(&gpio_cdev);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    // Configure button GPIO as input
    gpio_direction_input(GPIO_BUTTON);

    // Get IRQ number for the button GPIO
    irq_number = gpio_to_irq(GPIO_BUTTON);
    if (irq_number < 0) {
        pr_err("Cannot get IRQ number for GPIO %d\n", GPIO_BUTTON);
        gpio_free(GPIO_BUTTON);
        device_destroy(dev_class, dev);
        class_destroy(dev_class);
        cdev_del(&gpio_cdev);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    // Request IRQ for the button GPIO
    if (request_irq(irq_number, button_irq_handler, IRQF_TRIGGER_FALLING, "gpio_button_irq", NULL) < 0) {
        pr_err("Failed to request IRQ\n");
        gpio_free(GPIO_BUTTON);
        device_destroy(dev_class, dev);
        class_destroy(dev_class);
        cdev_del(&gpio_cdev);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    pr_info("GPIO Interrupt Driver Initialized\n");
    return 0;
}

// Module exit function
static void __exit gpio_driver_exit(void)
{
    free_irq(irq_number, NULL);
    gpio_free(GPIO_BUTTON);
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&gpio_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("GPIO Interrupt Driver Removed\n");
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Team2");
MODULE_DESCRIPTION("GPIO Interrupt Driver");
MODULE_VERSION("1.0");


