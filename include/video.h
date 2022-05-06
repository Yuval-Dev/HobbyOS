#ifndef VIDEO_H
#define VIDEO_H
#include"uint.h"
struct vbe_info_structure {
	char signature[4];
	uint16_t version;	
	uint32_t oem;
	uint32_t capabilities;
	uint32_t video_modes;
	uint16_t video_memory;
	uint16_t software_rev;
	uint32_t vendor;
	uint32_t product_name;
	uint32_t product_rev;
	char reserved[222];
	char oem_data[256];
} __attribute__ ((packed));

struct vbe_mode_info_structure {
	uint16_t attributes;
	uint8_t window_a;
	uint8_t window_b;
	uint16_t granularity;
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;
	uint16_t pitch;
	uint16_t width;
	uint16_t height;
	uint8_t w_char;
	uint8_t y_char;
	uint8_t planes;
	uint8_t bpp;
	uint8_t banks;
	uint8_t memory_model;
	uint8_t bank_size;
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;
	uint8_t reserved1[206];
} __attribute__ ((packed));

struct display_info {
	bool graphics_mode;
	bool linear_frame_buffer;
	uint16_t pitch;
	uint16_t width;
	uint16_t height;
	uint8_t bpp;
	uint8_t * frame_buffer;
	uint16_t mode_id;
};

bool get_vbe_mode_info(vbe_mode_info_structure *, uint16_t mode);
vbe_mode_info_structure get_vbe_mode_info(uint16_t mode);
bool get_vbe_info(vbe_info_structure *);
vbe_info_structure get_vbe_info();
display_info get_display_info(uint16_t mode);
void dump_mode_info(vbe_mode_info_structure * structure);
void init_video_driver();
uint16_t get_current_mode();
void set_current_mode(uint8_t);
void dump_mode_info(display_info structure);
#endif
