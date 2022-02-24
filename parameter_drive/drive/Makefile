ifneq ($(KERNELRELEASE),)
	obj-m:=parameter.o
else
	KDIR?=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm *.o *.ko *.mod *.mod.c *.order *.symvers
endif
