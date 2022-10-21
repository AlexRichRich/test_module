#include <linux/fs.h> 
#include <linux/init.h> 
#include <linux/kobject.h> 
#include <linux/module.h> 
#include <linux/string.h> 
#include <linux/sysfs.h> 

#define __MY_ATTR(_name, _mode, _show, _store) { \
		.attr = {.name = __stringify(_name), \
			.mode = _mode }, \
		.show	= _show, \
		.store	= _store, \
}

static struct kobject *read_dev_module; 
/* Переменная, которую нужно будет изменять. */ 
static int read_dev = 0;

static ssize_t read_dev_show(struct kobject *kobj, 
				struct kobj_attribute *attr, char *buf) { 
    return sprintf(buf, "%d\n", read_dev); 
}
static int boof_script_call( void ) {
	char *argv[] = { "/bin/bash", "-c", "/bin/sh /tmp/boof_script.sh >> /proc/read_dev_boof", NULL };
	static char *envp[] = {
		"HOME=/",
		"TERM=linux",
		"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

	return call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC);
}

static ssize_t read_dev_store(struct kobject *kobj, 
				struct kobj_attribute *attr, char *buf, 
				size_t count) {
	sscanf(buf, "%du", &read_dev);
	if (read_dev == 1) {
		boof_script_call();
	}	
	return count; 
} 
 
static struct kobj_attribute read_dev_attribute = __MY_ATTR(read_dev, 0777, read_dev_show, (void *)read_dev_store); 

    static int __init read_dev_module_init(void) { 
	int error = 0; 

	pr_info("read_dev_module: initialised\n"); 

	read_dev_module = kobject_create_and_add("read_dev_module", kernel_kobj); 
	if (!read_dev_module) 
		return -ENOMEM; 

	error = sysfs_create_file(read_dev_module, &read_dev_attribute.attr); 
	if (error) { 
		pr_info("failed to create the read_dev file " 
		"in /sys/kernel/read_dev_module\n"); 
	} 		 
	return error; 
} 
 
static void __exit read_dev_module_exit(void) {
	pr_info("read_dev_module: Exit success\n"); 
	kobject_put(read_dev_module); 
} 
 
module_init(read_dev_module_init); 
module_exit(read_dev_module_exit); 
 
MODULE_LICENSE("GPL");
