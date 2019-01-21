/* ENSC 351 Final Project Part B - getptr.c
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


char *kbuff; // Kernel Space buffer

static int open(struct inode* inode_pointer, struct file* file_pointer) {
  printk(KERN_INFO "Opened!");
  return 0;
}

static int release(struct inode* inode_pointer, struct file* file_pointer) {
  printk(KERN_INFO "Closed!");
  return 0;
}

static ssize_t read(struct file *file, char *data, size_t length, loff_t *offset_in_file){
	printk(KERN_INFO "Data requested from User Space...");
	printk(KERN_INFO "Transferring data from Kernel Space to User Space...");
	unsigned long res = copy_to_user(data, kbuff, 8);
	/* Fetch kernel buffer address to memaddress */
	char* memaddress = &kbuff;
	if (res) {
		printl(KERN_INFO, "Failed to transfer %ld bytes.\n", res);
		return -EFAULT;
	}
	else {
		printk(KERN_INFO "Kernel memory address: %s", memaddress);
		printk(KERN_INFO "Data transfer succeeded.");
		return 8;
	}
  
}

static ssize_t write(struct file *file, const char *data, size_t length, loff_t *offset_in_file){
  return 0;
}

static struct device* device_data;
static struct class* class_stuff;

static struct file_operations file_ops =
{
   .open = open,
   .release = release,
   .read = read,
   .write = write,
};

static int __init getptr(void) {


   /* Allocating memory for kernel buffer */
   kbuff = kmalloc(1024, GFP_KERNEL);
   printk(KERN_INFO "Allocating memory for kernel receive buffer...");
   /* Check kernel receive buffer allocation */
   if(!kbuff) {
	printk(KERN_INFO "Failed to allocate memory for kernel receive buffer!");
	return -ENOMEM;
}


   int major = register_chrdev(0, "Hello", &file_ops);   
   class_stuff = class_create(THIS_MODULE, "hello class");
   device_data = device_create(class_stuff, NULL, MKDEV(major, 0), NULL,
			       "getptrdev");
  
  printk(KERN_INFO "HI!\n");
  return 0;
}

static void __exit bye(void) {

  printk(KERN_INFO "BYE!\n");
}

module_init(getptr);
module_exit(bye);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("RL");
MODULE_DESCRIPTION("A sample driver that just says hello!");
