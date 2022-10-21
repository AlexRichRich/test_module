obj-m += parser_module.o
obj-m += procfs_boofer.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	cp boof_script.sh /tmp/boof_script.sh
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf /tmp/boof_script.sh
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
	sudo insmod procfs_boofer.ko
	cat /sys/kernel/read_dev_module/read_dev
	echo "123" > /sys/kernel/read_dev_module/read_dev
	cat /sys/kernel/read_dev_module/read_dev
	sudo echo "1" > /sys/kernel/read_dev_module/read_dev
	cat /sys/kernel/read_dev_module/read_dev
	cat /proc/read_dev_boof
	sudo rmmod parser_module.ko
	sudo rmmod procfs_boofer.ko
	dmesg
