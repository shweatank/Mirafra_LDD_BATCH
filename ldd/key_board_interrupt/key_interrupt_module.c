#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include "socket_module.h"

#define KEYBOARD_IRQ 1
#define ENTER_KEY_CODE 0x1C

// Interrupt handler for keyboard events
static irqreturn_t irq_handler(int irq, void *dev_id) {
    char message[128];

    snprintf(message, sizeof(message), "Keyboard interrupt received: IRQ %d\n", irq);
    pr_info("%s", message);

    // Send data to the socket module
    if (socket_send_data(message) < 0) {
        pr_err("Failed to send interrupt data to socket\n");
    }

    return IRQ_HANDLED;
}

static int __init key_interrupt_module_init(void) {
    static int dev_id; // Static variable for a unique identifier
    int ret;

    pr_info("Initializing key interrupt module\n");

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
    free_irq(KEYBOARD_IRQ, &dev_id);
    pr_info("Key interrupt module unloaded\n");
}

module_init(key_interrupt_module_init);
module_exit(key_interrupt_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("Key Interrupt Module");

