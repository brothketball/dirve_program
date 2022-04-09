1、什么是输入子系统
    输入子系统是Linux专门做的一套框架来处理输入事件的，如鼠标、键盘、触摸屏这些都为输入设备。
但是这些输入设备的类型又都是不一样的，所以为了统一这些输入设备驱动标准应运而生。
    统一后，在节点/dev/input下则为输入设备的节点
    可使用 cat /proc/bus/input/devices 查看当前系统有哪些输入设备
    可使用 hexdump /dev/input/某节点 来判断哪个节点和哪个输入设备对应，当使用hexdump后，
若有打印信息，说明该节点有对应输入设备，若操作输入设备，如点击键盘后有打印信息，说明该节点对应键盘设备。
    这些打印信息是什么意思？
    我们上报的数据要按照具体的格式上报给输入子系统的核心层，我们的应用就可以通过设备节点来获得按照具体
格式上报的数据。
    封装数据由核心层完成，我们只需要按指定的类型和该类型对应的数据来上报给核心层即可。
    如何指定类型？struct input_event结构体，定义于源码/include/uapi/linux/input.h中
struct input_event {
#if (__BITS_PER_LONG != 32 || !defined(__USE_TIME_BITS64)) && !defined(__KERNEL__)
	struct timeval time;//上报事件的时间
#define input_event_sec time.tv_sec
#define input_event_usec time.tv_usec
#else
	__kernel_ulong_t __sec;
#if defined(__sparc__) && defined(__arch64__)
	unsigned int __usec;
	unsigned int __pad;
#else
	__kernel_ulong_t __usec;
#endif
#define input_event_sec  __sec
#define input_event_usec __usec
#endif
	__u16 type;//类型，类型不同，则编码和值就不同
	__u16 code;//编码
	__s32 value;//值
};
type 在该文件中有对应宏定义在/include/uapi/linux/input-event-codes.h中
code 若为键盘，各按键对应宏定义在/include/uapi/linux/input-event-codes.h中
value 为1代表按下；为0代表抬起；为2代表长按
hexdump /dev/input/键盘节点 的打印信息，最后四列依次是type code value