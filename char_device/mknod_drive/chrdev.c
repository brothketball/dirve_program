/*包含头文件*/
#include<linux/init.h>//包含宏定义的头文件
#include<linux/module.h>//包含初始化加载模块的头文件
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/device/class.h>
#include<linux/device.h>

#define DEVNUM 1
#define DEVSNAME "schrdev"
#define DEVANAME "achrdev"
#define DEVMINOR_NUM 0
#define DEVCLASS_NAME "chrdev_class"
#define DEVNOD_NAME "chrdev_nod"

static int major_num,minor_num;
struct cdev cdev;
dev_t dev_num;
struct class *class;
struct device *device;

module_param(major_num,int,S_IRUSR);
module_param(minor_num,int,S_IRUSR);//传参函数要先于文件操作集结构体的定义

int chrdev_open(struct inode *inode,struct file *file)
{
	printk("chrdev_open\n");
	return 0;
}

struct file_operations chrdev_ops = {
	.owner = THIS_MODULE,
	.open = chrdev_open
};

/*功能的实现*/
static int chrdev_init(void)
{
	printk("hello!\n");	

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

	cdev.owner = THIS_MODULE;
	cdev_init(&cdev,&chrdev_ops);
	cdev_add(&cdev,dev_num,DEVNUM);
	class = class_create(THIS_MODULE,DEVCLASS_NAME);
	device = device_create(class,NULL,dev_num,NULL,DEVNOD_NAME);

	return 0;
}

static void chrdev_exit(void)
{
	unregister_chrdev_region(MKDEV(major_num,minor_num),DEVNUM);
	cdev_del(&cdev);
	device_destroy(class,dev_num);//先device_destroy再class_destroy，因为device在class中
	class_destroy(class);
	printk("bye!\n");
}

/*驱动模块的入口和出口*/
module_init(chrdev_init);
module_exit(chrdev_exit);

/*声明信息*/
MODULE_LICENSE("GPL");
