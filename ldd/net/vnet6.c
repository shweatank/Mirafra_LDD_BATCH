#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>/* this header has functions to acess the acess the network structures */
#include <linux/etherdevice.h>/* this header having ethernet  acess functions */
#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/timer.h>

/* formatting the pr_debug  to PDEBUG */
#define PDEBUG(fmt, args...) printk(KERN_DEBUG "snull: " fmt, ##args)
/*  structure  packet for pionting the data */
struct snull_packet {
    char *data;
    int datalen;
    struct snull_packet *next;
};

/* Structure to store device-specific data */
struct snull_priv {
    struct net_device_stats stats; /* stats: Tracks packet statistics (transmit/receive)*/
    struct sk_buff *skb;
    int status;
    spinlock_t lock; /* For synchronization */
    struct timer_list timer;/* in built timer code */
};

/* Simulate hardware transmission */
static void snull_hw_tx(const char *data, int len, struct net_device *dev) {
    /* Simulated: Pass data to the hardware (or a fake implementation) */
    PDEBUG("Simulating hardware transmission of %d bytes\n", len);
}

/* Packet Reception */
static void snull_rx(struct net_device *dev, struct snull_packet *pkt)  {
    struct sk_buff *skb;/* assiging the variable to structure */
    struct snull_priv *priv = netdev_priv(dev);
    
    /* allocating the socket buffer size for storing the frames */
    skb = dev_alloc_skb(pkt->datalen + 2);
    if (!skb) {
        if (printk_ratelimit())
            printk(KERN_NOTICE "snull rx: low on memory - packet dropped\n");
        priv->stats.rx_dropped++;
        return;
    }

    /*  void myMemCpy(void *dest, void *src, size_t n) 
     *  { 
     *  // Typecast src and dest addresses to (char *) 
     *  char *csrc = (char *)src; 
     *  char *cdest = (char *)dest; 
     *  for (int i=0; i<n; i++) 
     *  cdest[i] = csrc[i]; 
     *  }*/ 
    memcpy(skb_put(skb, pkt->datalen), pkt->data, pkt->datalen);

    skb->dev = dev;
    skb->protocol = eth_type_trans(skb, dev);
    skb->ip_summed = CHECKSUM_UNNECESSARY;

    priv->stats.rx_packets++;
    priv->stats.rx_bytes += pkt->datalen;

    netif_rx(skb);
}

/* Packet Transmission */
static int snull_tx(struct sk_buff *skb, struct net_device *dev) {
    struct snull_priv *priv = netdev_priv(dev);
    int len;
    char *data, shortpkt[ETH_ZLEN];

    data = skb->data;
    len = skb->len;

    if (len < ETH_ZLEN) {
        memset(shortpkt, 0, ETH_ZLEN);
        memcpy(shortpkt, skb->data, skb->len);
        len = ETH_ZLEN;
        data = shortpkt;
    }

    //dev->trans_start = jiffies;

    priv->skb = skb;

    /* Simulate the hardware transmission (specific to your device) */
    snull_hw_tx(data, len, dev);

    /* Free the skb after transmission */
    dev_kfree_skb(skb);

    /* Update statistics */
    priv->stats.tx_packets++;
    priv->stats.tx_bytes += len;

    return NETDEV_TX_OK;
}

/* Transmission Timeout */
static void snull_tx_timeout(struct net_device *dev, unsigned int txqueue){
    struct snull_priv *priv = netdev_priv(dev);

    PDEBUG("Transmit timeout at %lu, latency", jiffies);

    priv->stats.tx_errors++;
    netif_wake_queue(dev);
}

/* Get Network Statistics */
static struct net_device_stats *snull_get_stats(struct net_device *dev) {
    struct snull_priv *priv = netdev_priv(dev);
    return &priv->stats;
}

/* Open the device */
static int snull_open(struct net_device *dev) {
    netif_start_queue(dev);
    return 0;
}

/* Stop the device */
static int snull_stop(struct net_device *dev) {
    netif_stop_queue(dev);
    return 0;
}

/* Net Device Operations */
static const struct net_device_ops snull_netdev_ops = {
    .ndo_open = snull_open,
    .ndo_stop = snull_stop,
    .ndo_start_xmit = snull_tx,
    .ndo_tx_timeout = snull_tx_timeout,
    .ndo_get_stats = snull_get_stats,
};

/* Setup Network Device */
static void snull_setup(struct net_device *dev) {
    struct snull_priv *priv;
    
    /* it extract which type of protocol from the device */
    ether_setup(dev);

    dev->netdev_ops = &snull_netdev_ops;
    dev->watchdog_timeo = 5 * HZ;

    priv = netdev_priv(dev);
    memset(priv, 0, sizeof(struct snull_priv));
    spin_lock_init(&priv->lock);
}

/* Module Initialization */
static int __init snull_init(void) {
    struct net_device *dev;
    int result;

    dev = alloc_netdev(sizeof(struct snull_priv), "snull%d", NET_NAME_UNKNOWN, snull_setup);
    if (!dev) {
        printk(KERN_ERR "snull: Failed to allocate net device\n");
        return -ENOMEM;
    }

    result = register_netdev(dev);
    if (result) {
        printk(KERN_ERR "snull: Failed to register net device\n");
        free_netdev(dev);
        return result;
    }

    printk(KERN_INFO "snull: Network device registered successfully\n");
    return 0;
}

/* Module Cleanup */
static void __exit snull_exit(void) {
    struct net_device *dev = dev_get_by_name(&init_net, "snull0");

    if (dev) {
        unregister_netdev(dev);
        free_netdev(dev);
        printk(KERN_INFO "snull: Network device unregistered\n");
    }
}

module_init(snull_init);
module_exit(snull_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("Simple Network Device Driver Example Without Interrupts");

