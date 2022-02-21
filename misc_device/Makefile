ifneq ($(KERNELRELEASE),)
	obj-m:=misc.o
else
	KDIR?=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.mod *.ko *.symvvers *.order *.makers
endif
