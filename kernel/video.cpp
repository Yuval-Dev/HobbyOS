#include"video.h"
#include"bios_int.h"
#include"monitor.h"
vbe_info_structure vbe_info;
display_info display_modes[256];
uint8_t num_display_modes;
uint8_t active_display_mode = 80;
bool lfb_active;

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
	if((structure->attributes>>7)&1) print_str("has lfb");
	else print_str("no lfb");
}

void dump_mode_info(display_info structure) {
	print_str("{width:");
	print_dec(structure.width);
	print_str(",height:");
	print_dec(structure.height);
	print_str(",bits-per-pixel:");
	print_dec(structure.bpp);
	print_str(",buffer address:");
	print_hex(structure.frame_buffer);
	print_str(",has_lfb:");
	if(structure.linear_frame_buffer) print_str("true");
	else print_str("false");
	print_str(",mode:");
	if(structure.graphics_mode) print_str("graphics}");
	else print_str("text}");
}

vbe_mode_info_structure get_vbe_mode_info(uint16_t mode) {
	vbe_mode_info_structure * tmp = (vbe_mode_info_structure *)0x1000;
	if(!get_vbe_mode_info(tmp, mode));
	return *tmp;
}

display_info get_display_info(uint16_t mode) {
	vbe_mode_info_structure s = get_vbe_mode_info(mode);
	display_info ret;
	ret.mode_id = mode;
	ret.pitch = s.pitch;
	ret.width = s.width;
	ret.height = s.height;
	ret.bpp = s.bpp;
	ret.frame_buffer = s.framebuffer;
	ret.graphics_mode = (s.attributes>>4)&1;
	ret.linear_frame_buffer = (s.attributes>>7)&1;
	return ret;
}

vbe_info_structure get_vbe_info() {
	vbe_info_structure * tmp = 0x500;
	get_vbe_info(tmp);
	return *tmp;
}

void init_video_driver() {
	vbe_info = get_vbe_info();
	uint16_t * display_mode_codes = (uint16_t *)(((vbe_info.video_modes>>16)<<4) + vbe_info.video_modes&0xFFFF);
	while(display_mode_codes[num_display_modes]!=0xFFFF && num_display_modes<256) {
		display_modes[num_display_modes] = get_display_info(display_mode_codes[num_display_modes]);
		num_display_modes++;
	}
	uint16_t cur_mode = get_current_mode();
	if((cur_mode>>14)&1) lfb_active = true;
	uint16_t mode_id = (cur_mode<<2)>>2;
	for(uint8_t i = 0; i < num_display_modes; i++) {
		if(display_modes[i].mode_id==mode_id && lfb_active==display_modes[i].linear_frame_buffer) active_display_mode = i;
	}
}

uint16_t get_current_mode() {
	regs16_t out = bios_int(0x10, (regs16_t){ax:0x4F03});
	return out.bx;
}

void set_current_mode(uint8_t mode) {
	uint16_t arg = display_modes[mode].mode_id;
	if(display_modes[mode].linear_frame_buffer) arg += (1<<14);
	regs16_t out = bios_int(0x10, (regs16_t){bx:arg,ax:0x4F02});
	if(out.ax==0x004F) {
		active_display_mode = mode;
		lfb_active = display_modes[mode].linear_frame_buffer;
	}
}
