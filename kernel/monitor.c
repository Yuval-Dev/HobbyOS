#include"monitor.h"
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static char color = DEFAULT_COLOR;
static const char * hex = "0123456789abcdef";
static uint16_t * text_buffer = 0x00B8000;

void set_color(char col) {
	color = col;
}

void put_char(char character) {
	switch(character) {
		case '\b':
			if(cursor_x==0) {
				cursor_x = 79;
				if(cursor_y>0) cursor_y--;
			} else cursor_x--;
			break;
		case '\t':
			cursor_x -= (cursor_x&3) - 4;
			if(cursor_x==80) {
				cursor_x = 0;
				if(cursor_y < 24) cursor_y++;
			}
			break;
		case '\n':
			cursor_x = 0;
			if(cursor_y < 24) cursor_y++;
			break;
		case '\v':
			if(cursor_y < 24) cursor_y++;
			break;
		case '\r':
			cursor_x = 0;
			break;
		default:
			text_buffer[cursor_x+cursor_y*80] = (color << 8) + character;
			cursor_x = (cursor_x+1)%80;
			if(cursor_x == 0 && cursor_y < 24) cursor_y++;
	}
}

void print_str(const char * str) {
	uint32_t i = 0;
	while(str[i]) put_char(str[i++]);
}

void print_hex(uint32_t num) {
	print_str("0x");
	for(int i = 7; i >= 0; i--) {
		put_char(hex[(num>>(i*4))&0xF]);
	}

}
void print_dec(int num) {
	if(num) {
		if(num/10)
		print_dec(num/10);
		put_char(hex[num%10]);
	} else put_char('0');
}
