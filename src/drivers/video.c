#include "../../include/drivers/video.h"
#include "../../include/drivers/mouse.h"
#include "../../include/kernel/k_libasm.h"
#include "../../include/kernel/k_libc.h"
#include "../../include/kernel/interrupts.h"
#include "../../include/lib/string.h"
#include "../../include/shell.h"
#include "../../include/lib/stdio.h"

static int cursor_pos = 0;
static int times_scrolled_down = 0;
static int min_row = 0;
static int max_row = 24;
static char background_color = BACKGROUND_COLOR_BLACK;
static char character_color = CHAR_COLOR_LIGHT_GREY;
static char startbar_color = BACKGROUND_COLOR_LIGHT_GREY;
static char mouse_color = BACKGROUND_COLOR_RED+CHAR_COLOR_WHITE;
static char last_mouse_color = BACKGROUND_COLOR_BLACK + CHAR_COLOR_LIGHT_GREY;
static char last_mouse_char = CHAR_BLANK;

void clearScreen(){
	MousePosition mouse_pos = getMousePosition();
	char *vidmem = (char*) VGA_PORT;
	int i = min_row*160;
	int final = (max_row+1)*160;
	eraseMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	while(i < final)
	{
		vidmem[i]=CHAR_BLANK;
		i+=2;
	};
	cursor_pos=min_row*160;
	update_cursor(cursor_pos);
	drawMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	times_scrolled_down = 0;
	return;
}

void clearFullScreen(){
	MousePosition mouse_pos = getMousePosition();
	char *vidmem = (char*) VGA_PORT;
	int i=0;
	eraseMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	while(i < TOTAL_CHARS)
	{
		vidmem[i]=CHAR_BLANK;
		i+=2;
	};
	cursor_pos=0;
	update_cursor(cursor_pos);
	//drawMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	times_scrolled_down = 0;
	return;
}

char getBackgroundColor(){
	return background_color;
}

void setBackgroundColor(char backcolor){
	char *vidmem = (char*) VGA_PORT;
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
	char *vidmem = (char*) VGA_PORT;
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
	character_color = color;
	return;
}

void setAllCharacterColor(char color){
	char *vidmem = (char*) VGA_PORT;
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

void setStartBarColor(char color){
	startbar_color = color;
	return;
}

char getMouseColor(){
	return mouse_color;
}

void setMouseColor(char back, char arrow){
	mouse_color = back+arrow;
	return;
}

void set_vga_size(int min, int max){
	if(min > 0 && max < 26){
		if(min < max){
			min_row = min - 1;
			max_row = max - 1;
			cursor_pos = min_row * 160;
		}
	}
	return;
}

/* Prints a character in a specified VGA screen position without changing cursor */
void printxy(char c, int x, int y){
	char *vidmem = (char*) VGA_PORT;
	MousePosition mouse_pos = getMousePosition();
	eraseMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	vidmem[(y*160)+(x*2)] = c;
	drawMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	return;
}

void print(char c, char color){
	MousePosition mouse_pos = getMousePosition();
	int nextPos, canDelete, aux;
	char *vidmem = (char*) VGA_PORT;
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
	    	    cursor_pos -=2;
	    	    vidmem[cursor_pos] = ' ';
	    	}
	    	break;
	    default:
	    	nextPos = cursor_pos + 1;
	    	if(nextPos >= (max_row+1)*160){
	    	    scrolldown();
	    	    cursor_pos=(max_row)*160;
	    	}
	    	eraseMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	    	vidmem[cursor_pos] = c;
	    	cursor_pos++;
	    	vidmem[cursor_pos] = (vidmem[cursor_pos]-vidmem[cursor_pos]%16) + color;
	    	cursor_pos++;
	    	drawMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	    	break;
	}
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
	MousePosition mouse_pos = getMousePosition();
	char *vidmem = (char*) VGA_PORT;
	char *back = (char*) VGA_BACKUP;
	int i, j, final;
	eraseMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	// Save the first line
	i = min_row*160;
	j = times_scrolled_down*160;
	final = (min_row+1)*160;
	while(i < final){
		back[j++]=vidmem[i++];
	}
	times_scrolled_down++;
	// Scroll the rest of the screen
	i = min_row*160;
	final = max_row*160;
	while(i < final){	
		vidmem[i]= vidmem[i+160];
		i++;
		vidmem[i]= vidmem[i+160];
		i++;
	};
	final = (max_row+1)*160;
	while(i < final){
		vidmem[i]=CHAR_BLANK;
		i++;
		vidmem[i]=background_color+character_color;
		i++;
	};
	cursor_pos-=160;
	update_cursor(cursor_pos);
	drawMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	return;
}

void scrollup(){
	MousePosition mouse_pos = getMousePosition();
	char *vidmem = (char*) VGA_PORT;
	char *back = (char*) VGA_BACKUP;
	int i, j;
	eraseMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	if(times_scrolled_down > 0 && (cursor_pos+160) < max_row*160){
		// Scroll the screen up
		i = ((max_row+1)*160)-1;
		while(i>=(min_row+1)*160){
			vidmem[i] = vidmem[i-160];
			i--;
		}
		// Restores first line from saved
		times_scrolled_down--;
		i = min_row*160;
		j = times_scrolled_down*160;
		while(i<(min_row+1)*160){
			vidmem[i++]=back[j++];
		}
		cursor_pos+=160;
		update_cursor(cursor_pos);
	}
	drawMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	return;
}

void printStartBar(char * startBar){
	MousePosition mouse_pos = getMousePosition();
	char *vidmem = (char*) VGA_PORT;
	int i, j;
	i = j = 0;
	eraseMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	while(j<VGA_WIDTH){
		vidmem[i++] = startBar[j++];
		vidmem[i++] = CHAR_COLOR_GREEN + startbar_color;
	}
	// Gives color to logo
	for (i = 1; i < strlen(START_LOGO)*2; ){
		vidmem[i] = CHAR_COLOR_LIGHT_RED + startbar_color;
		i+=2;
	}
	// Gives color to arrow
	for (i = (strlen(START_LOGO)*2)+1; i < (strlen(START_LOGO)+strlen(ARROW))*2; ){
		vidmem[i] = CHAR_COLOR_LIGHT_BLUE + startbar_color;
		i+=2;
	}
	// Prints line
	i = 160;
	while(i<320){
		vidmem[i++] = '_';
		vidmem[i++] = CHAR_COLOR_BLACK + startbar_color;
	}
	drawMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	return;
}

void drawMouse(int x, int y){
	if(k_isMouseEnabled()){
		char *vidmem = (char*) VGA_PORT;
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

void eraseMouse(int x, int y){
	if(k_isMouseEnabled()){
		char *vidmem = (char*) VGA_PORT;
		int s = (y*160)+(x*2);
		if(s%2==0){
			s++;
		}
		vidmem[s-1] = last_mouse_char;
		vidmem[s] = last_mouse_color;
	}
	return;
}

void MARIO(){
	clearFullScreen();
	setFullBackgroundColor(BACKGROUND_COLOR_BLACK);
	// 1 row
	printSquare(28, 1, 0x44);
	printSquare(32, 1, 0x44);
	printSquare(36, 1, 0x44);
	printSquare(40, 1, 0x44);
	printSquare(44, 1, 0x44);
	// 2 row
	printSquare(24, 3, 0x44);
	printSquare(28, 3, 0x44);
	printSquare(32, 3, 0x44);
	printSquare(36, 3, 0x44);
	printSquare(40, 3, 0x44);
	printSquare(44, 3, 0x44);
	printSquare(48, 3, 0x44);
	printSquare(52, 3, 0x44);
	printSquare(56, 3, 0x44);
	// 3 row
	printSquare(24, 5, 0x66);
	printSquare(28, 5, 0x66);
	printSquare(32, 5, 0x66);
	printSquare(36, 5, 0xFF);
	printSquare(40, 5, 0xFF);
	printSquare(44, 5, 0x00);
	printSquare(48, 5, 0xFF);
	// 4 row
	printSquare(20, 7, 0x66);
	printSquare(24, 7, 0xFF);
	printSquare(28, 7, 0x66);
	printSquare(32, 7, 0xFF);
	printSquare(36, 7, 0xFF);
	printSquare(40, 7, 0xFF);
	printSquare(44, 7, 0x00);
	printSquare(48, 7, 0xFF);
	printSquare(52, 7, 0xFF);
	printSquare(56, 7, 0xFF);
	// 5 row
	printSquare(20, 9, 0x66);
	printSquare(24, 9, 0xFF);
	printSquare(28, 9, 0x66);
	printSquare(32, 9, 0x66);
	printSquare(36, 9, 0xFF);
	printSquare(40, 9, 0xFF);
	printSquare(44, 9, 0xFF);
	printSquare(48, 9, 0x66);
	printSquare(52, 9, 0xFF);
	printSquare(56, 9, 0xFF);
	printSquare(60, 9, 0xFF);
	// 6 row
	printSquare(20, 11, 0x66);
	printSquare(24, 11, 0x66);
	printSquare(28, 11, 0xFF);
	printSquare(32, 11, 0xFF);
	printSquare(36, 11, 0xFF);
	printSquare(40, 11, 0xFF);
	printSquare(44, 11, 0x66);
	printSquare(48, 11, 0x66);
	printSquare(52, 11, 0x66);
	printSquare(56, 11, 0x66);
	// 7 row
	printSquare(28, 13, 0xFF);
	printSquare(32, 13, 0xFF);
	printSquare(36, 13, 0xFF);
	printSquare(40, 13, 0xFF);
	printSquare(44, 13, 0xFF);
	printSquare(48, 13, 0xFF);
	printSquare(52, 13, 0xFF);
	// 8 row
	printSquare(24, 15, 0x11);
	printSquare(28, 15, 0x11);
	printSquare(32, 15, 0x44);
	printSquare(36, 15, 0x11);
	printSquare(40, 15, 0x11);
	printSquare(44, 15, 0x11);
	// 9 row
	printSquare(20, 17, 0x11);
	printSquare(24, 17, 0x11);
	printSquare(28, 17, 0x11);
	printSquare(32, 17, 0x44);
	printSquare(36, 17, 0x11);
	printSquare(40, 17, 0x11);
	printSquare(44, 17, 0x44);
	printSquare(48, 17, 0x11);
	printSquare(52, 17, 0x11);
	printSquare(56, 17, 0x11);
	// 10 row
	printSquare(16, 19, 0x11);
	printSquare(20, 19, 0x11);
	printSquare(24, 19, 0x11);
	printSquare(28, 19, 0x11);
	printSquare(32, 19, 0x44);
	printSquare(36, 19, 0x11);
	printSquare(40, 19, 0x11);
	printSquare(44, 19, 0x44);
	printSquare(48, 19, 0x11);
	printSquare(52, 19, 0x11);
	printSquare(56, 19, 0x11);
	printSquare(60, 19, 0x11);
	// 11 row
	printSquare(16, 21, 0xFF);
	printSquare(20, 21, 0xFF);
	printSquare(24, 21, 0x11);
	printSquare(28, 21, 0x11);
	printSquare(32, 21, 0x44);
	printSquare(36, 21, 0x44);
	printSquare(40, 21, 0x44);
	printSquare(44, 21, 0x44);
	printSquare(48, 21, 0x11);
	printSquare(52, 21, 0x11);
	printSquare(56, 21, 0xFF);
	printSquare(60, 21, 0xFF);
	// 12 row
	printSquare(16, 23, 0xFF);
	printSquare(20, 23, 0xFF);
	printSquare(24, 23, 0xFF);
	printSquare(28, 23, 0x44);
	printSquare(32, 23, 0xEE);
	printSquare(36, 23, 0x44);
	printSquare(40, 23, 0x44);
	printSquare(44, 23, 0xEE);
	printSquare(48, 23, 0x44);
	printSquare(52, 23, 0xFF);
	printSquare(56, 23, 0xFF);
	printSquare(60, 23, 0xFF);
	return;
}

void printSquare(int x, int y, char color){
	char *vidmem = (char*) VGA_PORT;
	int aux = (y*160)+(x*2)+1;
	int i;
	for (i = 0; i < 4; i++){
		vidmem[aux] = color;
		vidmem[aux+160] = color;
		aux+=2;
	}
	return;
}
