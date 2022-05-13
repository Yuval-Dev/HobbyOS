#include"vbe.h"
#include"bios_int.h"
#include"string.h"
static const char * hex = "0123456789abcdef";
extern uint8_t FONT_BEGIN[];
void init_vbe() {
	regs16_t regs = {di:0x2000,ax:0x4F00,es:0x0};
	bios_int(0x10, &regs);
	if(regs.ax!=0x004F) {
		print_str("Error: getting VBE info\n");
	
	}
	vbe_info = *((vbe_info_t*)0x2000);
	uint16_t * vbe_mode_vector = (uint16_t *)(((vbe_info.video_modes>>16)<<4) + vbe_info.video_modes&0xFFFF);
	regs = (regs16_t){ax:0x4F03};
	bios_int(0x10, &regs);
	uint16_t cur_mode_id = regs.bx;
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
		if(modes[num_modes].mode_id==cur_mode_id) {
			active_mode = num_modes;
			frame_buffer = modes[num_modes].frame_buffer;
			bpp = modes[num_modes].bpp;
			pitch = modes[num_modes].pitch;
			width = modes[num_modes].width;
			height = modes[num_modes].height;
			buffer_mode = modes[num_modes].buffer_mode;
			visual_mode = modes[num_modes].visual_mode;
			if(buffer_mode==banks) {
				if(visual_mode==graphics) frame_buffer = 0x000A0000;
				else frame_buffer = 0x000B8000;
			}
		}
		num_modes++;
	}
	for(uint8_t i = 0; i < num_modes; i++) {
		if(modes[i].bpp==24 && modes[i].buffer_mode==linear_frame && modes[i].visual_mode==graphics && modes[i].width==1024) {
			set_mode(i);
			break;
		}
	}
}
static void put_char(uint8_t c) {
	if(visual_mode==text) {
		frame_buffer[2*(cursor_x + cursor_y*width)] = c;
		frame_buffer[(2*(cursor_x + cursor_y*width))+1] = text_mode_color;
	} else {
		for(uint8_t i = 0; i < 16; i++) {
			for(uint8_t j = 0; j < 8; j++) if((FONT_BEGIN[((uint16_t)c)*16+((uint16_t)i)]<<j)&0x80) draw_pixel((cursor_x*8)+j, (cursor_y*16)+i, text_r, text_g, text_b);
		}
	}
}
static void scroll() {
	if(visual_mode==text) {
		memcpy(&frame_buffer[2*width], frame_buffer, 2*width*(height-1));
		memset(&frame_buffer[2*(height-1)*width], 0x00, 2*width);
	} else {
		memcpy(&frame_buffer[16*pitch], frame_buffer, pitch*(height-16));
		memset(&frame_buffer[pitch*(height-16)], 0x00, 16*pitch);
	}
}
void print_char(char c) {
	put_char(c);
	cursor_x++;
	if(visual_mode==text) {
		if(cursor_x==width) {
			cursor_x = 0;
			if(cursor_y==height-1) scroll();
			else cursor_y++;
		}
	} else {
		if((cursor_x+1)*8 > width) {
			cursor_x = 0;
			if((cursor_y+2)*16 > height) scroll();
			else cursor_y++;
		}
	}
}
void print_str(char* s) {
	uint32_t i = 0;
	while(s[i]) print_char(s[i++]);
}
void print_hex(uint32_t num) {
	print_str("0x");
	for(int i = 7; i >= 0; i--) {
		print_char(hex[(num>>(i*4))&0xF]);
	}
}
void print_dec(uint32_t) {

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
		cursor_x = 0;
		cursor_y = 0;
	} else print_str("Error: setting VBE mode\n");
}
void draw_pixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
	if(bpp==24 || bpp==32) {
		uint32_t pixel_offset = y*pitch + (x*(bpp/8));
		frame_buffer[pixel_offset] = b;
		frame_buffer[pixel_offset+1] = g;
		frame_buffer[pixel_offset+2] = b;
	}
}
