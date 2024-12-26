#include <linux/module.h>   // For all kernel modules
#include <linux/kernel.h>   // For kernel logging and macros
#include <linux/init.h>     // For module initialization and cleanup macros
#include <linux/interrupt.h> // For interrupt handling
#include <linux/timer.h>    // For using kernel timers
#include "socket_module.h" // For sending data to the socket module

// Define constants for the keyboard interrupt and ENTER key code
#define KEYBOARD_IRQ 1      // IRQ line number for the keyboard
#define ENTER_KEY_CODE 0x1C // Scan code for the ENTER key

// Global variables
static bool interrupt_handled = false; // Flag to track if the interrupt has been handled
static struct timer_list reset_timer;  // Timer to reset the interrupt-handled flag

// Timer callback function to reset the interrupt-handled flag
static void reset_flag(struct timer_list *t) {
    interrupt_handled = false; // Reset the flag
    pr_info("Interrupt flag reset\n"); // Log the reset action
}

// Interrupt handler function for the keyboard interrupt
static irqreturn_t irq_handler(int irq, void *dev_id) {
    char message[128]; // Buffer to store the message

    // Check if the interrupt has already been handled
    if (interrupt_handled) {
        return IRQ_HANDLED; // Exit if already handled
    }

    interrupt_handled = true; // Mark the interrupt as handled

    // Format the interrupt message
    snprintf(message, sizeof(message), "Interrupt received, perform GPIO interrupt in Raspberry Pi (IRQ: %d)\n", irq);
    pr_info("%s", message); // Log the message

    // Send the interrupt data to the socket module
    if (socket_send_data(message) < 0) {
        pr_err("Failed to send interrupt data to socket\n"); // Log error if sending fails
    }

    // Start or reset the timer to clear the interrupt-handled flag after 1 second
    mod_timer(&reset_timer, jiffies + HZ); // HZ represents one second in kernel time units

    return IRQ_HANDLED; // Indicate that the interrupt was successfully handled
}

// Module initialization function
static int __init key_interrupt_module_init(void) {
    static int dev_id; // Static variable to serve as a unique identifier for the device
    int ret;           // Variable to store the return value of functions

    pr_info("Initializing key interrupt module\n"); // Log module initialization

    // Initialize the reset timer with the reset_flag callback function
    timer_setup(&reset_timer, reset_flag, 0);

    // Register the keyboard interrupt handler
    ret = request_irq(KEYBOARD_IRQ, irq_handler, IRQF_SHARED, "key_interrupt_module", &dev_id);
    if (ret < 0) { // Check if IRQ registration failed
        pr_err("Failed to request IRQ: %d\n", ret); // Log the error
        return ret; // Return the error code
    }

    pr_info("Key interrupt module loaded successfully\n"); // Log successful initialization
    return 0; // Return success
}

// Module cleanup function
static void __exit key_interrupt_module_exit(void) {
    static int dev_id; // Static variable to identify the device

    // Free the IRQ to release the interrupt line
    free_irq(KEYBOARD_IRQ, &dev_id);

    // Delete the timer to stop any pending actions
    del_timer_sync(&reset_timer);

    pr_info("Key interrupt module unloaded\n"); // Log module cleanup
}

// Register the initialization and cleanup functions
module_init(key_interrupt_module_init);
module_exit(key_interrupt_module_exit);

// Module metadata
MODULE_LICENSE("GPL");
MODULE_AUTHOR("GOPI");
MODULE_DESCRIPTION("Key Interrupt Module");
