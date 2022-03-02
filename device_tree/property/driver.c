/*包含头文件*/
#include<linux/init.h>//包含宏定义的头文件
#include<linux/module.h>//包含初始化加载模块的头文件

#include<linux/of.h>

struct device_node *test_device_node;
struct property *test_property;
int size;
u32 out_values[2]={0};
const char *str;

/*功能的实现*/
static int hello_init(void)
{
	int ret;

	printk("Hello world!\r\n");
	
	/*查找我们要查找的节点*/
	test_device_node = of_find_node_by_path("/test");

	if(test_device_node ==  NULL)
	{
		printk("of_find_node_by_path is error\n");
		return -1;
	}

	printk("test_device_node name is %s\n",test_device_node->name);

	/*获取compatible属性内容*/
	test_property = of_find_property(test_device_node,"compatible",&size);
	if(test_property == NULL)
	{
		printk("of_find_property is error\n");
		return -1;	
	}

	printk("test_property value is %s\n",(char *)test_property->value);//没有char *，有警告
	printk("test_property name is %s\n",test_property->name);

	/*获取reg属性内容*/
	ret = of_property_read_u32_array(test_device_node,"reg",out_values,2);
	if(ret < 0)
	{
		printk("of_property_read_u32_array error\n");
		return -1;
	}
	printk("out_values[0] is 0x%08x\n",out_values[0]);
	printk("out_values[1] is 0x%08x\n",out_values[1]);
	
	/*获取status属性内容*/
	ret = of_property_read_string(test_device_node,"status",&str);//函数原型的最后一个参数要取两次地址
	if(ret < 0)
	{
		printk("of_property_read_string error\n");
		return -1;
	}
	printk("status is %s\n",str);

	return 0;
}

static void hello_exit(void)
{
	printk("Bye!\r\n");
}

/*驱动模块的入口和出口*/
module_init(hello_init);
module_exit(hello_exit);

/*声明信息*/
MODULE_LICENSE("GPL");
