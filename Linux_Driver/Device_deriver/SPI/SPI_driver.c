/*
 * NAME : PODDUTURI MANASA REDDY
 * KM_ID : KM54BESD03
 * DATE :
 * FILE_NAME :
 * SAMPLE_INPUT :
 * SAMPLE_OUTPUT :
 *
 * */

#include <linux/module.h>        // Core header for Linux kernel modules
#include <linux/spi/spi.h>       // SPI driver interface header
#include <linux/kernel.h>        // Kernel macros and types
#include <linux/init.h>          // Module init and exit macros
#include <linux/slab.h>          // Kernel memory allocation
#include <linux/debugfs.h>       // DebugFS for kernel debugging
#include <linux/uaccess.h>       // User-space access functions

#define DRIVER_NAME "rpi4b_spi_driver"
#define SPI_BUS 0                 // SPI bus number (adjust as per your setup)
#define SPI_CS 0                  // Chip Select number (adjust as per your setup)
#define SPI_MAX_SPEED 500000      // SPI max speed in Hz (500 kHz)

// Structure to hold SPI device-related data
struct rpi4b_spi_dev {
    struct spi_device *spi;       // SPI device handle
    struct dentry *debugfs_dir;   // DebugFS directory pointer
    u8 debug_tx_data[2];          // Debug data for SPI TX
    u8 debug_rx_data[2];          // Debug data for SPI RX
};

/**
 * debugfs_read_tx_data - Read TX data via DebugFS.
 * @data: Pointer to SPI device data.
 * @val: Pointer to store the read value.
 *
 * Returns: 0 on success.
 */
static int debugfs_read_tx_data(void *data, u64 *val)
{
    struct rpi4b_spi_dev *dev = data;
    *val = dev->debug_tx_data[1];
    return 0;
}

/**
 * debugfs_write_tx_data - Write TX data via DebugFS.
 * @data: Pointer to SPI device data.
 * @val: Value to be written to the TX buffer.
 *
 * Updates the TX data and logs it via kernel message.
 * Returns: 0 on success.
 */
static int debugfs_write_tx_data(void *data, u64 val)
{
    struct rpi4b_spi_dev *dev = data;
    dev->debug_tx_data[1] = (u8)val;
    pr_info("DebugFS: TX data updated to 0x%02x\n", dev->debug_tx_data[1]);
    return 0;
}

// Define simple file operations for DebugFS to read/write TX data
DEFINE_SIMPLE_ATTRIBUTE(debugfs_tx_data_fops, debugfs_read_tx_data, debugfs_write_tx_data, "%llu\n");

/**
 * rpi4b_spi_probe - Probe function for SPI device.
 * @spi: Pointer to the SPI device structure.
 *
 * This function initializes the SPI device, allocates memory,
 * creates DebugFS entries, and performs a test SPI transfer.
 *
 * Returns: 0 on success, negative error code otherwise.
 */
static int rpi4b_spi_probe(struct spi_device *spi)
{
    struct rpi4b_spi_dev *dev;
    int ret;
    struct spi_transfer t;
    struct spi_message m;

    dev_info(&spi->dev, "Probing SPI device\n");

    // Allocate memory for device-specific data
    dev = devm_kzalloc(&spi->dev, sizeof(struct rpi4b_spi_dev), GFP_KERNEL);
    if (!dev) {
        dev_err(&spi->dev, "Failed to allocate memory\n");
        return -ENOMEM;
    }

    // Associate device-specific data with SPI device
    spi_set_drvdata(spi, dev);
    dev->spi = spi;

    // Initialize debug data
    dev->debug_tx_data[0] = 0x00;
    dev->debug_tx_data[1] = 0xFF;

    // Create DebugFS directory and file for TX data
    dev->debugfs_dir = debugfs_create_dir(DRIVER_NAME, NULL);
    if (!dev->debugfs_dir) {
        dev_err(&spi->dev, "Failed to create debugfs directory\n");
        return -ENOMEM;
    }
    debugfs_create_file("tx_data", 0600, dev->debugfs_dir, dev, &debugfs_tx_data_fops);

    // Prepare an example SPI transfer
    u8 tx_buf[2] = { dev->debug_tx_data[0], dev->debug_tx_data[1] };
    u8 rx_buf[2] = { 0x00 };
    t.tx_buf = tx_buf;
    t.rx_buf = rx_buf;
    t.len = 2;                  // Transfer length
    t.speed_hz = SPI_MAX_SPEED; // Set SPI speed
    t.bits_per_word = 8;

    // Initialize SPI message and add transfer
    spi_message_init(&m);
    spi_message_add_tail(&t, &m);

    // Execute the SPI transfer synchronously
    ret = spi_sync(spi, &m);
    if (ret) {
        dev_err(&spi->dev, "SPI transfer failed: %d\n", ret);
        return ret;
    }

    // Log RX data received during SPI transfer
    dev_info(&spi->dev, "SPI transfer successful. RX Data: 0x%02x 0x%02x\n", rx_buf[0], rx_buf[1]);
    dev->debug_rx_data[0] = rx_buf[0];
    dev->debug_rx_data[1] = rx_buf[1];

    return 0;
}

/**
 * rpi4b_spi_remove - Remove function for SPI device.
 * @spi: Pointer to the SPI device structure.
 *
 * Cleans up DebugFS entries and frees resources associated with the device.
 *
 * Returns: 0 on success.
 */
static int rpi4b_spi_remove(struct spi_device *spi)
{
    struct rpi4b_spi_dev *dev = spi_get_drvdata(spi);

    // Remove DebugFS directory and entries
    debugfs_remove_recursive(dev->debugfs_dir);
    dev_info(&spi->dev, "Removing SPI device\n");

    return 0;
}

/* SPI Device ID Table */
static const struct spi_device_id rpi4b_spi_id[] = {
    { "rpi4b_spi_device", 0 },   // SPI device name
    { }
};
MODULE_DEVICE_TABLE(spi, rpi4b_spi_id);

/* SPI Driver Structure */
static struct spi_driver rpi4b_spi_driver = {
    .driver = {
        .name = DRIVER_NAME,     // Name of the driver
        .owner = THIS_MODULE,    // Module owner
    },
    .probe = rpi4b_spi_probe,    // Probe function
    .remove = rpi4b_spi_remove,  // Remove function
    .id_table = rpi4b_spi_id,    // Supported SPI devices
};

/**
 * rpi4b_spi_init - Module initialization function.
 *
 * Registers the SPI driver with the SPI subsystem.
 *
 * Returns: 0 on success, negative error code otherwise.
 */
static int __init rpi4b_spi_init(void)
{
    pr_info("Initializing %s\n", DRIVER_NAME);
    return spi_register_driver(&rpi4b_spi_driver);
}

/**
 * rpi4b_spi_exit - Module exit function.
 *
 * Unregisters the SPI driver and cleans up resources.
 */
static void __exit rpi4b_spi_exit(void)
{
    pr_info("Exiting %s\n", DRIVER_NAME);
    spi_unregister_driver(&rpi4b_spi_driver);
}

// Register module initialization and exit functions
module_init(rpi4b_spi_init);
module_exit(rpi4b_spi_exit);

MODULE_AUTHOR("techdhaba");
MODULE_DESCRIPTION("SPI Device Driver for Raspberry Pi 4B with DebugFS");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
