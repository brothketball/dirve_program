1、什么是pinctrl和gpio子系统？
	Linux提供的对gpio进行开发的驱动框架
2、Linux pinctrl子系统提供的功能：
	1 管理系统中所有可以控制的pin，在系统初始化的时候，枚举所有可以控制的pin，并标识这些p
in
	2 管理这些pin的复用。对于SoC而言，若干引脚还可以组成pin group，形成特定的功能
	3 配置这些pin的特性。例如开关引脚的pull-up、pull-down电阻，配置引脚的driver strength
3、不同SoC厂家对应的pin controller节点不同，这些设备树节点都是某些引脚复用成某些功能
4、不同SoC厂家的pin control的设备树节点中的属性的意思是什么？
	对应说明文档放在内核源码include/Documentation/devicetree/bindings/pinctrl目录下
5、怎么在代码里面使用pin control里面定义好的节点（树莓派dts中的pwm1、uart0等）？
（以下属性在同目录下的pinctrl-bindings.txt中）
	例1：
pinctrl-names="default";
pinctrl-0=<&pinctrl_hog_1>;
含义：
<1>pinctrl-names="default";
	设备的状态，可以有多个状态，default为状态0
<2>pinctrl-0=<&pinctrl_hog_1>;
	第0个状态所对应的引脚配置，也就是default状态对应的引脚在pin controller里面定义好的节
点pinctrl_hog_1里面的管脚配置，即将定义好的节点名字填入即可

例二：
pinctrl-names="default","wake up";
pinctrl-0=<&pinctrl_hog_1>;
pinctrl-1=<&pinctrl_hog_2>;
<1>pinctrl-names="default","wake up";
	设备的状态，可以有多个状态，default为状态0，wake up为状态1
<2>pinctrl-0=<&pinctrl_hog_1>;
	第0个状态所对应的引脚配置，也就是default状态对应的引脚在pin controller里面定义好的节
点pinctrl_hog_1里面的管脚配置，即将定义好的节点名字填入即可
<2>pinctrl-1=<&pinctrl_hog_2>;
	第0个状态所对应的引脚配置，也就是default状态对应的引脚在pin controller里面定义好的节
点pinctrl_hog_2里面的管脚配置，即将定义好的节点名字填入即可

例三：
pinctrl-names="default";
pinctrl-0=<&pinctrl_hog_1
			&pinctrl_hog_2>;
<1>pinctrl-names="default";
	设备的状态，可以有多个状态，default为状态0
<2>pinctrl-0=<&pinctrl_hog_1>;
	第0个状态所对应的引脚配置，也就是default状态对应的引脚在pin controller里面定义好的节
点pinctrl_hog_1和pinctrl_hog_2里面的管脚配置，即将定义好的节点名字填入即可
