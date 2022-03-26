#include<linux/init.h>//包含宏定义的头文件
#include<linux/module.h>//包含初始化加载模块的头文件
#include<linux/platform_device.h>
#include<linux/mod_devicetable.h>
#include<linux/of.h>
#include<linux/of_address.h>
#include<linux/miscdevice.h>//这两个都是杂项设备必要的头文件
#include<linux/fs.h>
#include<linux/uaccess.h>

#include<linux/gpio.h>
#include<linux/of_gpio.h>
#include<linux/interrupt.h>

int size;
u32 out_value[2]={0};
const char *str;

struct device_node *test_deive_node;
struct property *test_node_property;

int gpio_num;
int irq;

irq_handler_t key_func_handler(int irq, void *args)
{
	printk("key_func_handler is ok!\n");
	return IRQ_HANDLED;
}

int gpio_probe(struct platform_device *pdev)
{
	int ret = 0;
	
	printk("gpio_probe\n");
	//直接获取节点名
	//printk("node name is %s\n",pdev->dev.of_node->name);
	
	//使用of函数获取
	/*获取节点*/
	//pdev->dev.of_node等同于test_deivce_node
	test_deive_node = of_find_node_by_path("/test_key");
	if(test_deive_node == NULL)
	{
		printk("of_find_node_by_path is error\n");
		return -1;
	}

	//获得gpio编号
	gpio_num = of_get_named_gpio(test_deive_node,"mykey",0);
	if(gpio_num < 0)
	{
		printk("of_get_named_gpio is error\n");
		return -1;
	}
	//设置gpio为输入
	gpio_direction_input(gpio_num);

	//获得中断编号
	irq = gpio_to_irq(gpio_num);
	printk("irq is %d\n",irq);

	//申请中断
	ret = request_irq(irq,key_func_handler,IRQF_TRIGGER_RISING,"test_key",NULL);
	if(ret < 0)
	{
		printk("request_irq is error\n");
		return -1;
	}

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

const struct of_device_id of_match_table_test[] = {
	{.compatible = "keys"},//该匹配名优先于platform_device_id，更优先于platform_driver.driver.name
	{}//一定要有这个空的花括号
};

struct platform_driver gpio_device = {
	.probe = gpio_probe,
	.remove = gpio_remove,
	.driver = {
		.name = "keys",
		.owner = THIS_MODULE,
		.of_match_table = of_match_table_test
	},
	.id_table = &gpio_id_table
};


/*功能的实现*/
static int gpio_driver_init(void)
{
	int ret =0;

	printk("**********hello world*************\n");

	ret = platform_driver_register(&gpio_device);
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
	printk("************Bye!*******************\n");
	free_irq(irq,NULL);
	platform_driver_unregister(&gpio_device);
}

/*驱动模块的入口和出口*/
module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

/*声明信息*/
MODULE_LICENSE("GPL");
