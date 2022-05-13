#pragma once
#include<stdint.h>
#define get_macro(_1,_2,_3,_4,_5,_6,_7,_8,name,...) name
#define print_1(arg) _Generic((arg), \
	char : print_char, \
	unsigned char : print_char, \
	char * : print_str, \
	unsigned char * : print_str, \
	long unsigned int: print_hex, \
	unsigned short: print_hex, \
	long int: print_dec, \
	short: print_dec)(arg)
#define print_2(_1, _2) print_1(_1), print_1(_2)
#define print_3(_1, _2, _3) print_2(_1, _2), print_1(_3)
#define print_4(_1, _2, _3, _4) print_3(_1, _2, _3), print_1(_4)
#define print_5(_1, _2, _3, _4, _5) print_4(_1, _2, _3, _4), print1(_5)
#define print_6(_1, _2, _3, _4, _5, _6) print_5(_1, _2, _3, _4, _5), print1(_6)
#define print_7(_1, _2, _3, _4, _5, _6, _7) print_6(_1, _2, _3, _4, _5, _6), print1(_7)
#define print_8(_1, _2, _3, _4, _5, _6, _7, _8) print_7(_1, _2, _3, _4, _5, _6, _7), print1(_8)
#define print(...) get_macro(__VA_ARGS__, print_8, print_7, print_6, print_5, print_4, print_3, print_2, print_1)(__VA_ARGS__)
typedef struct vbe_info {
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
} __attribute__ ((packed)) vbe_info_t;

typedef struct vbe_mode_info {
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
} __attribute__ ((packed)) vbe_mode_info_t;
typedef enum visual_mode {
	text, graphics
} visual_mode_t;
typedef enum buffer_mode {
	banks, linear_frame
} buffer_mode_t;
typedef struct mode_info {
	visual_mode_t visual_mode;
	buffer_mode_t buffer_mode;
	uint16_t pitch, width, height;
	uint8_t bpp;
	uint8_t * frame_buffer;
	uint16_t mode_id;
} mode_info_t;
static vbe_info_t vbe_info;
static mode_info_t modes[256];
void init_vbe();
uint8_t get_orig_mode();
 void set_mode(uint8_t);
static void draw_pixel(uint32_t, uint32_t, uint8_t, uint8_t, uint8_t);
static visual_mode_t visual_mode;
static buffer_mode_t buffer_mode;
static uint16_t pitch, width, height;
static uint8_t bpp;
static uint8_t * frame_buffer;
static uint8_t active_mode;
static uint8_t num_modes = 0;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static uint8_t text_r = 255;
static uint8_t text_g = 255;
static uint8_t text_b = 255;
static uint8_t background_r = 0;
static uint8_t background_g = 0;
static uint8_t background_b = 0;
static uint8_t text_mode_color = 0x07;
static void put_char(uint8_t);
void print_char(char);
void print_str(char*);
void print_hex(uint32_t);
void print_dec(uint32_t);
static void scroll();
