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

#ifndef _video_
#define _video_

#include "../main.h"

#define VGA_PORT 0xB8000
#define VGA_BACKUP 0x200000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define TOTAL_CHARS 4000 // VGA_WIDTH*VGA_HEIGHT*2

#define V_TAB 6

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

/* Functions*/
void clearScreen(void); // Clears chars from screen between rows set by set_vga_size
void clearLine(int); // Clears row selected by parameter (1 to 25)
void clearCurrentLine(void);
void clearFullScreen(void); // Clears all chars from screen
char getBackgroundColor(void); // Returns the current background color between rows set by set_vga_size
void setBackgroundColor(char); // Changes the background between rows set by set_vga_size
void setFullBackgroundColor(char); // Changes the entire screen background
char getCharacterColor(void); // Returns the current char color between rows set by set_vga_size
void setCharacterColor(char); // Changes the color that print will use next
void setAllCharacterColor(char); // Changes all chars color between rows set by set_vga_size
char getMouseColor(void);
void setMouseColor(char, char);
void set_vga_size(int, int); // Changes the screen size that other functions can affect
void printxy(char, int, int);
void printxyc(char, char, int, int);
void print(char);
void printWithColor(char, char);
void printFromFile(char*, int, int);
void move_cursor_back(void);
void move_cursor_forward(void);
void update_cursor(int);
void scrolldown(void);
void scrollup(void);
void printLine(int, char*);
void draw_mouse(void);
void erase_mouse(void);
void printSquare(int, int, char);
void print_start_menu(startmenu*);
void clear_start_menu(startmenu*);

#endif