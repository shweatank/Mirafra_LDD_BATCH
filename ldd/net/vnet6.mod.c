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



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x9cde6e67, "netif_tx_wake_queue" },
	{ 0xeda1754, "ether_setup" },
	{ 0x4aa64814, "consume_skb" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0x26d7d0e6, "init_net" },
	{ 0xc441315, "dev_get_by_name" },
	{ 0x675f7ddd, "unregister_netdev" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x9ce8faae, "alloc_netdev_mqs" },
	{ 0x1b518d9e, "register_netdev" },
	{ 0x122c3a7e, "_printk" },
	{ 0xbe1b0582, "free_netdev" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x6ad2b3e, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D5D0389FAFF541617A57F26");
