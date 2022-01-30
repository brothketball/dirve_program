驱动分为四部分：
头文件
#include<linux/init.h>//包含宏定义的头文件
#include<linux/module.h>//包含初始化加载模块的头文件

驱动模块的入口和出口
module_init(hello_init);
module_exit(hello_exit);

声明信息
MODULE_LICENSE("GPL");

功能实现
static int hello_init(void)
{
	printk("Hello world!\r\n");
	return 0;
}

static voif hello_exit(void)
{
	printk("Bye!\r\n");
}

编译驱动的方法：
方法一：
把驱动编译成模块，然后使用命令把驱动加载到内核里面
方法二：
直接把驱动编译到内核

方法一步骤：
第一步：
先写一个Makefile，可参考first_drive中的Makefile
第二步：
编译驱动
编译驱动之前需要注意的问题：
1、内核源码一定要先编译通过
2、我们编译驱动模块用的内核源码一定要和我们开发板上运行的内核镜像是同一套
3、查看架构是否为arm，若为X86，需要改为arm
	通过export ARCH=arm来改变环境为arm
