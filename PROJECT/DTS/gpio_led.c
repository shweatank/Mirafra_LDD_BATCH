#include <linux/module.h>      // For module macros and initialization
#include <linux/kernel.h>      // For kernel-related functions and macros
#include <linux/init.h>        // For module initialization and cleanup macros
#include <linux/gpio.h>        // For GPIO handling
#include <linux/of.h>          // For Device Tree functions
#include <linux/of_gpio.h>     // For GPIO handling specific to Device Tree
#include <linux/delay.h>       // For delay functions like msleep()

// Static variable to hold the GPIO pin number for the LED
static int led_gpio = -1;      // Default value is -1, indicating uninitialized GPIO

// Function to initialize the GPIO LED module
static int __init gpio_led_init(void)
{
    struct device_node *node;  // Pointer to the device tree node
    int ret;                   // Variable to hold return values for error checking

    // Retrieve the device tree node named "gpio_led"
    node = of_find_node_by_name(NULL, "gpio_led");
    if (!node) {
        // Print an error message if the node is not found
        pr_err("GPIO LED node not found in Device Tree\n");
        return -ENODEV;       // Return an error code for "No such device"
    }

    // Retrieve the GPIO pin number associated with the "gpios" property in the node
    led_gpio = of_get_named_gpio(node, "gpios", 0);
    if (led_gpio < 0) {
        // Print an error message if the GPIO retrieval fails
        pr_err("Failed to get GPIO from Device Tree\n");
        return led_gpio;      // Return the error code from of_get_named_gpio
    }

    // Print the GPIO number found for debugging purposes
    pr_info("LED GPIO found: %d\n", led_gpio);

    // Request ownership of the GPIO pin
    ret = gpio_request(led_gpio, "LED GPIO");
    if (ret) {
        // Print an error message if the GPIO request fails
        pr_err("Failed to request GPIO %d\n", led_gpio);
        return ret;
    }

    // Set the GPIO direction to output and initialize it to OFF (low state)
    ret = gpio_direction_output(led_gpio, 0); // 0 means LED is OFF
    if (ret) {
        // Print an error message if setting direction fails
        pr_err("Failed to set GPIO direction\n");
        gpio_free(led_gpio);  // Free the GPIO before returning
        return ret;
    }

    // Turn the LED ON by setting the GPIO high
    gpio_set_value(led_gpio, 1);
    msleep(1000);             // Wait for 1 second
    gpio_set_value(led_gpio, 0); // Turn the LED OFF by setting GPIO low

    return 0;                 // Return 0 for successful initialization
}

// Function to clean up the GPIO LED module
static void __exit gpio_led_exit(void)
{
    if (led_gpio >= 0) {         // Check if the GPIO was properly initialized
        gpio_set_value(led_gpio, 0); // Ensure the LED is OFF before cleanup
        gpio_free(led_gpio);     // Free the GPIO to release resources
    }

    // Print a message indicating the module is being removed
    pr_info("GPIO LED driver removed\n");
}

// Macros to define the module initialization and cleanup functions
module_init(gpio_led_init);
module_exit(gpio_led_exit);

// Module metadata
MODULE_LICENSE("GPL");              // Specify the license type (GPL in this case)
MODULE_AUTHOR("Your Name");         // Author of the module
MODULE_DESCRIPTION("A simple driver for controlling GPIO LED using Device Tree");

