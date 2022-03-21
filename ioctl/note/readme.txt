unlocked_ioctl接口
1、什么是unlocked_ioctl接口？
	unlocked_ioctl接口就是ioctl接口，但是功能和对应的系统调用均没有发生变化。
2、unlocked_ioctl和read/write函数有什么相同点和不同点？
	相同点：都可以向内核读写数据，都定义于file_operation结构体中
	不同点：read函数只能完成读功能，write函数只能完成写功能，ioctl即可读也可以写
			读取大数据时read/write函数效率更高
	所以：ioctl接口用于对设备的参数进行设置和查询，read/write函数只用于数据传输
3、unlocked_ioctl接口命令规则
	其命令为unsigned int类型，即32位，分为4个分区
	第一个分区(%d)：0-7，存放命令的编号，范围0-255
	第二个分区(%c)：8-15，命令的幻数
		第一和第二分区用于区分命令，不同的命令，其一二分区至少有一个分区不同
	第三个分区(数据类型)：16-29，表示传递数据的大小
	第四个分区(%d)：30-31，代表读写方向
				00：表示用户与驱动间无数据传递
				01：表示用户向驱动里写数据
				10：表示用户从驱动里读数据
				11：表示用户先向驱动里写数据，再从驱动里读数据
4、命令的合成宏和分解宏
	合成宏：
		_IO(type,nr)：用于定义第四分区00的命令
		_IOW(type,nr,size)：用于定义第四分区01的命令
		_IOR(type,nr,size)：用于定义第四分区10的命令
		_IOWR(type,nr,size)：用于定义第四分区11的命令
	参数：
		type：表示命令的幻数
		nr：表示命令的编号
		size：表示命令的参数传递大小，使用数据类型表示大小，如4字节用int表示
	
	分解宏：
		_IOC_DIR(nr)：分解命令的方向
		_IOC_TYPE(nr)：分解命令幻数
		_IOC_NR(nr)：分解命令的编号
		_IOC_SIZE(nr)：分解命令的数据大小
	参数：
		nr：要分解的命令
	
	注意：
			需要在app和driver程序中定义相同的命令
			与read/write同理，在app中调用ioctl函数时，会触发driver中对应的ioctl函数
			app中ioctl的参数有，fd文件句柄、命令、数据（可选）
			driver中对应ioctl函数原型为：
			long misc_ioctl(struct file *file,unsigned int cmd,unsigned long value)
			两者参数一一对应，通过cmd和value形参传递
			
	注意：
		用户向驱动传递数据时，用户直接对value赋值并作为ioctl的参数
		用户从驱动读取数据时，用户定义变量value并将其指针作为ioctl参数，驱动使用copy_to_user函数向用户传递其要读取的数据
