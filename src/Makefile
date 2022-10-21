obj-m += parser_module.o
obj-m += procfs_boofer.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	sudo dmesg -C
	sudo insmod parser_module.ko
	sudo rmmod parser_module.ko
	sudo insmod procfs_boofer.ko
	sudo rmmod procfs_boofer.ko
	dmesg
q:
	sudo dmesg -C
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo insmod parser_module.ko
	#sudo insmod procfs_boofer.ko
	cat /sys/kernel/mymodule/myvariable
	sudo echo "123" > /sys/kernel/mymodule/myvariable
	cat /sys/kernel/mymodule/myvariable
	sudo echo "1" > /sys/kernel/mymodule/myvariable
	cat /sys/kernel/mymodule/myvariable
	#cat /proc/read_dev_boof
	sudo rmmod parser_module.ko
	dmesg
