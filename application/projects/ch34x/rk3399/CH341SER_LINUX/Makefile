KERN_DIR = /home/liujun/ftp_dir/SDK/lbd3399-lubuntu-sdk-v1/kernel

default:
	make -C $(KERN_DIR) M=`pwd` modules	
clean:
	rm -rf .tmp_versions Module.symvers *.mod.c *.o *.ko .*.cmd Module.markers modules.order
load:
	modprobe usbserial
	insmod ch34x.ko
unload:
	rmmod ch34x

obj-m	+= ch34x.o
