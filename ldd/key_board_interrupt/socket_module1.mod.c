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

KSYMTAB_FUNC(socket_send_data, "", "");

SYMBOL_CRC(socket_send_data, 0xe6aa799d, "");

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x4c03a563, "random_kmalloc_seed" },
	{ 0x24980310, "kmalloc_caches" },
	{ 0x1d199deb, "kmalloc_trace" },
	{ 0x754d539c, "strlen" },
	{ 0x5cd9fcf0, "kernel_sendmsg" },
	{ 0x37a0cba, "kfree" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x122c3a7e, "_printk" },
	{ 0x9ec0c31e, "sock_release" },
	{ 0x9ad53c4f, "sock_create" },
	{ 0x75341a7, "kernel_bind" },
	{ 0xae4ab5f2, "kernel_listen" },
	{ 0x53fe8265, "kernel_accept" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x6ad2b3e, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "37F4E71478D8B19EC0B532C");
