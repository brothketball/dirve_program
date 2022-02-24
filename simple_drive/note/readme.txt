需要注意的地方：
1 命令 pinout 可查询使用的树莓派对应的BCM数据手册版本，该板子对应BCM2711
2 树莓派上的引脚编号使用BCM编号
3 GPIO常用寄存器功能：
	GPFSELn 设置引脚输入输出或其他功能
	GPSETn  设置各个寄存器位为1，对应引脚置1
	GPCLRn  设置各个寄存器位为1，对应引脚置0
4 wiringpi命令 gpio readall 查看引脚状态
5 使用 sudo cat /proc/iomem 命令可以查看各个寄存器地址范围以及对应的基地址
6 无法使用GPIO对小风扇进行供能，虽然电压足够，但是电流过低
