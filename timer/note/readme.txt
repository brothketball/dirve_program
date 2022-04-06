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