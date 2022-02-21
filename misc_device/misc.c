/*包含头文件*/
#include<linux/init.h>//包含宏定义的头文件
#include<linux/module.h>//包含初始化加载模块的头文件
#include<linux/miscdevice.h>//这两个都是杂项设备必要的头文件
#include<linux/fs.h>

struct file_operations misc_fops = {
	.owner = THIS_MODULE
};

struct miscdevice misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "helo_misc",
	.fops =&misc_fops
};

/*功能的实现*/
static int misc_init(void)
{
	int ret;
	
	ret = misc_register(&misc_dev);

	if(ret<0)
	{
		printk("misc regidter is error\n");
		return -1;
	}
	
	printk("misc register is succeed\n");
	return 0;
}

static void misc_exit(void)
{
	misc_deregister(&misc_dev);
	printk("misc bye!\n");
}

/*驱动模块的入口和出口*/
module_init(misc_init);
module_exit(misc_exit);

/*声明信息*/
MODULE_LICENSE("GPL");
