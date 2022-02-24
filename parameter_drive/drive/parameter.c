/*包含头文件*/
#include<linux/init.h>//包含宏定义的头文件
#include<linux/module.h>//包含初始化加载模块的头文件

static int a;
static int b[5];
static int count;

module_param(a,int,S_IRUSR);
module_param_array(b,int,&count,S_IRUSR);

/*功能的实现*/
static int parameter_init(void)
{
	int i;

	for(i=0;i<count;i++)
	{
		printk("b[%d] = %d\n",i,b[i]);
	}

	printk("count = %d\n",count);

	printk("a = %d\n",a);
	return 0;
}

static void parameter_exit(void)
{
	//printk("a = %d\n",a);
	printk("Bye!\n");
}

/*驱动模块的入口和出口*/
module_init(parameter_init);
module_exit(parameter_exit);

/*声明信息*/
MODULE_LICENSE("GPL");
