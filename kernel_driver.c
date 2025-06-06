#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#define DEVICE_NAME "Test"
#define MESSAGE "Kernel says hello\n"

static dev_t dev_n;
static struct cdev my_cdev;
static char kernel_buf[]=MESSAGE;

static int open(struct inode *inode, struct file *file) {
	printk(KERN_INFO "Test: device opened\n");
	return 0;
}
static int close(struct inode *inode, struct file *file) {
	printk(KERN_INFO "Test: device closed\n");
	return 0;
}
static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
	int msg_len = strlen(kernel_buf);
	if (*offset >= msg_len) return 0;
	if (copy_to_user(buf, kernel_buf, msg_len)) {
		return -EFAULT;
	}
	*offset += msg_len;
	return msg_len;
}
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = open,
	.release = close,
	.read =my_read
};
static int __init my_driver_init(void) { 
	if (alloc_chrdev_region(&dev_n, 0, 1, DEVICE_NAME) < 0) {
		printk(KERN_ALERT "Test: failed to allocate device number\n");
		return -1;
	}
	cdev_init(&my_cdev, &fops);
	if (cdev_add(&my_cdev, dev_n, 1) == -1) {
		unregister_chrdev_region(dev_n, 1);
		printk(KERN_ALERT "Test: failed to add cdev\n");
		return -1;
	}
	printk(KERN_INFO "Test: unregistered device\n");
	return 0;
}
static void __exit my_driver_exit(void) {
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev_n, 1);
	printk(KERN_INFO "Test: unregistered device\n");
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mihiran C");
MODULE_DESCRIPTION("Simple Character Device Driver");
MODULE_VERSION("1.0");

module_init(my_driver_init);
module_exit(my_driver_exit);

