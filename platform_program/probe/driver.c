#include<linux/init.h>//包含宏定义的头文件
#include<linux/module.h>//包含初始化加载模块的头文件
#include<linux/platform_device.h>
#include<linux/mod_devicetable.h>

struct resource *gpio_mem;

int gpio_probe(struct platform_device *pdev)
{
	printk("gpio_probe\n");

	gpio_mem = platform_get_resource(pdev,IORESOURCE_MEM,0);

	if(gpio_mem==NULL)
	{
		printk("platform_get_resource is error\n");
		return -EBUSY;
	}

	printk("platform_get_resource is ok\n");

	printk("gpio_res start is 0x%llx\n",gpio_mem->start);
	printk("gpio_res end is 0x%llx\n",gpio_mem->end);
	
	return 0;
}

int gpio_remove(struct platform_device *pdev)
{
	printk("gpio_remove\n");
	return 0;
}

const struct platform_device_id gpio_id_table = {
	.name = "gpio_test"
};

struct platform_driver gpio_device = {
	.probe = gpio_probe,
	.remove = gpio_remove,
	.driver = {
		.name = "gpio_test",
		.owner = THIS_MODULE
	},
	.id_table = &gpio_id_table
};


/*功能的实现*/
static int gpio_driver_init(void)
{
	int ret =0;

	ret  =platform_driver_register(&gpio_device);
	if(ret<0)
	{
		printk("platform_driver_register error\n");
		return ret;
	}

	printk("platform_driver_register ok\n");
	return 0;
}

static void gpio_driver_exit(void)
{
	printk("Bye!\r\n");
	platform_driver_unregister(&gpio_device);
}

/*驱动模块的入口和出口*/
module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

/*声明信息*/
MODULE_LICENSE("GPL");
