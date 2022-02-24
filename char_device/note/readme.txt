1、字符设备和杂项设备的区别
	杂项设备的主设备号是固定的，固定为10，而字符类设备需要自己或者系统来分配
	杂项设备可以自动生成设备节点，字符设备需要我们自己生成设备节点

2、注册字符类设备号的两个方法
	1）静态分配设备号，使用的是：（定义于内核源码/include/linux/fs.h）
		register_chrdev_region(dev_t, unsigned, const char *);
		注意：需要明确知道我们的系统里面哪些设备号没有用
		
		参数：
		第一个 设备号的起始值，类型是dev_t
		第二个 次设备号的个数
		第三个 设备的名称（cat /proc/devices，可以查看设备名称和对应的主设备号）
		
		返回值：成功返回0，失败返回非0
		
		dev_t类型：
		用于保存设备号，是一个32位数，定义于内核源码/include/linux/types.h
		高12用于保存主设备号，低20位用于保存次设备号
		typedef u32 __kernel_dev_t;
		typedef __kernel_dev_t		dev_t;

		Linux提供几个宏定义来操作设备号，定义于内核源码/include/linux/kdev_t.h
		#define MINORBITS	20
		次设备号的位数，共20位
		
		#define MINORMASK	((1U << MINORBITS) - 1)
		次设备号的掩码
		
		#define MAJOR(dev)	((unsigned int) ((dev) >> MINORBITS))
		在dev_t中获取主设备号，实际上是将dev_t右移了20位
		
		#define MINOR(dev)	((unsigned int) ((dev) & MINORMASK))
		在dev_t中获取主设备号，实际上是将dev_t右移了2位

		#define MKDEV(ma,mi)	(((ma) << MINORBITS) | (mi))
		将主次设备号组成一个dev_t类型，第一个参数为主设备号，第二个参数为次设备号
	2）动态分配设备号，使用的是：（定义于内核源码/include/linux/fs.h）
		alloc_chrdev_region(dev_t *, unsigned, unsigned, const char *);
		注意：使用动态分配会优先使用255到234的设备号

		参数：
		第一个 保存生成的设备号
		第二个 请求的第一个次设备号，通常是0
		第三个 连续申请的设备号的个数
		第四个 设备名称
		
		返回值：成功返回0，失败返回非0

3、注销设备号
	我们使用的是 :（定义于内核源码/include/linux/fs.h）
	unregister_chrdev_region(dev_t,unsigned);
	
	参数：
	第一个 分配设备号的起始地址，即dev_t类型的变量
	第二个 申请的连续设备的个数

	建议：优先使用动态申请设备号，因为当多人使用一个Linux服务器时，静态申请容发生冲突
