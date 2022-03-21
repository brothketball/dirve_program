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
	{ 0x459115a1, "platform_driver_unregister" },
	{ 0x6f8e7e03, "misc_deregister" },
	{ 0xfe990052, "gpio_free" },
	{ 0x9ae801e0, "__platform_driver_register" },
	{ 0xba4c293, "gpiod_set_raw_value" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x451e2b82, "misc_register" },
	{ 0x4ccd9305, "gpiod_direction_output_raw" },
	{ 0xe30324cf, "gpio_to_desc" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xf3e50104, "of_get_named_gpio_flags" },
	{ 0xd8278420, "of_find_node_opts_by_path" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x5f754e5a, "memset" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "166BA16A0E0BB7893D95012");
