#include"monitor.h"
int cursor_x = 0;
int cursor_y = 0;

char * hex = "0123456789ABCDEF";//{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void end_line() {
	cursor_x = 0;
	cursor_y = (cursor_y+1)%25;
}

void reset_cursor() {
	cursor_x = 0;
	cursor_y = 0;
}

void put_char(char character, char colour) {
	*(short*)(0xb8000+cursor_x*2+cursor_y*160) = (colour << 8) + character;
	cursor_x++;
	if(cursor_x==80) {
		cursor_x = 0;
		cursor_y++;
	}
	if(cursor_y==25) cursor_y=0;
}

void put_char(char character) {
	put_char(character, DEFAULT_COLOR);
}

void print_str(const char* str, char colour) {
	int i = 0;
	while(str[i]!=0) put_char(str[i++], colour);
}

void print_str(const char * str) {
	print_str(str, DEFAULT_COLOR);
}

void print_hex(uint32_t num, char colour) {
	print_str("0x", colour);
	for(int i = 7; i >= 0; i--) {
		put_char(hex[(num>>(i*4))&0xF], colour);
	}
}

void print_hex(uint32_t num) {
	print_hex(num, DEFAULT_COLOR);
}
void print_dec(int num, char colour) {
	if(num) {
		print_dec(num/10, colour);
		put_char(hex[num%10], colour);
	}
}
void print_dec(int num) {
	print_dec(num, DEFAULT_COLOR);
}
