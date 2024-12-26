#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include "socket_module.h"

#define KEYBOARD_IRQ 1
#define ENTER_KEY_CODE 0x1C

static bool interrupt_handled = false; // Flag to track interrupt state
static struct timer_list reset_timer; // Timer to reset the flag

// Reset interrupt flag after a timeout
static void reset_flag(struct timer_list *t) {
    interrupt_handled = false;
    pr_info("Interrupt flag reset\n");
}

// Interrupt handler for keyboard events
static irqreturn_t irq_handler(int irq, void *dev_id) {
    char message[128];

    // Check if interrupt has already been handled
    if (interrupt_handled) {
        return IRQ_HANDLED;
    }

    interrupt_handled = true; // Mark interrupt as handled

    snprintf(message, sizeof(message), "Interrupt received, perform GPIO interrupt in Raspberry Pi (IRQ: %d)\n", irq);
    pr_info("%s", message);

    // Send data to the socket module
    if (socket_send_data(message) < 0) {
        pr_err("Failed to send interrupt data to socket\n");
    }

    // Start/reset the timer to clear the interrupt flag after 1 second
    mod_timer(&reset_timer, jiffies + HZ);

    return IRQ_HANDLED;
}

static int __init key_interrupt_module_init(void) {
    static int dev_id; // Static variable for a unique identifier
    int ret;

    pr_info("Initializing key interrupt module\n");

    // Initialize the reset timer
    timer_setup(&reset_timer, reset_flag, 0);

    // Register the keyboard interrupt
    ret = request_irq(KEYBOARD_IRQ, irq_handler, IRQF_SHARED, "key_interrupt_module", &dev_id);
    if (ret < 0) {
        pr_err("Failed to request IRQ: %d\n", ret);
        return ret;
    }

    pr_info("Key interrupt module loaded successfully\n");
    return 0;
}



static void __exit key_interrupt_module_exit(void) {
    static int dev_id;

    // Free the IRQ
    free_irq(KEYBOARD_IRQ, &dev_id);

    // Delete the timer
    del_timer_sync(&reset_timer);

    pr_info("Key interrupt module unloaded\n");
}

module_init(key_interrupt_module_init);
module_exit(key_interrupt_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("Key Interrupt Module");

