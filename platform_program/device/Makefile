ifneq ($(KERNELRELEASE),)
	obj-m:=device.o
else
	KDIR?=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.mod *.ko *.symvers *.order *.makers *.mod.c
endif
