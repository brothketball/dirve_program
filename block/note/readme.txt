一、阻塞与非阻塞的概念
    阻塞：当前设备如果不可读或不可写时，也就是不能获得资源的时候，那么当前进程会被挂起。
        只有当设备满足条件的时候才可以返回。默认情况下，文件都以该方式打开。
    非阻塞：当前设备不可读或不可写时，该函数不会阻塞当前的进程，要么放弃，要么不停的查询，
        或者直到可以操作为止。
    读写函数是否阻塞可以通过参数来指定：
        fd = open(filepath,O_RDWR);//默认阻塞打开
        fd = open(filepath,O_RDWR|O_NONBLOCK);//非阻塞打开
二、等待队列基础知识
    等待队列可以实现，驱动在条件不满足时进程休眠；在条件满足时由内核唤醒进程。
    因为在休眠时会将所有资源给其他进程，所有需要中断的配合唤醒。
<1>等待队列头
    等待队列头就是一个等待队列的头部，每个访问设备的进程都是一个队列项，当设备的不可用时
就要将这些进程对应的等待队列项添加到等待队列里面。
    等待队列头使用结构体wait_queue_head_t来表示，这个结构体定义在源码/include/linux/wait.h里面
    struct wait_queue_head {
	spinlock_t		lock;//自旋锁
	struct list_head	head;//链表头
    };
    typedef struct wait_queue_head wait_queue_head_t;
    类型名是wait_queue_head_t,只需记住这个即可。
    定义一个等待队列头：
    wait_queue_head_t test_wq;//定义一个等待队列的头
    定义后需要初始化，使用init_waitqueue_head函数初始化等待队列头
    void init_waitqueue_head(wait_queue_head_t *q);
    
    也可以使用宏DECLARE_WAIT_QUEUE_HEAD来一次性完成等待队列头的定义和初始化
    DECLARE_WAIT_QUEUE_HEAD(wait_queue_head_t *q);
三、等待队列相关函数
<1>init_waitqueue_head宏
原型：void init_waitqueue_head(wait_queue_head_t *q);
作用：动态初始化等待队列头结构体
参数：q是wait_queue_head_t指针

<2>wait_event宏
原型：wait_event(wq,condition)
作用：不可中断的阻塞等待，让调用进程加入不可中断的睡眠状态，在等待队列里面睡眠直到condition变成真，被内核唤醒。
参数：
    wq：wait_queue_head_t类型变量
    condition：等待的条件，为假时进入休眠
注意：调用时要确认condition值的真假，若真，则不会休眠

<3>wait_event_interruptible宏
原型：wait_event(wq,condition)
作用：可中断的阻塞等待，让调用进程加入可中断的睡眠状态，在等待队列里面睡眠直到condition变成真，被内核唤醒或被中断唤醒。
参数：
    wq：wait_queue_head_t类型变量
    condition：等待的条件，为假时进入休眠
返回值：判断condition是否为真，若为真则返回0，否则检查进程是否被信号唤醒，返回-ERESTARTSYS错误码。
注意：调用时要确认condition值的真假，若真，则不会休眠

<4>wake_up宏
原型：wake_up(x)
作用：唤醒所有休眠进程
参数：x为等待队列头的结构体指针

<5>wake_up_interruptible宏
原型：wake_up_interruptible(x)
作用：唤醒可中断的休眠进程
参数：x为等待队列头的结构体指针

注意：
    <2><3>在阻塞时，不会向后执行代码；不阻塞后，就会执行后续代码
    <4><5>在唤醒休眠进程时，若condition为假，则会唤醒失败