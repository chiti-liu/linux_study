KERNELDIR := /home/liujun/ftp_dir/SDK/lbd3399-lubuntu-sdk-v1/kernel
CURRENT_PATH := $(shell pwd)

obj-m := mpu6050.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
	date
	$(CC) mpu6050Demo.c -o mpu6050Demo -Wall -pthread -O2

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
	rm mpu6050Demo -rf
