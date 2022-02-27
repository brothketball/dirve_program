#include<linux/init.h>//包含宏定义的头文件
#include<linux/module.h>//包含初始化加载模块的头文件
#include<linux/platform_device.h>
#include<linux/mod_devicetable.h>
#include<linux/ioport.h>

#include<linux/miscdevice.h>//这两个都是杂项设备必要的头文件
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/io.h>//对虚拟地址和物理地址的操作

struct resource *gpio_sel2;
struct resource *gpio_set0;
struct resource *gpio_clr0;

struct resource *gpio_sel2_tmp;
struct resource *gpio_set0_tmp;
struct resource *gpio_clr0_tmp;

unsigned int *vir_gpio_sel2;
unsigned int *vir_gpio_set0;
unsigned int *vir_gpio_clr0;

//文件操作集的函数
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

	*vir_gpio_sel2=(0x001<<12);

	if(kbuf[0]==1)
		*vir_gpio_set0=(0x1<<24);
	else if(kbuf[0]==0)
		*vir_gpio_clr0=(0x1<<24);	

	return 0;
};
//定义文件操作集
struct file_operations misc_fops = {
	.owner = THIS_MODULE,
	.open = misc_open,
	.release = misc_release,
	.read = misc_read,
	.write = misc_write
};
//定义杂项设备
struct miscdevice misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "platform_simp_misc",
	.fops = &misc_fops
};



int gpio_probe(struct platform_device *pdev)
{
	int ret = 0;//注册杂项设备的返回值
	printk("gpio_probe\n");
/*********************************************/
//获取和登记硬件
/*********************************************/
//获取硬件资源
	gpio_sel2 = platform_get_resource(pdev,IORESOURCE_MEM,0);
	gpio_set0 = platform_get_resource(pdev,IORESOURCE_MEM,1);
	gpio_clr0 = platform_get_resource(pdev,IORESOURCE_MEM,2);

	if((gpio_sel2==NULL)|(gpio_set0==NULL)|(gpio_clr0==NULL))
	{
		printk("platform_get_resource is error\n");
		return -EBUSY;
	}

	printk("platform_get_resource is ok\n");

/********************************************/
//登记寄存器
#if 0
	gpio_sel2_tmp = request_mem_region(gpio_sel2->start,gpio_sel2->end-gpio_sel2->start+1,"gpio");
	gpio_set0_tmp = request_mem_region(gpio_set0->start,gpio_set0->end-gpio_set0->start+1,"gpio");
	gpio_clr0_tmp = request_mem_region(gpio_clr0->start,gpio_clr0->end-gpio_clr0->start+1,"gpio");
	
	if((gpio_sel2_tmp==NULL)|(gpio_set0_tmp==NULL)|(gpio_clr0_tmp==NULL))
	{
		printk("request_mem_region is error\n");
		goto err_region;
	}
#endif
/******************************************/
//杂项设备	
/******************************************/
//物理地址转为虚拟地址
	vir_gpio_sel2 = ioremap(gpio_sel2->start,4);
	vir_gpio_set0 = ioremap(gpio_set0->start,4);
	vir_gpio_clr0 = ioremap(gpio_clr0->start,4);
	if((vir_gpio_sel2 == NULL || vir_gpio_set0 == NULL)||vir_gpio_clr0 == NULL)
	{
		printk("GPIO ioremap error\n");
		return -EBUSY;//EBUSY宏定义为16，表示设备和资源忙碌
	}

	printk("GPIO ioremap ok\n");
/******************************************/
//注册杂项设备
	ret = misc_register(&misc_dev);
	if(ret<0)
	{
		printk("misc regidter is error\n");
		return -1;
	}
	
	printk("misc register is succeed!\n");


/******************************************/
	return 0;
/******************************************/

/******************************************/
//登记失败的goto
/******************************************/

#if 0
err_region:
	release_mem_region(gpio_sel2->start,gpio_sel2->end-gpio_sel2->start+1);
	release_mem_region(gpio_set0->start,gpio_set0->end-gpio_set0->start+1);
	release_mem_region(gpio_clr0->start,gpio_clr0->end-gpio_clr0->start+1);
	return -EBUSY;
#endif
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
	int ret = 0;

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
	printk("Bye!\r\n");
	platform_driver_unregister(&gpio_device);
	misc_deregister(&misc_dev);
	iounmap(vir_gpio_sel2);
	iounmap(vir_gpio_set0);
	iounmap(vir_gpio_clr0);
}

/*驱动模块的入口和出口*/
module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

/*声明信息*/
MODULE_LICENSE("GPL");
