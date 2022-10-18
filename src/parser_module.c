/*
Тестовое задание:
Создать модуль, который создает кольцевой буфер в procfs и файл read_dev в sysfs. При записи 1 в read_dev, в кольцевой буфер должно записываться все существующие устройства из /sys/bus/platform в формате: [время] имя_устройства, ресурсы_устройства (в виде диапозонов), device_node, (если поля нет, то указывать NULL). Причем read_dev можно вызвать из другого модуля
 */

#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/poll.h>

static int read_dev_open(struct inode *, struct file *);
static int read_dev_release(struct inode *, struct file *);
static ssize_t read_dev_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t read_dev_write(struct file *, const char __user *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "read_dev" /* Имя устройства, как оно показано в /proc/devices   */
#define BUF_LEN 80 /* Максимальная длина сообщения устройства. */

/* Глобальные переменные объявляются как static, поэтому являются глобальными в пределах файла. */

static int major; /* Старший номер, присвоенный драйверу устройства */

enum {
	CDEV_NOT_USED = 0,
	CDEV_EXCLUSIVE_OPEN = 1,
};

static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);

static char msg[BUF_LEN]; /* msg, которое устройство будет выдавать при запросе. */

static struct class *cls;
 
static struct file_operations read_dev_fops = {
	.read = read_dev_read,
	.write = read_dev_write,
	.open = read_dev_open,
	.release = read_dev_release,
};

static int __init read_dev_init(void) {
	major = register_chrdev(0, DEVICE_NAME, &read_dev_fops);
	if (major < 0) {
		pr_alert("Registrating char device failed with %d\n", major);
		return major;
	}
	pr_info("Assigned major number %d\n", major);
	cls = class_create(THIS_MODULE, DEVICE_NAME);
	device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
	pr_info("Device created on /dev/%s\n", DEVICE_NAME);
	return SUCCESS;
}

static void __exit read_dev_exit(void) {
	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);
	unregister_chrdev(major, DEVICE_NAME);
}

static int read_dev_open(struct inode *inode, struct file *file) {
	static int counter = 0;
	if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN)) return -EBUSY;
	sprintf(msg, "I already told you %d times Hello world!\n", counter++);
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int read_dev_release(struct inode *inode, struct file *file) {
	atomic_set(&already_open, CDEV_NOT_USED);
	module_put(THIS_MODULE);
	return SUCCESS;
}

static ssize_t read_dev_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset) {
	int bytes_read = 0;
	const char *msg_ptr = msg;
	if (!*(msg_ptr + *offset)) {
		*offset = 0;
		return 0;
	}
	msg_ptr += *offset;
	while (length && *msg_ptr) {
		put_user(*(msg_ptr++), buffer++);
		length--;
		bytes_read++;
	}
	*offset += bytes_read;
	return bytes_read;
}

static ssize_t read_dev_write(struct file *filp, const char __user *buff, size_t len, loff_t *off) {
	pr_alert("Sorry, this operation is not supported.\n");
	return -EINVAL;
}

module_init(read_dev_init);
module_exit(read_dev_exit);

MODULE_LICENSE("GPL");

