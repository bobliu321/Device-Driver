/* ENSC 351 Final Project Part B - peek.c
 * Bob Liu - 301236133
 * Keith Leung - 301221899 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

/* Global variables */
char *kbuff; // Kernel Space buffer
char *peek_address; // 8 byte memory location
size_t factorEight = 8; // Factor of 8 byte tracking
size_t start = 0; // Index to User Space buffer
int count = 0; // Single byte index


static int open_peek(struct inode *inode_pointer, struct file *file_pointer) {
    printk(KERN_INFO
    "Opened!");
    return 0;
}

static int close_peek(struct inode *inode_pointer, struct file *file_pointer) {
    printk(KERN_INFO
    "Closed!");
    return 0;
}

static ssize_t read_peek(struct file *file, char *data, size_t length, loff_t *offset_in_file) {
    /* Retrieve a byte from peek_address */
    char copy_data = peek_address[count];
    copy_to_user(data, copy_data, 1);
    count++;

    /* Reset single byte index to 0 */
    if (count == 8) {
        count = 0;
    }
    return 0;
}

static ssize_t write_peek(struct file *file, const char *data, size_t length, loff_t *offset_in_file) {
    /* Reset addressIndex */
    int addressIndex = 0;
    printk(KERNEL_INFO "Data incoming to Kernel Space...");
    /* Full copy from User Space to Kernel Space */
    copy_from_user(kbuff, data, length);

    /* Check if User Space buffer has enough data */
    if (length - start > 7) {
        for (int ii = start; ii < length; ++ii) {
            if (ii < factorEight) {
                peek_address[addressIndex] = kbuff[ii];
                addressIndex++;
            }
        }
        /* Update offset */
        start = start + 7;
        /* Update factor of 8 */
        factorEight = factorEight + 8;
        return 0;

    } else {
        printk(KERN_INFO "Not enough data!");
        return 0;
    }

}

static struct device *device_data;
static struct class *class_stuff;

static struct file_operations file_ops =
        {
                .open = open_peek,
                .release = close_peek,
                .read = read_peek,
                .write = write_peek,
        };

static int __init peek(void) {

    int major = register_chrdev(0, "Hello", &file_ops);
    class_stuff = class_create(THIS_MODULE, "hello class");
    device_data = device_create(class_stuff, NULL, MKDEV(major, 0), NULL,
                                "peekdev");

    printk(KERN_INFO
    "HI!\n");
    return 0;
}

static void __exit bye(void) {
    printk(KERN_INFO
    "BYE!\n");
}

module_init(peek);
module_exit(bye);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("RL");
MODULE_DESCRIPTION("A sample driver that just says hello!");
