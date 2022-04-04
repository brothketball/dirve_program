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
    <1>宏DECLARE_WORK
    原型：#define DECLARE_WORK(n, f)