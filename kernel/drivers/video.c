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

#include "../include/system.h"

static char* vidmem = (char*) VGA_PORT;
static int cursor_pos = 0;
static int min_row = 0;
static int max_row = 24;
static char background_color = BACKGROUND_COLOR_BLACK;
static char character_color = CHAR_COLOR_LIGHT_GREY;
static char mouse_color = BACKGROUND_COLOR_BLACK+CHAR_COLOR_LIGHT_GREY;
static char last_mouse_color = BACKGROUND_COLOR_BLACK + CHAR_COLOR_LIGHT_GREY;
static char last_mouse_char = CHAR_BLANK;

void clearScreen(){
	int i = min_row*160;
	int final = (max_row+1)*160;
	erase_mouse();
	while(i < final){
		vidmem[i]=CHAR_BLANK;
		i+=2;
	};
	cursor_pos=min_row*160;
	update_cursor(cursor_pos);
	draw_mouse();
	return;
}

void clearLine(int x){
	erase_mouse();
	int i = (x-1)*160;
	int final = x*160;
	while(i < final){
		vidmem[i]=CHAR_BLANK;
		i+=2;
	}
	update_cursor(cursor_pos);
	draw_mouse();
	return;
}

void clearCurrentLine(){
	clearLine((int)cursor_pos/160);
	cursor_pos = cursor_pos - (cursor_pos%160);
}

void clearFullScreen(){
	int i=0;
	erase_mouse();
	while(i < TOTAL_CHARS)
	{
		vidmem[i]=CHAR_BLANK;
		i+=2;
	};
	cursor_pos=0;
	update_cursor(cursor_pos);
	draw_mouse();
	return;
}

char getBackgroundColor(){
	return background_color;
}

void setBackgroundColor(char backcolor){
	backcolor = SECOND_BYTE(backcolor);
	int i = (min_row*160)+1;
	int final = (max_row+1)*160;
	char old;
	while(i < final){
		old = vidmem[i] % 16;
		vidmem[i] = backcolor + old;
		i+=2;
	};
	background_color = backcolor;
	last_mouse_color = backcolor;
	return;
}

void setFullBackgroundColor(char backcolor){
	backcolor = SECOND_BYTE(backcolor);
	int i = 1;
	char old;
	while(i < TOTAL_CHARS){
		old = vidmem[i] % 16;
		vidmem[i] = backcolor + old;
		i+=2;
	};
	background_color = backcolor;
	last_mouse_color = backcolor;
	return;
}

char getCharacterColor(){
	return character_color;
}

void setCharacterColor(char color){
	character_color = FIRST_BYTE(color);
	return;
}

void setAllCharacterColor(char color){
	color = FIRST_BYTE(color);
	int i = (min_row*160)+1;
	int final = (max_row+1)*160;
	while(i < final){
		if(vidmem[i] == background_color+character_color){
			vidmem[i] = background_color + color;
		}
		i+=2;
	};
	character_color = color;
	return;
}

char getMouseColor(){
	return mouse_color;
}

void setMouseColor(char back, char arrow){
	back = SECOND_BYTE(back);
	arrow = FIRST_BYTE(arrow);
	mouse_color = back + arrow;
	return;
}

void set_vga_size(int min, int max){
	if(min > 0 && max < 26){
		if(min < max){
			min_row = min - 1;
			max_row = max - 1;
			cursor_pos = min_row * 160;
			update_cursor(cursor_pos);
		}
	}
	return;
}

/* Prints a character in a specified VGA screen position without changing cursor */
void printxy(char c, int x, int y){
	printxyc(c, getCharacterColor(), x, y);
	return;
}

void printxyc(char c, char color, int x, int y){
	erase_mouse();
	vidmem[(y*160)+(x*2)] = c;
	vidmem[(y*160)+(x*2)+1] = color;
	draw_mouse();
	return;
}

void print(char c){
	printWithColor(c, getCharacterColor());
	return;
}

void printWithColor(char c, char color){
	int nextPos, canDelete, aux;
	switch(c){
	    case '\t':
	    	nextPos = cursor_pos + V_TAB;
	    	if(nextPos >= (max_row+1)*160){
	    		aux = (max_row+1)*160 - cursor_pos;
	    	    scrolldown();
	    	    cursor_pos = max_row*160;
	    	    cursor_pos += (V_TAB-aux);
	    	}else{
	    		cursor_pos += V_TAB;
	    	}
	    	break;
	    case '\n':
	    	nextPos = cursor_pos + 160 - (cursor_pos%160);
	    	if(nextPos >= (max_row+1)*160){
	    	    scrolldown();
	    	    cursor_pos=(max_row)*160;
	    	} else {
	    		cursor_pos += 160 - cursor_pos%160;
	    	}
	    	break;
	    case '\b':
	    	canDelete=0;
	    	if(cursor_pos>0 && cursor_pos<=TOTAL_CHARS){
	    		canDelete=1;
	    	}
	    	if(canDelete){            
	    	    move_cursor_back();
	    	    vidmem[cursor_pos] = ' ';
	    	}
	    	return;
	    default:
	    	nextPos = cursor_pos + 1;
	    	if(nextPos >= (max_row+1)*160){
	    	    scrolldown();
	    	    cursor_pos=(max_row)*160;
	    	}
	    	erase_mouse();
	    	vidmem[cursor_pos] = c;
	    	cursor_pos++;
	    	vidmem[cursor_pos] = (vidmem[cursor_pos]-vidmem[cursor_pos]%16) + color;
	    	cursor_pos++;
	    	draw_mouse();
	    	break;
	}
	update_cursor(cursor_pos);
	return;
}

/*
 * Prints on the entire available screen what is stored
 * in the file and from the line specified.
 * If color is set, it means that the file has one uint8_t for
 * the printable character and the next uint8_t is the color.
 * The 0 line is the one taht begins at file[0].
 */
void printFromFile(char* file, int lineFrom, int color){
	clearScreen();
	int v_start = min_row*160;
	int v_final = (max_row+1)*160;
	int v_current = v_start;
	int f_start;
	int f_current;
	if(color){
		f_start = lineFrom*160;
	}else{
		f_start = lineFrom*80;
	}
	f_current = f_start;
	while(v_current<v_final){
		vidmem[v_current++] = file[f_current++];
		if(color){
			vidmem[v_current++] = file[f_current++];
		}else{
			vidmem[v_current++] = character_color+background_color;
		}
	}
	return;
}

void move_cursor_back(){
	cursor_pos -=2;
	update_cursor(cursor_pos);
	return;
}

void move_cursor_forward(){
	cursor_pos +=2;
	update_cursor(cursor_pos);
	return;
}

void update_cursor(int position){
	unsigned short aux = position;
	aux = aux/2;
	_outb(0x3D4, 0x0F); 
	_outb(0x3D5, (unsigned char)aux); 
	_outb(0x3D4, 0x0E); 
	_outb(0x3D5, (unsigned char )((aux>>8)));
}

void scrolldown(){
	int i, final;
	/* If we are not in the first printable line */
	if( (min_row*160) != (cursor_pos-(cursor_pos%160)) ){
		erase_mouse();
		// Scroll the printable screen
		i = min_row*160;
		final = max_row*160;
		while(i < final){	
			vidmem[i]= vidmem[i+160];
			i++;
			vidmem[i]= vidmem[i+160];
			i++;
		};
		final = (max_row+1)*160;
		// Clears the last row
		while(i < final){
			vidmem[i]=CHAR_BLANK;
			i++;
			vidmem[i]=background_color+character_color;
			i++;
		};
		cursor_pos-=160;
		update_cursor(cursor_pos);
		draw_mouse();
	}
	return;
}

void scrollup(){
	return;
}

void printLine(int x, char* line){
	erase_mouse();
	int j = 0;
	int i = (x-1)*160;
	int final = x*160;
	while(i < final){
		vidmem[i] = line[j];
		i+=2;
		j++;
	}
	draw_mouse();
	return;
}

void draw_mouse(){
	if(k_isMouseEnabled()){
		MousePosition mouse_pos = getMousePosition();
		int x = mouse_pos.actual_x;
		int y = mouse_pos.actual_y;
		int s = (y*160)+(x*2);
		if(s%2==0){
			s++;
		}
		last_mouse_char = vidmem[s-1];
		last_mouse_color = vidmem[s];
		vidmem[s-1] = '+';
		vidmem[s] = mouse_color;
	}
	return;
}

void erase_mouse(){
	if(k_isMouseEnabled()){
		MousePosition mouse_pos = getMousePosition();
		int x = mouse_pos.actual_x;
		int y = mouse_pos.actual_y;
		int s = (y*160)+(x*2);
		if(s%2==0){
			s++;
		}
		vidmem[s-1] = last_mouse_char;
		vidmem[s] = last_mouse_color;
	}
	return;
}

void printSquare(int x, int y, char color){
	int aux = (y*160)+(x*2)+1;
	int i;
	for (i = 0; i < 4; i++){
		vidmem[aux] = color;
		vidmem[aux+160] = color;
		aux+=2;
	}
	return;
}

void print_start_menu(startmenu* start_menu){
	/* Save first */
	k_memcpy(&start_menu->savedfirstline, (char*)VGA_PORT+(160*2), START_MENU_SIZE);
	k_memcpy(&start_menu->savedsecondline, (char*)VGA_PORT+(160*3), START_MENU_SIZE);
	k_memcpy(&start_menu->savedthirdline, (char*)VGA_PORT+(160*4), START_MENU_SIZE);
	// /* Then print */
	k_memcpy((char*)VGA_PORT+(160*2), &start_menu->firstline, START_MENU_SIZE);
	k_memcpy((char*)VGA_PORT+(160*3), &start_menu->secondline, START_MENU_SIZE);
	k_memcpy((char*)VGA_PORT+(160*4), &start_menu->thirdline, START_MENU_SIZE);
	return;
}

void clear_start_menu(startmenu* start_menu){
	k_memcpy((char*)VGA_PORT+(160*2), &start_menu->savedfirstline, START_MENU_SIZE);
	k_memcpy((char*)VGA_PORT+(160*3), &start_menu->savedsecondline, START_MENU_SIZE);
	k_memcpy((char*)VGA_PORT+(160*4), &start_menu->savedthirdline, START_MENU_SIZE);
	return;
}
