1、设备树，将硬件资源通过bootloader加载到内核中，不需要在换底板时重新修改内核并编译
2、设备树常用名词解释
	1）DT：Device Tree//设备树
	2）FDT：Flattened Device Tree//展开设备树//开放固件，设备树起源于OF，所以可以在设备树
中看到很多of字母的函数
	3）device tree source(dts)//设备树代码
	4）device tree source include(dtsi)//更通用的设备树代码，也就是相同芯片但不同都可以使
用的代码
	5）device tree blob(dtb)//DTS编译后得到的DTB文件
	6）device tree compiler(dtc)//设备树编译器
其中，DTS,DTSI,DTB,DTC的关系：
DTS,DTSI---DTC--->DTB

如何查看节点及其属性
方法一：/proc/device-tree目录下
方法二：/sys/firmware/devicetree/base目录下
两个目录下的内容相同

.dts .dtsi文件都放在内核源码 arch/arm/boot/dts 目录下
对对应板子的.dts文件进行添加自定义设备节点，该树莓派对应的为bcm2711-rpi-4-b.dts文件
在make dtbs或make bcm2711-rpi-4-b.dtb之前，先make config或make defconfig等生成内核编译

修改内核源码中的dts并编译后，按官方文档的构建内核文档执行
最后重启，树莓派将使用我们修改后的内核
