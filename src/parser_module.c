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

static struct kobject *mymodule; 
/* Переменная, которую нужно будет изменять. */ 
static int myvariable = 0;

static ssize_t myvariable_show(struct kobject *kobj, 
				struct kobj_attribute *attr, char *buf) { 
    return sprintf(buf, "%d\n", myvariable); 
}
static int umh_test( void ) {
	char *argv[] = { "/bin/bash", "-c", "/bin/sh /tmp/test.sh >> /proc/read_dev_boof", NULL };
	static char *envp[] = {
		"HOME=/",
		"TERM=linux",
		"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

	return call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC);
}

static ssize_t myvariable_store(struct kobject *kobj, 
				struct kobj_attribute *attr, char *buf, 
				size_t count) {
	sscanf(buf, "%du", &myvariable);
	if (myvariable == 1) {
		umh_test();
	}	
	return count; 
} 
 
static struct kobj_attribute myvariable_attribute = __MY_ATTR(myvariable, 0777, myvariable_show, (void *)myvariable_store); 

    static int __init mymodule_init(void) { 
	int error = 0; 

	pr_info("mymodule: initialised\n"); 

	mymodule = kobject_create_and_add("mymodule", kernel_kobj); 
	if (!mymodule) 
		return -ENOMEM; 

	error = sysfs_create_file(mymodule, &myvariable_attribute.attr); 
	if (error) { 
		pr_info("failed to create the myvariable file " 
		"in /sys/kernel/mymodule\n"); 
	} 		 
	return error; 
} 
 
static void __exit mymodule_exit(void) {
	pr_info("mymodule: Exit success\n"); 
	kobject_put(mymodule); 
} 
 
module_init(mymodule_init); 
module_exit(mymodule_exit); 
 
MODULE_LICENSE("GPL");
