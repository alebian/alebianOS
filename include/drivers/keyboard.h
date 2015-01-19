#ifndef _keyboard_
#define _keyboard_

// LED States
#define SET_LED 0xED

#define NOP 0x00
#define ESC 27
#define BACKSPACE '\b'
#define TAB '\t'
#define ENTER '\n'

// Non-ASCII special scancodes // Esc in scancode is 1
#define KESC 1
#define KF1 0x80
#define KF2 (KF1 + 1)
#define KF3 (KF2 + 1)
#define KF4 (KF3 + 1)
#define KF5 (KF4 + 1)
#define KF6 (KF5 + 1)
#define KF7 (KF6 + 1)
#define KF8 (KF7 + 1)
#define KF9 (KF8 + 1)
#define KF10 (KF9 + 1)
#define KF11 (KF10 + 1)
#define KF12 (KF11 + 1)

// Cursor Keys
#define KINS 0x90
#define KDEL (KINS + 1)
#define KHOME (KDEL + 1)
#define KEND (KHOME + 1)
#define KPGUP (KEND + 1)
#define KPGDN (KPGUP + 1)
#define KLEFT (KPGDN + 1)
#define KUP (KLEFT + 1)
#define KDOWN (KUP + 1)
#define KRIGHT (KDOWN + 1)

#define MAKE_SHIFT_L 0x2A
#define MAKE_SHIFT_R 0x36
#define BREAK_SHIFT_L 0xAA
#define BREAK_SHIFT_R 0xB6

#define MAKE_CAPS_LOCK 0x3A
#define BREAK_CAPS_LOCK 0xBA

char scanCodeToAscii(int);
int printable(unsigned char);
void turn_on_leds(void);
void turn_off_leds(void);
void turn_on_capslock(void);

#endif