example_Kconfig代码说明：

1、source "drivers/redled/Kconfig"
他会包含dirvers/redled/路径下的驱动文件，方便我们对菜单进行管理

2、config LED_4412
配置选项的名称（全称为CONFIG_LED_4412，该处省略了CONFIG）
在执行配置的时候，将会生成一个变量CONFIG_LED_4412，该变量在编译时会被Makefi
le引用

3、tristate "Led Support for GPIO Led"
tristate 表示驱动的状态，编译成模块，编译到内核，不编译
与之对应的还有bool，分别是编译到内核，不编译
"Led Support for GPIO Led" 代表make menuconfig显示的名字


4、depends on LEDS_CLASS
如 A depends on B 表示只有在选择B的时候才可以选择A，即A是依赖B的依赖，此处为
LED_CLASS是LED_4412的依赖
但，如果想直接去掉LED相关的驱动，直接去修改.config文件可以吗？
可以，但不推荐。如果有依赖的话，直接修改.config是不成功的

还有select，表示反向依赖，该选项被选中的时候，后面的定义也会被选中

5、help
   This option enable support for led
表示帮助信息

https://mp.weixin.qq.com/s/z8uUYp5A60hZFGcA_V997w
上述网址中有如何将驱动编译到内核的详细讲解
我自己下载的内核已经被我编译进了hello驱动
