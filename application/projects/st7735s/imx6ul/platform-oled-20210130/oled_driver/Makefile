
KERNELDIR := /home/dazen/linux/ubootlinux/linux-imx6ull-4.1.15

CURRENT_PATH := $(shell pwd)

ARCH := arm
CROSS_COMPILE := arm-linux-gnueabihf-

obj-m := oled_device.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
