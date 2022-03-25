1、中断基本流程：
	1 中断请求
	2 中断响应
	3 保护现场
	4 中断处理
	5 恢复现场
	6 中断返回
或者
	1 中断屏蔽

2、Linux提出把中断服务程序分为两部分：中断上文和中断下文
	中断上文：完成尽可能少且比较急的任务，中断上文的特点就是响应速度快。
			若在中断上文处理耗时间的任务，系统将会崩溃。
	中断下文：处理中断剩余的大量比较耗时间的任务，而且可以被新的中断打断。
	总之：中断上文越快越好，中断下文可以做比较耗时间的事，但不能死循环。

3、Linux中断嵌套
	以前可以，现在不可以

/*******************************************************/

一、设备树中的中断节点
	若一个设备需要用到中断功能，开发人员就需要在设备树中配置好中断属性信息，因为设备树时用来描述硬件信息的，然后Linux内核通过设备树配置的中断属性来配置中断功能。
	设备树中断的参考绑定文档：源码/Documentation/devicetree/bindings/arm/gic.txt
	中断触发类型宏定义在源码/include/linux/irq.h中定义

	在设备树中配置中断时，需要两个步骤（当为gpio中断时）：
	第一：将管教设置为gpio功能
	第二：使用interrupt-parent和intrrupts属性来描述中断
		或者，在配置gpio中断时，可以使用函数来获取gpio对应的中断号，不需要使用这两个属性
		但是写上该两个属性是标准的

二、中断相关函数
	1、获取中断号相关函数
		编写驱动时需要用到中断号，每个中断都有中断号，中断信息已经写到设备树中，因此可以通过irq_of_parse_and_map函数从interrupts属性中提取到对应的设备号，函数原型如下：
		unsigned int irq_of_parse_and_map(struct device_node *dev,int index)
		参数：
			dev：设备节点
			index：索引号，interrupts属性包含多条中断信息，通过index指定要获取的信息
		返回值：中断号
	
		如果使用GPIO的话，可以使用gpio_to_irq函数来获取gpio对应的中断号，函数原型如下：
		int gpio_to_irq(unsigned int gpio)
		参数：
			gpio:要获得的GPIO的编号
		返回值：GPIO对应的中断号
	2、申请中断函数
		int request_irq(
			unsigned int irq,//要申请中断的中断号
			irq_handler_t handler,//中断处理函数，当中断发生以后就会执行此中断处理函数
			unsigned long flags,//中断标志
			const char *name,//中断名字，设置后可/proc/interrupts文件中看到对应的中断名字
			void *dev//若flags为IRQF_SHARED，则dev用于区分不同的中断
			)
		flags：宏定义在 源码/include/linux/inerrupt.h 里面查看所有的中断标志
		dev：一般情况下将dev设置为设备结构体，dev会传递给中断处理函数irq_handler_t的第二个参数
		返回值：
			0，中断申请成功；-EBUSY，中断已被申请；其他负值，中断申请失败
	3、中断处理函数
		使用request_irq函数申请中断的时候需要设置中断处理函数，中断