#ifndef _keyboard_
#define _keyboard_

#define NOP 0x00
#define ESC 0x01
#define BKSP 0x0E
#define CTRL 0x1D

#define MAKE_SHIFT_L 0x2A
#define MAKE_SHIFT_R 0x36
#define BREAK_SHIFT_L 0xAA
#define BREAK_SHIFT_R 0xB6

#define MAKE_CAPS_LOCK 0x3A
#define BREAK_CAPS_LOCK 0xBA

char scanCodeToAscii(int);
void specialKey(int);
int printable(unsigned char c);
unsigned char get_char_from_keyboard_buffer();
int keyboard_buffer_can_read();
void add_to_keyboard_buffer(unsigned char c);

#endif