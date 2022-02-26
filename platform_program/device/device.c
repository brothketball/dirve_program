/*包含头文件*/
#include<linux/init.h>//包含宏定义的头文件
#include<linux/module.h>//包含初始化加载模块的头文件
#include<linux/platform_device.h>

void gpio_release(struct device *dev)
	{
		printk("gpio_release ok\n");
	}

struct resource gpio_res[] = {
	[0] = {
		.start = 0xfe200008,
		.end   = 0xfe20000b,
		.name  = "GPSEL2",
		.flags = IORESOURCE_MEM
	},
	[1] = {
		.start = 0xfe20001c,
		.end   = 0xfe20001f,
		.name  = "GPSET0",
		.flags = IORESOURCE_MEM
	},
	[2] = {
		.start = 0xfe200028,
		.end   = 0xfe20002b,
		.name  = "GPCLR0",
		.flags = IORESOURCE_MEM
	}
};

struct platform_device gpio_device = {
	.name = "gpio_test",
	.id = -1,
	.num_resources = ARRAY_SIZE(gpio_res),
	.resource = gpio_res,
	.dev = {
		.release = gpio_release
	}
};

/*功能的实现*/
static int device_init(void)
{
	printk("Hello world!\r\n");
	return platform_device_register(&gpio_device);
}

static void device_exit(void)
{
	printk("Bye!\r\n");
	platform_device_unregister(&gpio_device);
}

/*驱动模块的入口和出口*/
module_init(device_init);
module_exit(device_exit);

/*声明信息*/
MODULE_LICENSE("GPL");
