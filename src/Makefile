obj-m += sysfs_file.o
obj-m += procfs_boofer.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	cp boof_script.sh /tmp/boof_script.sh
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf /tmp/boof_script.sh
test:
	sudo dmesg -C
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo insmod sysfs_file.ko
	sudo insmod procfs_boofer.ko
	cat /sys/kernel/read_dev_module/read_dev
	echo "123" > /sys/kernel/read_dev_module/read_dev
	cat /sys/kernel/read_dev_module/read_dev
	sudo echo "1" > /sys/kernel/read_dev_module/read_dev
	cat /sys/kernel/read_dev_module/read_dev
	cat /proc/read_dev_boof
	sudo rmmod sysfs_file.ko
	sudo rmmod procfs_boofer.ko
	dmesg
