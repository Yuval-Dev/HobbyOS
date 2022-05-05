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

int cursor_x = 0;
int cursor_y = 0;

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
