/*包含头文件*/
#include<linux/init.h>//包含宏定义的头文件
#include<linux/module.h>//包含初始化加载模块的头文件
#include<linux/miscdevice.h>//这两个都是杂项设备必要的头文件
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/io.h>//对虚拟地址和物理地址的操作

#define GPIO5_0 0x3f20001C  
#define GPIO5_1 0x3f200020  

unsigned int *vir_gpio5_0;
unsigned int *vir_gpio5_1;

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

	if(kbuf[0]==0)
		*vir_gpio5_0=0x12;
	else if(kbuf[0]==1)
		*vir_gpio5_1=0x12;	

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
	.name = "hello_misc",
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
	
	printk("misc register is succeed!\n");
	
	vir_gpio5_0 = ioremap(GPIO5_0,4);
	vir_gpio5_1 = ioremap(GPIO5_1,4);
	if(vir_gpio5_0 == NULL || vir_gpio5_1 == NULL)
	{
		printk("GPIO5 ioremap error\n");
		return -EBUSY;//EBUSY宏定义为16，表示设备和资源忙碌
	}

	printk("GPIO5 ioremap ok\n");

	return 0;
}

static void misc_exit(void)
{
	misc_deregister(&misc_dev);

	iounmap(vir_gpio5_0);
	iounmap(vir_gpio5_1);

	printk("misc bye!\n");
}

/*驱动模块的入口和出口*/
module_init(misc_init);
module_exit(misc_exit);

/*声明信息*/
MODULE_LICENSE("GPL");
