#include "../../include/drivers/video.h"
#include "../../include/drivers/mouse.h"
#include "../../include/arch/x86/x86.h"
#include "../../include/kernel/kernel.h"
#include "../../include/kernel/interrupts.h"
#include "../../include/lib/string.h"
#include "../../include/lib/stdio.h"
#include "../../include/programs/shell.h"

static char* vidmem = (char*) VGA_PORT;
static int cursor_pos = 0;
static int min_row = 0;
static int max_row = 24;
static char background_color = BACKGROUND_COLOR_BLACK;
static char character_color = CHAR_COLOR_LIGHT_GREY;
static char startbar_color = START_BAR_COLOR;
static char mouse_color = BACKGROUND_COLOR_RED+CHAR_COLOR_WHITE;
static char last_mouse_color = BACKGROUND_COLOR_BLACK + CHAR_COLOR_LIGHT_GREY;
static char last_mouse_char = CHAR_BLANK;
static startmenu start_menu;
static shell_file shellfile;

void clearScreen(){
	MousePosition mouse_pos = getMousePosition();
	int i = min_row*160;
	int final = (max_row+1)*160;
	eraseMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	while(i < final){
		vidmem[i]=CHAR_BLANK;
		i+=2;
	};
	cursor_pos=min_row*160;
	update_cursor(cursor_pos);
	drawMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	return;
}

void clearLine(int x){
	MousePosition mouse_pos = getMousePosition();
	eraseMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	int i = (x-1)*160;
	int final = x*160;
	while(i < final){
		vidmem[i]=CHAR_BLANK;
		i+=2;
	}
	update_cursor(cursor_pos);
	drawMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	return;
}

void clearCurrentLine(){
	clearLine((int)cursor_pos/160);
	cursor_pos = cursor_pos - (cursor_pos%160);
}

void clearFullScreen(){
	MousePosition mouse_pos = getMousePosition();
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
	return;
}

char getBackgroundColor(){
	return background_color;
}

void setBackgroundColor(char backcolor){
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
	setStartMenuColor(color);
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
	MousePosition mouse_pos = getMousePosition();
	eraseMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	vidmem[(y*160)+(x*2)] = c;
	drawMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	return;
}

void printxyc(char c, char color, int x, int y){
	MousePosition mouse_pos = getMousePosition();
	eraseMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	vidmem[(y*160)+(x*2)] = c;
	vidmem[(y*160)+(x*2)+1] = color;
	drawMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	return;
}

void print(char c, char color){
	MousePosition mouse_pos = getMousePosition();
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

/*
 * Prints on the entire available screen what is stored
 * in the file and from the line specified.
 * If color is set, it means that the file has one byte for
 * the printable character and the next byte is the color.
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
	MousePosition mouse_pos = getMousePosition();
	int i, final;
	/* If we are not in the first printable line */
	if( (min_row*160) != (cursor_pos-(cursor_pos%160)) ){
		eraseMouse(mouse_pos.actual_x, mouse_pos.actual_y);
		// Scroll the printable screen
		i = min_row*160;
		final = max_row*160;
		// Save first row in shell file
		if(k_isShellEnabled()){
			save_shell_row(vidmem+i);
		}
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
		drawMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	}
	return;
}

void scrollup(){
	MousePosition mouse_pos = getMousePosition();
	int i, j, k, final;
	if(shellfile.scroll_downs > 0){
		i = (min_row+1)*160;
		final = max_row*160;
		// Save last row in shell file
		while(i < final){	
			vidmem[i] = vidmem[i-160];
			i++;
			vidmem[i] = vidmem[i-160];
			i++;
		};
		j = shellfile.writepos%160;
		i = j-1;
		final = shellfile.writepos;
		k = final - 160;
		j = min_row*160;
		while(i < final){	
			vidmem[j++] = shellfile.init[k++];
			vidmem[j++] = shellfile.init[k++];
		};
		cursor_pos+=160;
		update_cursor(cursor_pos);
		drawMouse(mouse_pos.actual_x, mouse_pos.actual_y);
	}
	return;
}

void printStartBar(char* startBar){
	MousePosition mouse_pos = getMousePosition();
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
	int aux = (y*160)+(x*2)+1;
	int i;
	for (i = 0; i < 4; i++){
		vidmem[aux] = color;
		vidmem[aux+160] = color;
		aux+=2;
	}
	return;
}

void setSTARTMENU(){
	/* Shutdown   | */
	/* Reboot     | */
	/* Logout     | */
	/* Theme: 0000| */
	/* ------------ */
	memcpy(start_menu.firstline,  "SShhuuttddoowwnn      ||", START_MENU_SIZE);
	memcpy(start_menu.secondline, "RReebboooott          ||", START_MENU_SIZE);
	memcpy(start_menu.thirdline,  "LLooggoouutt          ||", START_MENU_SIZE);
	memcpy(start_menu.fourthline, "TThheemmee::          ||", START_MENU_SIZE);
	memcpy(start_menu.fifthline,  "------------------------", START_MENU_SIZE);
	setStartMenuColor(startbar_color);
	return;
}

void setStartMenuColor(char color){
	char menucharcolor;
	if(color == BACKGROUND_COLOR_LIGHT_GREY){
		menucharcolor = CHAR_COLOR_BLACK;
	}
	if(color == BACKGROUND_COLOR_RED){
		menucharcolor = CHAR_COLOR_WHITE;
	}
	if(color == BACKGROUND_COLOR_BLUE){
		menucharcolor = CHAR_COLOR_WHITE;
	}
	int i;
	for(i = 1; i < START_MENU_SIZE ; i+=2){
		start_menu.firstline[i] = color + menucharcolor;
		start_menu.secondline[i] = color + menucharcolor;
		start_menu.thirdline[i] = color + menucharcolor;
		start_menu.fourthline[i] = color + menucharcolor;
		start_menu.fifthline[i] = color + menucharcolor;
	}
	start_menu.fourthline[15] = BACKGROUND_COLOR_BLACK;
	start_menu.fourthline[17] = BACKGROUND_COLOR_LIGHT_GREY;
	start_menu.fourthline[19] = BACKGROUND_COLOR_RED;
	start_menu.fourthline[21] = BACKGROUND_COLOR_BLUE;
	return;
}

void printSTARTMENU(){
	/* Save first */
	memcpy(start_menu.savedfirstline, (char*)VGA_PORT+(160*2), START_MENU_SIZE);
	memcpy(start_menu.savedsecondline, (char*)VGA_PORT+(160*3), START_MENU_SIZE);
	memcpy(start_menu.savedthirdline, (char*)VGA_PORT+(160*4), START_MENU_SIZE);
	memcpy(start_menu.savedfourthline, (char*)VGA_PORT+(160*5), START_MENU_SIZE);
	memcpy(start_menu.savedfifthline, (char*)VGA_PORT+(160*6), START_MENU_SIZE);
	/* Then print */
	memcpy((char*)VGA_PORT+(160*2), start_menu.firstline, START_MENU_SIZE);
	memcpy((char*)VGA_PORT+(160*3), start_menu.secondline, START_MENU_SIZE);
	memcpy((char*)VGA_PORT+(160*4), start_menu.thirdline, START_MENU_SIZE);
	memcpy((char*)VGA_PORT+(160*5), start_menu.fourthline, START_MENU_SIZE);
	memcpy((char*)VGA_PORT+(160*6), start_menu.fifthline, START_MENU_SIZE);
	return;
}

void clearSTARTMENU(){
	memcpy((char*)VGA_PORT+(160*2), start_menu.savedfirstline, START_MENU_SIZE);
	memcpy((char*)VGA_PORT+(160*3), start_menu.savedsecondline, START_MENU_SIZE);
	memcpy((char*)VGA_PORT+(160*4), start_menu.savedthirdline, START_MENU_SIZE);
	memcpy((char*)VGA_PORT+(160*5), start_menu.savedfourthline, START_MENU_SIZE);
	memcpy((char*)VGA_PORT+(160*6), start_menu.savedfifthline, START_MENU_SIZE);
	return;
}

void init_shell_file(){
	/******************************************************/
	/* WE SHOULD USE MALLOC BUT IT IS NOT IMPLEMENTED YET */
	shellfile.init = (char*) 0x300000;
	shellfile.limit = 0x100000;
	/******************************************************/
	reset_shell_file();
	return;
}

void reset_shell_file(){
	memset(shellfile.init, 0, shellfile.limit);
	shellfile.writepos = 0;
	shellfile.scroll_downs = 0;
	return;
}

void save_shell_row(char* from){
	memcpy((char*)from, shellfile.init+shellfile.writepos, 160);
	shellfile.writepos+=160;
	shellfile.scroll_downs++;
	return;
}
