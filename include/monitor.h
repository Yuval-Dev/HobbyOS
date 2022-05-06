#ifndef MONITOR_H
#define MONITOR_H
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
extern int cursor_x;
extern int cursor_y;

extern char * hex;

void end_line();

void reset_cursor();

void put_char(char character, char colour);
void put_char(char character);

void print_str(const char* str, char colour);
void print_str(const char* str);

void print_hex(int num, char colour);
void print_hex(int num);

void print_dec(int num, char colour);
void print_dec(int num);

#endif
