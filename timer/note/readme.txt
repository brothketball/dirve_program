一、Linux内核定时器概念
    不同于单片机定时器，Linux内核定时器是一种基于未来时间点的计时方式，以当前时刻为驱动时间点，
以未来的某一时刻为终止点。如，需要在10：05定时5分钟，则需要定时10：10。
    注意：内核定时器精度不高，不可作为高精度定时器使用，且内核定时器不是周期性运行的，
超时后会自动关闭，因此如果要实现周期性定时，则需要在定时处理函数中重新开启定时器。
类比手机闹钟的定时和默认响一次

二、Linux内核定时器基础知识
    Linux内核使用timer_list结构体表示内核定时器，timer_list定义在源码/include/linux/timer.h中
struct timer_list {
	struct hlist_node	entry;
	unsigned long		expires;//定时器超时时间(类比几点几分)，不是时长，单位是节拍数
	void			(*function)(struct timer_list *);//定时处理函数
	u32			flags;

#ifdef CONFIG_LOCKDEP
	struct lockdep_map	lockdep_map;
#endif
};//与视频教程中不同
	expires：
	单位是节拍数，等于定时的当前的时钟节拍计数（存储在系统的全局变量jiffues）+定时时长对应的时钟节拍数
	jiffies可以类比几时几分
	示例：从现在开始定时1s
	内核中有一个宏HZ，表示1s对应的节拍数，那么我们就可以通过这个宏把时间转换成节拍数。
	所以定时1s就是：expires = jiffies + 1*HZ
	HZ的值可以设置，Linux内核会使用CONFIG_HZ来设置自己的系统时钟，在源码/include/asm-generic/param.h中
# undef HZ
# define HZ		CONFIG_HZ	/* Internal kernel timer frequency */
# define USER_HZ	100		/* some user interfaces are */
	宏HZ就是CONFIG_HZ，因此HZ=100
	我们可以在编译内核时通过menuconfig设置，进入Kernel Features的Timer frequency进行设置
	function：
	超时处理函数

三、Linux内核定时器相关操作函数
	1、时间转换函数
	<1>ms转换为时钟节拍函数
	unsigned long msecs_to_jiffies(const unsigned int m)
	<2>us转换为时钟节拍函数
	unsigned long usecs_to_jiffies(const unsigned int u)

	举例：
	<1>定时10ms
	jiffies+msecs_to_jiffies(10)
	<2>定时10us
	jiffies+usecs_to_jiffies(10)
	2、宏DEFINE_TIMER（定义于源码/include/linux/timer.h下）
	原型：#define DEFINE_TIMER(_name, _function)
	作用：静态定义结构体变量并初始化
	参数：_name，变量名；function，超时处理函数
	3、add_timer函数
	原型：void add_timer(struct timer_list *timer);
	作用：add_timer函数用于向Linux内核注册定时器，使用add_timer函数向内核注册定时器后，定时器就开始运行
	参数：timer，要注册的定时器
	4、del_timer函数
	原型：int del_timer(struct timer_list * timer);
	作用：del_timer函数用于删除一个定时器，不管定时器有没有被激活，都可以用此函数删除。
		在多处理器系统上，定时器可能会在其他的处理器上运行，因此调用del_timer函数删除
		定时器之前要先等待其他处理的定时处理器函数退出
	参数：timer，要删除的定时器
	返回值：0，定时器未被激活；1，定时器已激活。
	5、mod_timer函数
	原型：int mod_timer(struct timer_list *timer, unsigned long expires);
	作用：用于修改定时器的值，若定时器未激活，该函数会激活定时器。
	参数：timer，要修改的定时器；expires，修改后的超时时间
	返回值：0，调用该函数时定时器未激活；1调用该函数时定时器已激活