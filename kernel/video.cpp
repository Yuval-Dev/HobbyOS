#include"video.h"
#include"bios_int.h"
#include"monitor.h"
bool get_vbe_info(vbe_info_structure * dst) {
	regs16_t out = bios_int(0x10, (regs16_t){di:(uint16_t)dst,ax:0x4F00,es:0x0});
	return out.ax==0x004F;
}
bool get_vbe_mode_info(vbe_mode_info_structure * dst, uint16_t mode) {
	regs16_t out = bios_int(0x10, (regs16_t){di:(uint16_t)dst,cx:mode,ax:0x4F01,es:0x0});
	return out.ax==0x004F;
}
void dump_mode_info(vbe_mode_info_structure * structure) {
	print_str("Width: ");
	print_dec(structure->width);
	print_str(", Height: ");
	print_dec(structure->height);
	print_str(", Bits-per-pixel: ");
	print_dec(structure->bpp);
	print_str(", Buffer address: ");
	print_hex(structure->framebuffer);
}
