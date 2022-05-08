#include"vbe.h"
#include"monitor.h"
#include"bios_int.h"
void init_vbe() {
	regs16_t regs = {di:0x2000,ax:0x4F00,es:0x0};
	bios_int(0x10, &regs);
	if(regs.ax!=0x004F) {
		print_str("Error: getting VBE info\n");
	
	}
	vbe_info = *((vbe_info_t*)0x2000);
	uint16_t * vbe_mode_vector = (uint16_t *)(((vbe_info.video_modes>>16)<<4) + vbe_info.video_modes&0xFFFF);
	
	while(vbe_mode_vector[num_modes]!=0xFFFF) {
		vbe_mode_info_t * target = (vbe_mode_info_t *)0x3000;
		regs = (regs16_t){di:0x3000, cx:vbe_mode_vector[num_modes],ax:0x4F01, es:0x0};
		bios_int(0x10, &regs);
		if(regs.ax!=0x004F) {
			print_str("Error: getting VBE mode info\n");
		}
		if((target->attributes>>4)&1) modes[num_modes].visual_mode = graphics;
		else modes[num_modes].visual_mode = text;
		if((target->attributes>>7)&1) modes[num_modes].buffer_mode = linear_frame;
		else modes[num_modes].buffer_mode = banks;
		modes[num_modes].mode_id = vbe_mode_vector[num_modes];
		modes[num_modes].frame_buffer = target->framebuffer;
		modes[num_modes].bpp = target->bpp;
		modes[num_modes].pitch = target->pitch;
		modes[num_modes].width = target->width;
		modes[num_modes].height = target->height;
		num_modes++;
	}
}
void set_mode(uint8_t mode) {
	regs16_t regs = {bx: modes[mode].mode_id, ax:0x4F02};
	bios_int(0x10, &regs);
	if(regs.ax==0x004F) {
		active_mode = mode;
		frame_buffer = modes[mode].frame_buffer;
		bpp = modes[mode].bpp;
		pitch = modes[mode].pitch;
		width = modes[mode].width;
		height = modes[mode].height;
		buffer_mode = modes[mode].buffer_mode;
		visual_mode = modes[mode].visual_mode;
		if(buffer_mode==banks) {
			if(visual_mode==graphics) frame_buffer = 0x000A0000;
			else frame_buffer = 0x000B8000;
		}
	} else print_str("Error: setting VBE mode\n");
}
void draw_pixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
	uint32_t pixel_offset = y*pitch + (x*(bpp/8));
	frame_buffer[pixel_offset] = b;
	frame_buffer[pixel_offset+1] = g;
	frame_buffer[pixel_offset+2] = b;
}
