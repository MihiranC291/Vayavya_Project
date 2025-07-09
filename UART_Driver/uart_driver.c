//Including necessary headers
# include <linux/module.h>   
# include <linux/init.h>
# include <linux/fs.h>       //For file operations
# include <linux/uaccess.h>  //For safe access to user space
# include <linux/cdev.h>     //For character device structure
# include <linux/io.h>       //For memory mapped input and output
# include <linux/spinlock.h> //For concurrency protection
# include <linux/device.h>   //For sysfs device interface
# include <linux/ioctl.h>    //For control interface

//Defining macros for later use
//Notably device and class names, UART base address, and ioctl command macros
#define DEVICE_NAME "uart_driver"
#define CLASS_NAME "uart_class"
#define UART_BASE 0xFE201000             //UART0 base address on RPi
#define UART_SIZE 0X48                   //Size of PL011 UART memory region
#define IOCTL_M 'u'                      //Unique namespace for ioctl
#define UART_IOCTL_RESET _IO(IOCTL_M, 0) //No data transfer just read, write

//Global variables
static void __iomem *uart_base;             //UART memory pointer
static int major;                           //Device major number
static struct class *uart_class;
static struct cdev uart_cdev;
static struct device *uart_device;          //Actual device created 
static spinlock_t uart_lock;                //Spinlock
static char last_msg[100] = "No messages."; //Required for troubleshooting with logs

//File Operations
//Read
static ssize_t uart_read(struct file *file, char __user *buf, size_t len, loff_t *off) {
	char ch;
	unsigned long flags;
	printk(KERN_ALERT "uart_read(): Attempting to acquire spinlock by PID: %d\n", current->pid);
	spin_lock_irqsave(&uart_lock, flags);
	printk(KERN_ALERT "uart_read(): Acquired spinlock by PID: %d\n", current->pid);
	while(!(readl(uart_base + 0x18) & 0x10))    //Offsets in the UART flag register to inform the system to wait as bit 4 isn't 0 i.e. there is data present
		cpu_relax();
	ch = readl(uart_base + 0x00) & 0xFF;       //Reading byte 
	spin_unlock_irqrestore(&uart_lock, flags); //Releasing the lock
	printk(KERN_ALERT "uart_read(): Released spinlock by PID: %d\n", current->pid);
	if (copy_to_user(buf, &ch, 1))
		return -EFAULT;
	snprintf(last_msg, sizeof(last_msg), "Read char: %c", ch);
	return 1;
}
	
//Write
static ssize_t uart_write(struct file *file, const char __user *buf, size_t len, loff_t *off) {
	char ch;
	unsigned long flags;
	if (copy_from_user(&ch, buf, 1))
		return -EFAULT;
	printk(KERN_ALERT "uart_write(): Attempting to acquire spinlock by PID: %d\n", current->pid);
	spin_lock_irqsave(&uart_lock, flags);
	printk(KERN_ALERT "uart_write(): Acquired spinlock by PID: %d\n", current->pid);
	while (readl(uart_base + 0x18) & 0x20) //UART flag register bit 5 to set spinlock
		cpu_relax();
	writel(ch, uart_base + 0x00);
	spin_unlock_irqrestore(&uart_lock, flags);
	printk(KERN_ALERT "uart_write(): Attempting to acquire spinlock by PID: %d\n", current->pid);
	snprintf(last_msg, sizeof(last_msg), "Wrote char: %c", ch);
	return 1;
}

//To handle IOCTL calls 
static long uart_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
	switch (cmd) {
		case UART_IOCTL_RESET:
			writel(0, uart_base + 0x30);                                  //Since 0x30 is the offset of the control register CR as per the BCM processor datasheet
			writel(0x301, uart_base + 0x30);                              //0x301 enables UART, TX, and RX which is 0b1100000001 and bits 0, 8 and 9 determine these features
			snprintf(last_msg, sizeof(last_msg), "UART reset performed.");
			break;
		default:
			return -EINVAL;
	}
	return 0;
}

//Open
static int uart_open(struct inode *inode, struct file *file) {
	return 0;
}

//Release
static int uart_release(struct inode *inode, struct file *file) {
	return 0;
}

//File Ops structure
static struct file_operations uart_fops = {
	.owner = THIS_MODULE,
	.read = uart_read,
	.write = uart_write,
	.unlocked_ioctl = uart_ioctl,
	.open = uart_open,
	.release = uart_release,
};

//Sysfs log
//Defines how we display the content of the sys log
static ssize_t log_show(struct device *dev, struct device_attribute *attr, char *buf) {
	return snprintf(buf, PAGE_SIZE, "%s\n", last_msg);
}
static DEVICE_ATTR_RO(log); //Generates a struct device attribute named log with .show() access

//Module init
static int __init uart_init(void) {
	dev_t dev;
	if (alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) < 0)                  //Allocates the character device major number
		return -1;
	major = MAJOR(dev);                                                    //Registers the character device
	cdev_init(&uart_cdev, &uart_fops);
	if (cdev_add(&uart_cdev, dev, 1) < 0)
		return -1;
	uart_class = class_create(CLASS_NAME);                    //Creates the device node in the dev directory
	if (IS_ERR(uart_class))
		return PTR_ERR(uart_class);      
	uart_device = device_create(uart_class, NULL, dev, NULL, DEVICE_NAME); //Create the log file 
	if (IS_ERR(uart_device)) 
		return PTR_ERR(uart_device);               
	if (device_create_file(uart_device, &dev_attr_log)) {
		printk(KERN_ERR "Failed to create sysfs log attribute.\n");
		return -1;
	}
	uart_base = ioremap(UART_BASE, UART_SIZE);                             //Maps UART registers
	if (!uart_base)
		return -ENOMEM;
	spin_lock_init(&uart_lock);
	writel(0, uart_base + 0x30);                                           //Disable UART
       	writel(0x301, uart_base + 0x30);                                       //Enable UART, TX and RX
       	printk(KERN_INFO "UART driver with sysfs and spinlock loaded.\n");
       	return 0;
}

//Module Exit
//Unmaps memory, destroys sysfs log, and frees resources after completion
static void __exit uart_exit(void) {
	device_remove_file(uart_device, &dev_attr_log);
	device_destroy(uart_class, MKDEV(major, 0));
	class_destroy(uart_class);
	cdev_del(&uart_cdev);
	unregister_chrdev_region(MKDEV(major, 0), 1);
	iounmap(uart_base);
	printk(KERN_INFO "UART driver unloaded.\n");
}

//Key module calls
module_init(uart_init);
module_exit(uart_exit);

//Metadata
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mihiran Chakraborty");
MODULE_DESCRIPTION("UART driver with ioctl, sysfs, and spinlock.");
