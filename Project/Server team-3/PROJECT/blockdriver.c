/*
 * NAME : PODDUTURI MANASA REDDY
 * KM_ID : KM54BESD03
 * DATE :
 * FILE_NAME :
 * SAMPLE_INPUT :
 * SAMPLE_OUTPUT :
 *
 * */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/vmalloc.h>

// Definitions for our block device
#define MY_SECTOR_SIZE 512       // Sector size in bytes
#define NUM_SECTORS 1024         // Total number of sectors (size = NUM_SECTORS * MY_SECTOR_SIZE)

static int major_number;         // Major number for the block device
static struct gendisk *gdisk;    // gendisk structure representing the disk device
static struct request_queue *queue; // Request queue for I/O operations
static char *device_data;        // Memory buffer simulating the block device's storage

// Function to handle I/O requests (read/write operations)
static blk_qc_t my_make_request(struct request_queue *q, struct bio *bio)
{
    struct bio_vec bvec;
    struct bvec_iter iter;
    char *buffer;
    sector_t sector = bio->bi_iter.bi_sector; // Starting sector
    unsigned long offset;

    pr_info("Block device: Received request at sector %llu\n", (unsigned long long)sector);

    // Calculate the offset in the device_data buffer
    offset = sector * MY_SECTOR_SIZE;

    // Iterate through each segment of the bio (I/O request)
    bio_for_each_segment(bvec, bio, iter) {
        buffer = kmap_atomic(bvec.bv_page) + bvec.bv_offset;

        if (bio_data_dir(bio) == READ) {
            // Simulate a read: copy data from device_data into the buffer
            memcpy(buffer, device_data + offset, bvec.bv_len);
        } else if (bio_data_dir(bio) == WRITE) {
            // Simulate a write: copy data from buffer into device_data
            memcpy(device_data + offset, buffer, bvec.bv_len);
        }

        kunmap_atomic(buffer);
        offset += bvec.bv_len; // Advance the offset for the next bio segment
    }

    // Mark the bio request as completed
    bio_endio(bio);
    return BLK_QC_T_NONE;
}

// Open operation for the block device (when the device is opened by a user)
static int device_open(struct block_device *bdev, fmode_t mode)
{
    pr_info("Block device opened\n");
    return 0;
}

// Release operation for the block device (when the device is closed by a user)
static void device_release(struct gendisk *disk, fmode_t mode)
{
    pr_info("Block device released\n");
}

// Block device operations structure, specifying the device's open and release functions
static struct block_device_operations my_block_ops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
};

// Module initialization function (called when the module is loaded)
static int __init block_dev_init(void)
{
    pr_info("Initializing block device driver\n");

    // Allocate memory for device data (simulating the storage of the device)
    device_data = vmalloc(NUM_SECTORS * MY_SECTOR_SIZE);
    if (!device_data) {
        pr_err("Failed to allocate memory for block device\n");
        return -ENOMEM;
    }
    memset(device_data, 0, NUM_SECTORS * MY_SECTOR_SIZE); // Initialize the memory to zero

    // Register the block device with a major number (0 means dynamically assigned)
    major_number = register_blkdev(0, "testblk");
    if (major_number < 0) {
        pr_err("Failed to register block device\n");
        vfree(device_data);
        return major_number;
    }

    // Allocate the gendisk structure (used to represent the block device)
    gdisk = alloc_disk(1); // Allocate for 1 minor number (just a single disk)
    if (!gdisk) {
        pr_err("Failed to allocate gendisk\n");
        unregister_blkdev(major_number, "testblk");
        vfree(device_data);
        return -ENOMEM;
    }

    // Allocate the request queue for I/O operations
    queue = blk_alloc_queue(GFP_KERNEL);
    if (!queue) {
        pr_err("Failed to allocate request queue\n");
        put_disk(gdisk);
        unregister_blkdev(major_number, "testblk");
        vfree(device_data);
        return -ENOMEM;
    }
    blk_queue_make_request(queue, my_make_request); // Set the function to handle I/O requests

    // Set up the gendisk structure with necessary properties
    gdisk->major = major_number;
    gdisk->first_minor = 0;
    gdisk->fops = &my_block_ops;
    gdisk->queue = queue;
    snprintf(gdisk->disk_name, 32, "testblk"); // Set the name of the block device
    set_capacity(gdisk, NUM_SECTORS); // Set the size of the device in sectors

    // Add the gendisk to the system
    add_disk(gdisk);

    pr_info("Block device registered: /dev/testblk\n");
    return 0;
}

// Module cleanup function (called when the module is unloaded)
static void __exit block_dev_exit(void)
{
    pr_info("Cleaning up block device driver\n");

    // Clean up the request queue
    if (queue)
        blk_cleanup_queue(queue);

    // Clean up the gendisk structure
    if (gdisk) {
        del_gendisk(gdisk);      // Remove the gendisk from the system
        put_disk(gdisk);         // Free the memory allocated for gendisk
    }

    // Unregister the block device and free allocated memory
    unregister_blkdev(major_number, "testblk");
    vfree(device_data);

    pr_info("Block device driver cleaned up\n");
}

module_init(block_dev_init);  // Register the initialization function
module_exit(block_dev_exit);  // Register the cleanup function

MODULE_LICENSE("GPL");  // Specify the module license
MODULE_AUTHOR("Your Name");  // Author of the module
MODULE_DESCRIPTION("Simple Block Device Driver Example with Read/Write");  // Description of the module

