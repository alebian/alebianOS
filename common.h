#ifndef _common_
#define _common_

#define OS_VERSION "0.2"
#define OS_NAME "alebianOS"
#define START_LOGO "aOS"

#define TRUE    1
#define FALSE   0
#define EOF     -1
#define NULL    0

/* Video constants */
#define S_CHAR_COLOR_BLACK 0x00
#define S_CHAR_COLOR_BLUE 0x01
#define S_CHAR_COLOR_GREEN 0x02
#define S_CHAR_COLOR_CYAN 0x03
#define S_CHAR_COLOR_RED 0x04
#define S_CHAR_COLOR_MAGENTA 0x05
#define S_CHAR_COLOR_BROWN 0x06
#define S_CHAR_COLOR_LIGHT_GREY 0x07
#define S_CHAR_COLOR_DARK_GREY 0x08
#define S_CHAR_COLOR_LIGHT_BLUE 0x09
#define S_CHAR_COLOR_LIGHT_GREEN 0x0A
#define S_CHAR_COLOR_LIGHT_CYAN 0x0B
#define S_CHAR_COLOR_LIGHT_RED 0x0C
#define S_CHAR_COLOR_LIGHT_MAGENTA 0x0D
#define S_CHAR_COLOR_LIGHT_BROWN 0x0E
#define S_CHAR_COLOR_WHITE 0x0F
#define S_BACKGROUND_COLOR_BLACK 0x00
#define S_BACKGROUND_COLOR_BLUE 0x10
#define S_BACKGROUND_COLOR_GREEN 0x20
#define S_BACKGROUND_COLOR_CYAN 0x30
#define S_BACKGROUND_COLOR_RED 0x40
#define S_BACKGROUND_COLOR_MAGENTA 0x50
#define S_BACKGROUND_COLOR_BROWN 0x60
#define S_BACKGROUND_COLOR_LIGHT_GREY 0x70
#define S_BACKGROUND_COLOR_DARK_GREY 0x80
#define S_BACKGROUND_COLOR_LIGHT_BLUE 0x90
#define S_BACKGROUND_COLOR_LIGHT_GREEN 0xA0
#define S_BACKGROUND_COLOR_LIGHT_CYAN 0xB0
#define S_BACKGROUND_COLOR_LIGHT_RED 0xC0
#define S_BACKGROUND_COLOR_LIGHT_MAGENTA 0xD0
#define S_BACKGROUND_COLOR_LIGHT_BROWN 0xE0
#define S_BACKGROUND_COLOR_WHITE 0xF0

/* Keyboard constants */
#define S_F1 200
#define S_F2 (F1+1)
#define S_F3 (F2+1)
#define S_F4 (F3+1)
#define S_F5 (F4+1)
#define S_F6 (F5+1)
#define S_F7 (F6+1)
#define S_F8 (F7+1)
#define S_F9 (F8+1)
#define S_F10 (F9+1)
#define S_F11 (F10+1)
#define S_F12 (F11+1)
#define S_KINS 0x52
#define S_KDEL 0x53
#define S_KHOME 0x47
#define S_KEND 0x4F
#define S_KPGUP 0x49
#define S_KPGDN 0x51
#define S_KLEFT 0x4B
#define S_KUP 0x48
#define S_KDOWN 0x50
#define S_KRIGHT 0x4D
#define S_INS 220
#define S_DEL 221
#define S_HOME 222
#define S_END 223
#define S_PGUP 224
#define S_PGDN 225
#define S_LEFT 226
#define S_UP 227
#define S_DOWN 228
#define S_RIGHT 229

typedef char 		   sint8_t;
typedef unsigned char  uint8_t;
typedef short int	   sint16_t;
typedef unsigned short uint16_t;
typedef int 		   sint32_t;
typedef unsigned int   uint32_t;
typedef long 		   sint64_t;
typedef unsigned long  uint64_t;

#endif