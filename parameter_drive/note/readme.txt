1、什么是驱动传参
	驱动传参就是传递参数给我们的驱动
举例：
insmod simp.ko a=1

2、驱动传参有什么作用
	1）设置驱动的相关参数，比如设置缓冲区的大小
	2）设置安全校验，防止我们写的驱动被人盗用

3、怎么给驱动传参数
	1）传递普通的参数，如char int类型
		函数：
		module_param(name,type,perm);
		参数：
		name 要传递进去参数的名称
		type 类型
		perm 参数读写的权限
		注意：在加载该驱动的时候会传入命令中的参数，然后在/sys/module/下有与加载驱动同名的目录，在该目录下有parameters目录，parameters目录下放有传入的参数文件，以变量名命名，perm所定义的权限就是该文件的权限
	2）传递数组
		函数：
		module_param_array(name,type,nump,perm);
		参数：
		name 要传递进去的参数的名称
		nump 实际传入进去的参数的个数，不是数组的长度
		perm 参数读写的权限
		注意：当传入参数个数大于数组定义长度则报错
