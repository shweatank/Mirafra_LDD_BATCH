#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

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



static const char ____versions[]
__used __section("__versions") =
	"\x18\x00\x00\x00\x04\x6b\xad\x9a"
	"gpio_to_desc\0\0\0\0"
	"\x1c\x00\x00\x00\x38\xf4\xd4\x03"
	"gpiod_set_raw_value\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x20\x00\x00\x00\xb6\x1a\xd4\xbd"
	"sysfs_remove_file_ns\0\0\0\0"
	"\x14\x00\x00\x00\xb0\xd8\xae\x22"
	"kobject_put\0"
	"\x14\x00\x00\x00\x52\x00\x99\xfe"
	"gpio_free\0\0\0"
	"\x18\x00\x00\x00\x5c\x9b\x22\x47"
	"gpio_request\0\0\0\0"
	"\x24\x00\x00\x00\x07\xb6\x33\xb5"
	"gpiod_direction_output_raw\0\0"
	"\x14\x00\x00\x00\xcd\x6b\xa8\xc7"
	"kernel_kobj\0"
	"\x20\x00\x00\x00\x01\xd2\xee\xd6"
	"kobject_create_and_add\0\0"
	"\x20\x00\x00\x00\x34\x86\xf4\xdf"
	"sysfs_create_file_ns\0\0\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x10\x00\x00\x00\xfd\xf9\x3f\x3c"
	"sprintf\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x14\x00\x00\x00\xcb\x69\x85\x8c"
	"kstrtoint\0\0\0"
	"\x18\x00\x00\x00\x3a\x0a\xd8\xfc"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "A9712B4BFDA7CE3D10C8CB4");
