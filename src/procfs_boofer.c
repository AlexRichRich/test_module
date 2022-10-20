#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/kmod.h>
#include <linux/init.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROCFS_MAX_SIZE 2048
#define PROCFS_NAME "read_dev_boof"

static struct proc_dir_entry *read_dev_file;
static char procfs_buffer[PROCFS_MAX_SIZE];
static unsigned long procfs_buffer_size = 0;

static ssize_t procfile_read(struct file *filePointer, char __user *buffer,
			size_t buffer_length, loff_t *offset) {
	static int finished = 0;
	if (finished) {
		pr_info("procfile_read: END\n");
		finished = 0;
		return 0;
	}
	finished = 1;
	if (copy_to_user(buffer, procfs_buffer, procfs_buffer_size))
		return -EFAULT;
	pr_info("procfile_read: read %lu bytes\n", procfs_buffer_size);

	return procfs_buffer_size;
}

static int umh_test( void ) {
	char *argv[] = { "/bin/bash", "-c", "bin/echo 123 >> proc/read_dev_boof", NULL };
	static char *envp[] = {
		"HOME=/",
		"TERM=linux",
		"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

	return call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC);
}

static ssize_t procfile_write(struct file *file, const char __user *buffer, size_t len, loff_t *off) {
	if (len > PROCFS_MAX_SIZE)
		procfs_buffer_size = PROCFS_MAX_SIZE;
	else
		procfs_buffer_size = len;
	if (copy_from_user(procfs_buffer, buffer, procfs_buffer_size)) {
		return -EFAULT;
	}

	pr_info("procfile_write: write %lu bytes\n", procfs_buffer_size);
	return procfs_buffer_size;
}

static int procfile_open(struct inode *inode, struct file *file) {
	try_module_get(THIS_MODULE);
	return 0;
}

static int procfile_close(struct inode *inode, struct file *file) {
	module_put(THIS_MODULE);
	pr_info("EXIT CODE OF TEST - %d", umh_test());
	return 0;
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_file_fops = {
	.proc_read = procfile_read,
	.proc_write = procfile_write,
	.proc_open = procfile_open,
	.proc_release = procfile_close,
};
#else
static const struct file_operations proc_file_fops = {
	.read = procfile_read,
	.write = procfile_write,
	.open = procfile_open,
	.release = procfile_close,
};
#endif

static int __init proc_file_init(void) {
	read_dev_file = proc_create(PROCFS_NAME, 0666, NULL, &proc_file_fops);
	if(read_dev_file == NULL) {
		remove_proc_entry(PROCFS_NAME, NULL);
		pr_alert("Error:Could not initialize /proc/%s\n", PROCFS_NAME);
		return -ENOMEM;
	}
	proc_set_size(read_dev_file, 80);
	proc_set_user(read_dev_file, GLOBAL_ROOT_UID, GLOBAL_ROOT_GID);
	pr_info("/proc/%s created", PROCFS_NAME);
	return 0;
}

static void __exit proc_file_exit(void) {
	remove_proc_entry(PROCFS_NAME, NULL);
	pr_info("/proc/%s removed\n", PROCFS_NAME);
}
module_init(proc_file_init);
module_exit(proc_file_exit);

MODULE_LICENSE("GPL");
