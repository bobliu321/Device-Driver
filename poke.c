/* ENSC 351 Final Project Part B - poke.c
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
char *kbuff; // Kernel space buffer
char *peek_address; // 8 byte memory location
size_t factorEight = 8; // Factor of 8 byte tracking
size_t start = 0; // Index to user space buffer



static int open_poke(struct inode* inode_pointer, struct file* file_pointer) {
  printk(KERN_INFO "Opened!");
  return 0;
}

static int close_poke(struct inode* inode_pointer, struct file* file_pointer) {
  printk(KERN_INFO "Closed!");
  return 0;
}

static ssize_t read_poke(struct file *file, char *data, size_t length, loff_t *offset_in_file){
  return 0;
}

static ssize_t write_poke(struct file *file, const char *data, size_t length, loff_t *offset_in_file){

    /* Reset addressIndex */
    int addressIndex = 0;
    printk(KERNEL_INFO "Data incoming to Kernel Space...");
    copy_from_user(kbuff, data, length);

    /* Check if User Space buffer has enough data */
    if (length - start > 8){
        for (int ii = start; ii < length; ++ii) {
            /* Storing 8 bytes into peek_address as memory location */
            if (ii < factorEight){
                peek_address[addressIndex] = kbuff[ii];
                addressIndex++;
            }
        }
        /* Update offset */
        start = start + 9;

        /* Writes the 9th byte to peek_address */
        *peek_address = kbuff[start-1];

        /* Update factor of 8 */
        factorEight = factorEight + 8;
        return 0;

    } else {
        printk(KERN_INFO "Not enough data!");
        return 0;
    }


}

static struct device* device_data;
static struct class* class_stuff;

static struct file_operations file_ops =
{
   .open = open_sesame,
   .release = bye_sesame,
   .read = pillage_sesame,
   .write = restock_sesame,
};

static int __init poke(void) {

   int major = register_chrdev(0, "Hello", &file_ops);   
   class_stuff = class_create(THIS_MODULE, "hello class");
   device_data = device_create(class_stuff, NULL, MKDEV(major, 0), NULL,
			       "pokedev");
  
  printk(KERN_INFO "HI!\n");
  return 0;
}

static void __exit bye(void) {
  printk(KERN_INFO "BYE!\n");
}

module_init(poke);
module_exit(bye);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("RL");
MODULE_DESCRIPTION("A sample driver that just says hello!");
