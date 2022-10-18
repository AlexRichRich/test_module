#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROCFS_NAME "read_dev_boof"

static struct proc_dir_entry *read_dev_file;

static ssize_t procfile_read(struct file *filePointer, char __user *buffer,
			size_t buffer_length, loff_t *offset) {
	char s[13] = "HelloWorld!\n";
	int len = sizeof(s);
	ssize_t ret = len;

	if (*offset >= len || copy_to_user(buffer, s, len)) {
		pr_info("copy_to_user failed\n");
		ret = 0;
	} else {
		pr_info("procfile read %s\n", filePointer->f_path.dentry->d_name.name);
		*offset += len;
	}

	return ret;
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_file_fops = {
	.proc_read = procfile_read,
};
#else
static const struct file_operations proc_file_fops = {
	.read = procfile_read,
};
#endif

static int __init proc_file_init(void) {
	read_dev_file = proc_create(PROCFS_NAME, 0644, NULL, &proc_file_fops);
	if(read_dev_file == NULL) {
		proc_remove(read_dev_file);
		pr_alert("Error:Could not initialize /proc/%s\n", PROCFS_NAME);
		return -ENOMEM;
	}

	pr_info("/proc/%s created", PROCFS_NAME);
	return 0;
}

static void __exit proc_file_exit(void) {
	proc_remove(read_dev_file);
	pr_info("/proc/%s removed\n", PROCFS_NAME);
}
module_init(proc_file_init);
module_exit(proc_file_exit);

MODULE_LICENSE("GPL");
