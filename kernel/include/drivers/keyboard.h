/*
* Copyright 2014, 2015 Alejandro Bezdjian
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _keyboard_
#define _keyboard_

#define KEYBOARD_BUFFER_SIZE  32

typedef struct{
	unsigned char vec[KEYBOARD_BUFFER_SIZE]; 
	int read;
	int write;
} __attribute__ ((aligned)) keyboard_buffer;

// LED States
#define SET_LED 0xED

#define ESC 27
#define BACKSPACE '\b'
#define TAB '\t'
#define ENTER '\n'

// Non-ASCII special scancodes // Esc in scancode is 1
#define KESC 1
#define KF1 0x3B
#define KF2 (KF1+1)
#define KF3 (KF2+1)
#define KF4 (KF3+1)
#define KF5 (KF4+1)
#define KF6 (KF5+1)
#define KF7 (KF6+1)
#define KF8 (KF7+1)
#define KF9 (KF8+1)
#define KF10 (KF9+1)
#define KF11 (KF10+1)
#define KF12 (KF11+1)
// When one of the above is pressed they will return this ascii
#define F1 200
#define F2 (F1+1)
#define F3 (F2+1)
#define F4 (F3+1)
#define F5 (F4+1)
#define F6 (F5+1)
#define F7 (F6+1)
#define F8 (F7+1)
#define F9 (F8+1)
#define F10 (F9+1)
#define F11 (F10+1)
#define F12 (F11+1)

// Cursor Keys
#define KINS 0x52
#define KDEL 0x53
#define KHOME 0x47
#define KEND 0x4F
#define KPGUP 0x49
#define KPGDN 0x51
#define KLEFT 0x4B
#define KUP 0x48
#define KDOWN 0x50
#define KRIGHT 0x4D
// When one of the above is pressed they will return this ascii
#define INS 220
#define DEL 221
#define HOME 222
#define END 223
#define PGUP 224
#define PGDN 225
#define LEFT 226
#define UP 227
#define DOWN 228
#define RIGHT 229

#define MAKE_SHIFT_L 0x2A
#define MAKE_SHIFT_R 0x36
#define BREAK_SHIFT_L 0xAA
#define BREAK_SHIFT_R 0xB6

#define MAKE_NUM_LOCK 0x45
#define MAKE_CAPS_LOCK 0x3A
#define MAKE_SCROLL_LOCK 0x46
#define BREAK_CAPS_LOCK 0xBA

int init_keyboard(void);
void restart_keyboard(void);
unsigned char get_char_from_keyboard_buffer(void);
void add_to_keyboard_buffer(unsigned char);
unsigned char scanCodeToAscii(int);
int printable(unsigned char);
void turn_on_leds(void);
void turn_off_leds(void);
void turn_on_capsLock(void);
void turn_on_numLock(void);
void turn_on_scrollLock(void);

#endif