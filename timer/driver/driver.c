/*包含头文件*/
#include<linux/init.h>//包含宏定义的头文件
#include<linux/module.h>//包含初始化加载模块的头文件

#include<linux/timer.h>

static void timer_func(struct timer_list *timer);

DEFINE_TIMER(test_timer,timer_func);

static void timer_func(struct timer_list *timer)
{
    	printk("This is timer_func\n");
        mod_timer(&test_timer,jiffies + 1*HZ);
}

/*功能的实现*/
static int hello_init(void)
{
	printk("hello world!\n");

    test_timer.expires = jiffies + 1*HZ;
    add_timer(&test_timer);

	return 0;
}

static void hello_exit(void)
{
	printk("bye!\n");
    del_timer(&test_timer);
}

/*驱动模块的入口和出口*/
module_init(hello_init);
module_exit(hello_exit);

/*声明信息*/
MODULE_LICENSE("GPL");
