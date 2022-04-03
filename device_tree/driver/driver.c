#include<linux/init.h>//包含宏定义的头文件
#include<linux/module.h>//包含初始化加载模块的头文件
#include<linux/platform_device.h>
#include<linux/mod_devicetable.h>
#include<linux/of.h>
#include<linux/of_address.h>
#include<linux/miscdevice.h>//这两个都是杂项设备必要的头文件
#include<linux/fs.h>
#include<linux/uaccess.h>

int size;
u32 out_value[2]={0};
const char *str;

struct device_node *test_deive_node;
struct property *test_node_property;

unsigned int *vir_gpsel2;
unsigned int *vir_gpset0;
unsigned int *vir_gpclr0;

int misc_open(struct inode *inode,struct file *file)
{
	printk("hello misc_open\n");
	return 0;
}

int misc_release(struct inode *inode,struct file *file)
{
	printk("bye misc_release\n");
	return 0;
}

ssize_t misc_read(struct file *file,char __user *ubuf,size_t size,loff_t *loff_t)
{
	char kbuf[64] = "this data is from kernel";
	if(raw_copy_to_user(ubuf,kbuf,strlen(kbuf))!=0)
	{
		printk("copy_to_user error\n");
		return -1;
	}

	printk("misc read\n");
	return 0;
}

ssize_t misc_write(struct file *file,const char __user *ubuf,size_t size,loff_t *loff_t)
{
	int kbuf[64] = {0};
	if(raw_copy_from_user(kbuf,ubuf,size)!=0)
	{
		printk("copy_from_user error\n");
		return -1;
	}

	printk("misc write\n");
	printk("kbuf is %d\n",kbuf[0]);

	*vir_gpsel2=(0x001<<12);

	if(kbuf[0]==1)
		*vir_gpset0=(0x1<<24);
	else if(kbuf[0]==0)
		*vir_gpclr0=(0x1<<24);	

	return 0;
};

struct file_operations misc_fops = {
	.owner = THIS_MODULE,
	.open = misc_open,
	.release = misc_release,
	.read = misc_read,
	.write = misc_write
};

struct miscdevice misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "gpio_misc",
	.fops =&misc_fops
};


int gpio_probe(struct platform_device *pdev)
{
	int ret = 0;
	
	printk("gpio_probe\n");
	//直接获取节点名
	//printk("node name is %s\n",pdev->dev.of_node->name);
	
	//使用of函数获取
	/*获取节点*/
	//pdev->dev.of_node等同于test_deivce_node
	test_deive_node = of_find_node_by_path("/test");
	if(test_deive_node == NULL)
	{
		printk("of_find_node_by_path is error\n");
		return -1;
	}

	/*获取reg属性*/
	ret = of_property_read_u32_array(test_deive_node,"reg",out_value,6);
	if(ret<0)
	{
		printk("of_property_read_u32_array is eeror\n");
		return -1;
	}
	printk("out_value[0] is 0x%08x\n",out_value[0]);
	printk("out_value[1] is 0x%08x\n",out_value[1]);
	printk("out_value[2] is 0x%08x\n",out_value[2]);
	printk("out_value[3] is 0x%08x\n",out_value[3]);
	printk("out_value[4] is 0x%08x\n",out_value[4]);
	printk("out_value[5] is 0x%08x\n",out_value[5]);

	vir_gpsel2 = of_iomap(pdev->dev.of_node,0);
	vir_gpset0 = of_iomap(pdev->dev.of_node,1);
	vir_gpclr0 = of_iomap(pdev->dev.of_node,2);
	if((vir_gpsel2 == NULL)||(vir_gpset0 == NULL)||(vir_gpclr0 == NULL))
	{
		printk("of_iomap is error\n");
		return -1;
	}
	//注册杂项设备
	ret = misc_register(&misc_dev);
	if(ret < 0)
	{
		printk("misc register is error\n");
		return -1;
	}

	printk("misc register is ok\n");

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
	{.compatible = "test"},//该匹配名优先于platform_device_id，更优先于platform_driver.driver.name
	{}//一定要有这个空的花括号
};

struct platform_driver gpio_device = {
	.probe = gpio_probe,
	.remove = gpio_remove,
	.driver = {
		.name = "gpio_test",
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
	misc_deregister(&misc_dev);
	platform_driver_unregister(&gpio_device);
}

/*驱动模块的入口和出口*/
module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

/*声明信息*/
MODULE_LICENSE("GPL");
