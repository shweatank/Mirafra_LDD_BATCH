#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/types.h>  // For uint32_t

#define DEVICE_NAME "uart_dma"
#define UART_BASE 0x3F201000  // UART base address for Raspberry Pi 4
#define UART_DR_OFFSET 0x00  // Data register offset
#define UART_DMACR_OFFSET 0x48 // DMA control register offset
#define DMA_BASE 0x3F007000  // DMA base address for Raspberry Pi 4
#define DMA_CS_OFFSET 0x00   // Control and status offset
#define DMA_CONBLK_AD_OFFSET 0x04 // Control block address offset
#define DMA_DEBUG_OFFSET 0x20 // Debug register offset

// DMA Control Block
struct dma_cb {
    uint32_t ti;           // Transfer information
    uint32_t src_addr;     // Source address
    uint32_t dest_addr;    // Destination address
    uint32_t length;       // Transfer length
    uint32_t stride;       // Stride (set to 0)
    uint32_t next_cb;      // Next control block (set to 0 for no chaining)
    uint32_t padding[2];   // Padding to make it 32 bytes
};

static void __iomem *uart_regs;
static void __iomem *dma_regs;
static struct dma_cb *dma_cb;
static dma_addr_t dma_cb_handle, dma_buffer_handle;
static char *dma_buffer;
static int major;

// Interrupt handler for DMA completion
static irqreturn_t dma_uart_irq_handler(int irq, void *dev_id) {
    uint32_t dma_status = ioread32(dma_regs + DMA_CS_OFFSET);

    printk(KERN_INFO "DMA IRQ triggered, status: 0x%x\n", dma_status);

    if (dma_status & (1 << 8)) { // Check if transfer error
        printk(KERN_ERR "DMA transfer error detected\n");
    } else if (dma_status & (1 << 1)) { // Check if transfer completed
        printk(KERN_INFO "DMA transfer completed successfully\n");
    }

    // Clear interrupt flags
    iowrite32(0x1, dma_regs + DMA_CS_OFFSET);
    return IRQ_HANDLED;
}

// Device open
static int uart_dma_open(struct inode *inode, struct file *file) {
    return 0;
}

// Device release
static int uart_dma_release(struct inode *inode, struct file *file) {
    return 0;
}

// Device write (trigger DMA transfer)
static ssize_t uart_dma_write(struct file *file, const char __user *buf, size_t count, loff_t *pos) {
    if (count > PAGE_SIZE)
        return -EINVAL;

    // Copy data from user space to DMA buffer
    if (copy_from_user(dma_buffer, buf, count)) {
        printk(KERN_ERR "Failed to copy data from user space\n");
        return -EFAULT;
    }

    // Configure DMA control block
    dma_cb->ti = 0x30240; // Transfer info: SRC_INC, DEST_DREQ
    dma_cb->src_addr = dma_buffer_handle;
    dma_cb->dest_addr = UART_BASE + UART_DR_OFFSET;
    dma_cb->length = count;
    dma_cb->stride = 0;
    dma_cb->next_cb = 0;

    // Start DMA transfer
    iowrite32(dma_cb_handle, dma_regs + DMA_CONBLK_AD_OFFSET);
    iowrite32(0x1, dma_regs + DMA_CS_OFFSET); // Start DMA

    printk(KERN_INFO "DMA transfer started for %zu bytes\n", count);
    return count;
}

static const struct file_operations uart_dma_fops = {
    .owner = THIS_MODULE,
    .open = uart_dma_open,
    .release = uart_dma_release,
    .write = uart_dma_write,
};

// Module initialization
static int __init uart_dma_init(void) {
    int irq = 53;  //IRQ number for UART DMA on Raspberry Pi 4

    printk(KERN_INFO "Initializing UART-DMA driver\n");

    // Map UART and DMA registers
    uart_regs = ioremap(UART_BASE, 0x100);
    dma_regs = ioremap(DMA_BASE, 0x1000);

    if (!uart_regs || !dma_regs) {
        printk(KERN_ERR "Failed to map UART or DMA registers\n");
        return -ENOMEM;
    }

    // Enable UART DMA
    iowrite32(0x1, uart_regs + UART_DMACR_OFFSET);
    printk(KERN_INFO "UART DMA enabled\n");

    // Allocate DMA buffer and control block
    dma_buffer = dma_alloc_coherent(NULL, PAGE_SIZE, &dma_buffer_handle, GFP_KERNEL);
    dma_cb = dma_alloc_coherent(NULL, sizeof(struct dma_cb), &dma_cb_handle, GFP_KERNEL);

    if (!dma_buffer || !dma_cb) {
        printk(KERN_ERR "Failed to allocate DMA buffer or control block\n");
        return -ENOMEM;
    }

    // Register character device
    major = register_chrdev(0, DEVICE_NAME, &uart_dma_fops);
    if (major < 0) {
        printk(KERN_ERR "Failed to register character device\n");
        return major;
    }

    // Request IRQ
    if (request_irq(irq, dma_uart_irq_handler, IRQF_SHARED, DEVICE_NAME, &major)) {
        printk(KERN_ERR "Failed to request IRQ\n");
        unregister_chrdev(major, DEVICE_NAME);
        return -EIO;
    }

    printk(KERN_INFO "UART-DMA driver loaded successfully on Raspberry Pi 4\n");
    return 0;
}

// Module cleanup
static void __exit uart_dma_exit(void) {
    free_irq(53, &major); // Release IRQ
    unregister_chrdev(major, DEVICE_NAME);
    dma_free_coherent(NULL, PAGE_SIZE, dma_buffer, dma_buffer_handle);
    dma_free_coherent(NULL, sizeof(struct dma_cb), dma_cb, dma_cb_handle);
    iounmap(uart_regs);
    iounmap(dma_regs);
    printk(KERN_INFO "UART-DMA driver unloaded\n");
}

module_init(uart_dma_init);
module_exit(uart_dma_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kanna");
MODULE_DESCRIPTION("Enhanced UART DMA Driver Example for Raspberry Pi 4 with Debugging");
