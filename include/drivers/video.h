#ifndef _video_
#define _video_

#define VGA_PORT 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define TOTAL_CHARS 4000

#define TAB 4;
#define SPACE 1;
#define BACKSPACE -1;

#define CHAR_BLANK ' '

#define CHAR_COLOR_BLACK 0x00
#define CHAR_COLOR_BLUE 0x01
#define CHAR_COLOR_GREEN 0x02
#define CHAR_COLOR_CYAN 0x03
#define CHAR_COLOR_RED 0x04
#define CHAR_COLOR_MAGENTA 0x05
#define CHAR_COLOR_BROWN 0x06
#define CHAR_COLOR_LIGHT_GREY 0x07
#define CHAR_COLOR_DARK_GREY 0x08
#define CHAR_COLOR_LIGHT_BLUE 0x09
#define CHAR_COLOR_LIGHT_GREEN 0x0A
#define CHAR_COLOR_LIGHT_CYAN 0x0B
#define CHAR_COLOR_LIGHT_RED 0x0C
#define CHAR_COLOR_LIGHT_MAGENTA 0x0D
#define CHAR_COLOR_LIGHT_BROWN 0x0E
#define CHAR_COLOR_WHITE 0x0F

#define BACKGROUND_COLOR_BLACK 0x00
#define BACKGROUND_COLOR_BLUE 0x10
#define BACKGROUND_COLOR_GREEN 0x20
#define BACKGROUND_COLOR_CYAN 0x30
#define BACKGROUND_COLOR_RED 0x40
#define BACKGROUND_COLOR_MAGENTA 0x50
#define BACKGROUND_COLOR_BROWN 0x60
#define BACKGROUND_COLOR_LIGHT_GREY 0x70
#define BACKGROUND_COLOR_DARK_GREY 0x80
#define BACKGROUND_COLOR_LIGHT_BLUE 0x90
#define BACKGROUND_COLOR_LIGHT_GREEN 0xA0
#define BACKGROUND_COLOR_LIGHT_CYAN 0xB0
#define BACKGROUND_COLOR_LIGHT_RED 0xC0
#define BACKGROUND_COLOR_LIGHT_MAGENTA 0xD0
#define BACKGROUND_COLOR_LIGHT_BROWN 0xE0
#define BACKGROUND_COLOR_WHITE 0xF0

/* Functions */
void clearScreen();
void scrollScreen();
void changeBackgroundColor(char charcolor, char backcolor);
void printxy(char c, int x, int y);
void print(char c);
void update_cursor(int position);
void scrollup();
void scrolldown();

#endif