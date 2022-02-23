#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbd97005b, "module_layout" },
	{ 0xedc03953, "iounmap" },
	{ 0x6f8e7e03, "misc_deregister" },
	{ 0x1d37eeed, "ioremap" },
	{ 0x451e2b82, "misc_register" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x5f754e5a, "memset" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "818FC8C75C5C9F703B510D2");
