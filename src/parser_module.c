#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Bolotov");
MODULE_DESCRIPTION("Hello, world module wersion");
MODULE_VERSION("0.01");

static char *kbuf;
static dev_t first;
static unsigned int dev_count = 1;
static int my_major = 700, my_minor = 0;
static struct cdev *my_cdev;

#define MYDEV_NAME "read_dev"
#define KBUF_SIZE size_t ((10) * PAGE_SIZE) 

static int read_dev_open(struct inode *inode, struct file *file) {
	pr_info("Opening device %s\n", MYDEV_NANE);
	// int nbytes = lbuf - copy_to_user(buf, kbuf + *ppos, lbuf);
	return 0;
}

static int read_dev_release(struct inode *inode, struct file *file) {
	pr_info("Releasing device %s\n", MYDEV_NAME);
	return 0;
}

static ssize_t read_dev_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos) {
	pr_info("Reading device %s\n", MYDEV_NAME);
	return 0;
}

static ssize_t read_dev_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos) {
	pr_info("Reading device %s\n", MYDEV_NAME);
	return 0;
}

static const struct file_operations read_dev_fops = {
	.owner = THIS_MOULE,
	.open = read_dev_open,
	.read = read_dev_read,
	.write = read_dev_write,
	.release = read_dev_release
};

static int __init read_dev_init(void) {
	pr_info("Hello, YOPTA\n");
	kbuf = kmalloc(KBUT_SIZE, GFP_KERNEL);
	first = MKDEV (my_major, my_minor);
	register_chrdev_region(first, count, MYDEV_NAME);
	my_cdev = cdev_alloc();
	cdev_init(my_cdev, &read_dev_fops);
	cdev_add(my_cdev, first, count);
	return 0;
}

static void __exit read_dev_exit(void) {
	pr_info("Goodbye, YOPTA!\n");
}
module_init(read_dev_init);
module_exit(read_dev_exit);
