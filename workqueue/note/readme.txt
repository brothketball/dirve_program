一、工作队列的基础概念
1、什么是工作队列？
    工作队列是实现中断下文的机制之一，是一种将工作退后执行的形式。
    与tasklet不同的是，工作队列可以休眠，而tasklet不可以休眠。
    所以tasklet用于处理耗时事件，而工作队列用于处理更耗时且复杂的事件
2、工作队列的工作原理
    Linux系统在秋冬期间会创建内核线程，然后处于sleep状态并一直去队列读，查看是否有任务，
    若有，则执行；若无，则休眠。其实现机制复杂，初学阶段只需要了解基本概念接口。
    共享工作队列：多个不同的任务使用同一个工作队列
        优缺点：不需要自己创建，但当前面工作耗时时，会影响后续工作
    自定义工作队列：当前工作队列不能处理该任务时，需要重新创建工作队列
        优缺点：不受其他工作影响，但需要自己创建，系统开销大

二、工作队列相关API
    一个具体的工作我们使用结构体work_struct来描述，定义在源码/include/linux/workqueue.h
    使用工作队列第一步，定义一个工作队列
    struct work_struct {
        atomic_long_t data;
        struct list_head entry;
        work_func_t func;//关注该成员即可，是一个函数指针，对应函数用于完成需要完成的工作
    #ifdef CONFIG_LOCKDEP
        struct lockdep_map lockdep_map;
    #endif
    };

    第二步，初始化工作队列
    <1>宏DECLARE_WORK
    原型：#define DECLARE_WORK(n, f)
    作用：静态定义并初始化工作队列
    <2>宏INIT_WORK(_work,_func)
    作用：动态定义并初始化工作结构
    参数：
        work：工作队列地址
        func：工作函数

    注意：func中的输入参数需要添加一个struct work_struct *work，原因不明

    举例：
    struct work_struct test;
    在模块的初始化函数中：
    INIT_WORK(&test,func);
    相当于：
    DECLARE_WORK(test,func);

    第三步，调度工作
    <3>schedule_work
    原型：int_schedule_work(struct work_struct *work);
    作用：调度工作，把work_struct挂到CPU相关的工资结构队列链表上，等待工作者线程处理。
    参数：_work工作队列地址；
    需要注意的是，如果调度完工作，并不会马上执行，只是加到了共享的工作队列里面去，等轮到他才会执行。
    如果我们多次调用相同的任务，假如上一次的任务还没有处理完成，那么多次调度相同的任务是无效的。
    
    注意：共享队列中，没有和tasklet一样有类似的tasklet_kill的API
        和tasklet一样，也是先等中断上文执行完再过一段的时间，然后在执行中断下文
        因为workqueue和tasklet一样，都是中断下文的实现方法