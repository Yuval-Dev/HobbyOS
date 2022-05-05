#define COLOR_BLACK 0x0
#define COLOR_BLUE 0x1
#define COLOR_GREEN 0x2
#define COLOR_CYAN 0x3
#define COLOR_RED 0x4
#define COLOR_MAGENTA 0x5
#define COLOR_YELLOW 0x6
#define COLOR_WHITE 0x7
#define COLOR_GREY 0x8
#define COLOR_BRIGHT_BLUE 0x9
#define COLOR_BRIGHT_GREEN 0xA
#define COLOR_BRIGHT_CYAN 0xB
#define COLOR_BRIGHT_MAGENTA 0xD
#define COLOR_BRIGHT_YELLOW 0xE
#define COLOR_BRIGHT_WHITE 0xF
#define BLINKING_TEXT 0x80
#define NORMAL_TEXT 0x00
#define COLOR(foreground, background, blinking) ((((background & 0x7) >> 4) + foreground) | blinking)
#define DEFAULT_COLOR COLOR(COLOR_GREEN,COLOR_BLACK,NORMAL_TEXT)
int cursor_x = 0;
int cursor_y = 0;

char hex[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void end_line() {
	cursor_x = 0;
	cursor_y = (cursor_y+1)%25;
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

void print_str(const char* str, char colour) {
	int i = 0;
	while(str[i]!=0) put_char(str[i++], colour);
}

void print_hex(int num, char colour) {
	print_str("0x", colour);
	for(int i = 7; i >= 0; i--) {
		put_char(hex[(num>>(i*4))%16], colour);
	}
}
