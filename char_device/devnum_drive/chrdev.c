/*包含头文件*/
#include<linux/init.h>//包含宏定义的头文件
#include<linux/module.h>//包含初始化加载模块的头文件
#include<linux/fs.h>
#include<linux/kdev_t.h>

#define DEVNUM 1
#define DEVSNAME "schrdev"
#define DEVANAME "achrdev"
#define DEVMINOR_NUM 0

static int major_num,minor_num;

module_param(major_num,int,S_IRUSR);
module_param(minor_num,int,S_IRUSR);

/*功能的实现*/
static int chrdev_init(void)
{	
	dev_t dev_num;

	int ret;

	if(major_num)
	{
		printk("major_num = %d\n",major_num);
		printk("minor_num = %d\n",minor_num);

		dev_num = MKDEV(major_num,minor_num);
		ret = register_chrdev_region(dev_num,DEVNUM,DEVSNAME);
		if(ret < 0)
		{
			printk("register_chrdev_region error\n");
			return -1;
		}
		printk("register_chrdev_region ok\n");
	}
	else
	{
		ret = alloc_chrdev_region(&dev_num,DEVMINOR_NUM,DEVNUM,DEVANAME);	
		if(ret < 0)
		{
			printk("alloc_chrdev_region error\n");
			return -1;
		}
		printk("alloc_chrdev_region ok\n");
		
		major_num=MAJOR(dev_num);
		minor_num=MINOR(dev_num);

		printk("major_num = %d\n",major_num);
		printk("minor_num = %d\n",minor_num);
	}
	return 0;
}

static void chrdev_exit(void)
{
	unregister_chrdev_region(MKDEV(major_num,minor_num),DEVNUM);
}

/*驱动模块的入口和出口*/
module_init(chrdev_init);
module_exit(chrdev_exit);

/*声明信息*/
MODULE_LICENSE("GPL");
