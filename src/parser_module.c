#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Bolotov");
MODULE_DESCRIPTION("Hello, world module wersion");
MODULE_VERSION("0.01");
static int __init lkm_example_init(void) {
	pr_info("Hello, YOPTA\n");
	return 0;
}
static void __exit lkm_example_exit(void) {
	pr_info("Goodbye, YOPTA!\n");
}
module_init(lkm_example_init);
module_exit(lkm_example_exit);
