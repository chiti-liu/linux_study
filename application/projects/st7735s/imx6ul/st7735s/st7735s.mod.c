#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x876437a0, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x149c7752, __VMLINUX_SYMBOL_STR(param_ops_uint) },
	{ 0xf267bf9d, __VMLINUX_SYMBOL_STR(fb_sys_read) },
	{ 0x78fdddb1, __VMLINUX_SYMBOL_STR(driver_unregister) },
	{ 0x75688a29, __VMLINUX_SYMBOL_STR(spi_register_driver) },
	{ 0x111baabc, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0xe2674708, __VMLINUX_SYMBOL_STR(devm_kfree) },
	{ 0x328594b7, __VMLINUX_SYMBOL_STR(register_framebuffer) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x74c266bf, __VMLINUX_SYMBOL_STR(fb_deferred_io_init) },
	{ 0x7a890c8, __VMLINUX_SYMBOL_STR(fb_alloc_cmap) },
	{ 0x6fde5db3, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x5210281a, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x9d669763, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xeb74cc90, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x156eaad1, __VMLINUX_SYMBOL_STR(framebuffer_alloc) },
	{ 0x7fbfea2a, __VMLINUX_SYMBOL_STR(spi_setup) },
	{ 0xe374f372, __VMLINUX_SYMBOL_STR(gpiod_direction_output) },
	{ 0xd025a460, __VMLINUX_SYMBOL_STR(__devm_gpiod_get_optional) },
	{ 0x6dbab696, __VMLINUX_SYMBOL_STR(devm_kmalloc) },
	{ 0xe707d823, __VMLINUX_SYMBOL_STR(__aeabi_uidiv) },
	{ 0xda81fad, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xf77193b9, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x8e865d3c, __VMLINUX_SYMBOL_STR(arm_delay_ops) },
	{ 0xfbc74f64, __VMLINUX_SYMBOL_STR(__copy_from_user) },
	{ 0x9d5fdc26, __VMLINUX_SYMBOL_STR(sys_fillrect) },
	{ 0x24e20674, __VMLINUX_SYMBOL_STR(sys_copyarea) },
	{ 0x9963a089, __VMLINUX_SYMBOL_STR(queue_delayed_work_on) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0xd36254f2, __VMLINUX_SYMBOL_STR(sys_imageblit) },
	{ 0xb9e0a28c, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x72493cd7, __VMLINUX_SYMBOL_STR(gpiod_put) },
	{ 0x8feee93d, __VMLINUX_SYMBOL_STR(framebuffer_release) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x98b71c6, __VMLINUX_SYMBOL_STR(fb_dealloc_cmap) },
	{ 0xbb92068, __VMLINUX_SYMBOL_STR(arm_dma_ops) },
	{ 0x612545b8, __VMLINUX_SYMBOL_STR(fb_deferred_io_cleanup) },
	{ 0x72659148, __VMLINUX_SYMBOL_STR(unregister_framebuffer) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0x9c4d5eea, __VMLINUX_SYMBOL_STR(spi_sync) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x5f754e5a, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x3fd4468a, __VMLINUX_SYMBOL_STR(gpiod_set_value) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("spi:st7735s");
MODULE_ALIAS("of:N*T*Cst7735s*");

MODULE_INFO(srcversion, "D361447A9BA98D40EF8DAA1");
