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

// Definitions for our block device
#define MY_SECTOR_SIZE 512       // Custom sector size in bytes
#define NUM_SECTORS 1024         // Total number of sectors

static int major_number;		//it stores the major num
static struct gendisk *gdisk;	//Points to a gendisk structure representing the disk device.
static struct request_queue *queue;		 //Represents the request queue for I/O operations.
static char *device_data;		//Pointer to a memory buffer simulating the device's storage.

// Function to handle I/O requests
static blk_qc_t my_make_request(struct request_queue *q, struct bio *bio)
{
    pr_info("Block device: Received request\n");

    // Complete the request successfully
    bio_endio(bio); // For kernel 5.4, only one argument is accepted		// Signals the kernel that the bio has been processed successfully.
    return BLK_QC_T_NONE;		//Indicates that no specific hardware queuing context is used.

}

// Open operation for the block device
static int device_open(struct block_device *bdev, fmode_t mode)		//The mode in which the device is opened (read/write).
{
    pr_info("Block device opened\n");
    return 0;
}

// Release operation for the block device
static void device_release(struct gendisk *disk, fmode_t mode)
{
    pr_info("Block device released\n");
}

// Block device operations structure
static struct block_device_operations my_block_ops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
};

// Module initialization function
static int __init block_dev_init(void)
{
    pr_info("Initializing block device driver\n");

    // Allocate memory for device data
    device_data = vmalloc(NUM_SECTORS * MY_SECTOR_SIZE);
    if (!device_data) {
        pr_err("Failed to allocate memory for block device\n");
        return -ENOMEM;
    }

    // Register the block device and get a major number
    major_number = register_blkdev(0, "testblk");
    if (major_number < 0) {
        pr_err("Failed to register block device\n");
        vfree(device_data);
        return major_number;
    }

    // Allocate gendisk structure
    gdisk = alloc_disk(1); // Allocate one minor number
    if (!gdisk) {
        pr_err("Failed to allocate gendisk\n");
        unregister_blkdev(major_number, "testblk");
        vfree(device_data);
        return -ENOMEM;
    }

    // Initialize the queue
    queue = blk_alloc_queue(GFP_KERNEL);
    if (!queue) {
        pr_err("Failed to allocate request queue\n");
        put_disk(gdisk);
        unregister_blkdev(major_number, "testblk");
        vfree(device_data);
        return -ENOMEM;
    }
    blk_queue_make_request(queue, my_make_request);

    // Set up the gendisk structure
    gdisk->major = major_number;		//Assign the major number to the gendisk structure.
    gdisk->first_minor = 0;				//Set the first minor number for the disk.
    gdisk->fops = &my_block_ops;		//Assign the block device operations to the gendisk. and	The fops field in gendisk tells the kernel which functions to call for these operations.
    gdisk->queue = queue;				//Associate the request queue with the disk.
    snprintf(gdisk->disk_name, 32, "testblk");
    set_capacity(gdisk, NUM_SECTORS); // Set the device size in sectors

    // Add the disk to the system
    add_disk(gdisk);

    pr_info("Block device registered: /dev/testblk\n");
    return 0;
}

// Module cleanup function
static void __exit block_dev_exit(void)
{
    pr_info("Cleaning up block device driver\n");

    if (queue)
        blk_cleanup_queue(queue); // Free the request queue

    if (gdisk) {
        del_gendisk(gdisk);      // Remove the gendisk from the system
        put_disk(gdisk);         // Free the gendisk structure
    }

    unregister_blkdev(major_number, "testblk"); // Unregister the block device
    vfree(device_data); // Free the device data

    pr_info("Block device driver cleaned up\n");
}

module_init(block_dev_init);
module_exit(block_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple Block Device Driver Example");

