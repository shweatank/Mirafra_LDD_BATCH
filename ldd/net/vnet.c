#include <linux/module.h>   /* the module contain macros that intializes the init and exit functions  and also definition and function that need to intialized*/
#include <linux/netdevice.h>
/* header file in the Linux kernel provides definitions, structures, and functions for network device drivers. It is a fundamental part of the Linux networking subsystem and is used to manage network interfaces, implement drivers, and interact with network devices */
#include <linux/etherdevice.h>
/*  header file is a part of the Linux kernel source and provides helper functions, macros, and utilities for working with Ethernet network devices in kernel space  */
#include <linux/skbuff.h>

/* Custom net_device_ops function declarations   */
static int mynet_open(struct net_device *dev);
static int mynet_stop(struct net_device *dev);
static netdev_tx_t mynet_start_xmit(struct sk_buff *skb, struct net_device *dev);

/* Define net_device_ops structure   */
static const struct net_device_ops mynet_netdev_ops = {
    .ndo_open = mynet_open,
    .ndo_stop = mynet_stop,
    .ndo_start_xmit = mynet_start_xmit,
};

/* Open function  */
static int mynet_open(struct net_device *dev) {
    netif_start_queue(dev); /* Start the transmit queue  */
    pr_info("device has be opened you can send the data \n");
    return 0;
}

/* Stop function    */
static int mynet_stop(struct net_device *dev) {
    netif_stop_queue(dev); // Stop the transmit queue
    pr_info("device has been closed\n");
    return 0;
}

/* Transmit function   */
static netdev_tx_t mynet_start_xmit(struct sk_buff *skb, struct net_device *dev) {
    pr_info("transmitting the packet \n");
    dev_kfree_skb(skb); // Free the socket buffer
    return NETDEV_TX_OK;
    /*NETDEV_TX_OK: Indicates the packet was successfully queued for transmission.
NETDEV_TX_BUSY: Indicates the transmission queue is full or unable to process the packet*/
}

// Init function
static void mynet_setup(struct net_device *dev) {
    ether_setup(dev); // Initialize Ethernet settings
    dev->netdev_ops = &mynet_netdev_ops; // Assign net_device_ops
    dev->flags |= IFF_NOARP; // Disable ARP
    dev->features |= NETIF_F_HW_CSUM; // Enable checksumming
}

// Module init
static int __init mynet_init(void) {
    struct net_device *dev;

    // Allocate and register net_device
    dev = alloc_netdev(0, "mynet%d", NET_NAME_UNKNOWN, mynet_setup);
    if (register_netdev(dev)) {
        pr_info("mynet: Failed to register device\n");
        free_netdev(dev);
        return -ENODEV;
    }
    pr_info(" Module loaded  %p\n",dev);
    return 0;
}

// Module exit
static void __exit mynet_exit(void) {
    struct net_device *dev = first_net_device(&init_net);
    unregister_netdev(dev); // Unregister the device
    free_netdev(dev); // Free memory
    pr_info("Module unloaded\n");
}

module_init(mynet_init);
module_exit(mynet_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("Custom Network Driver");

