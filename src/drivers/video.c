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

void clearScreen(){
	MousePosition mouse_pos = getMousePosition();
	int i = min_row*160;
	int final = (max_row+1)*160;
	erase_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
	while(i < final){
		vidmem[i]=CHAR_BLANK;
		i+=2;
	};
	cursor_pos=min_row*160;
	update_cursor(cursor_pos);
	draw_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
	return;
}

void clearLine(int x){
	MousePosition mouse_pos = getMousePosition();
	erase_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
	int i = (x-1)*160;
	int final = x*160;
	while(i < final){
		vidmem[i]=CHAR_BLANK;
		i+=2;
	}
	update_cursor(cursor_pos);
	draw_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
	return;
}

void clearCurrentLine(){
	clearLine((int)cursor_pos/160);
	cursor_pos = cursor_pos - (cursor_pos%160);
}

void clearFullScreen(){
	MousePosition mouse_pos = getMousePosition();
	int i=0;
	erase_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
	while(i < TOTAL_CHARS)
	{
		vidmem[i]=CHAR_BLANK;
		i+=2;
	};
	cursor_pos=0;
	update_cursor(cursor_pos);
	//draw_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
	return;
}

char getBackgroundColor(){
	return background_color;
}

void setBackgroundColor(char backcolor){
	backcolor = SECONDBYTE(backcolor);
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
	backcolor = SECONDBYTE(backcolor);
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
	character_color = FIRSTBYTE(color);
	return;
}

void setAllCharacterColor(char color){
	color = FIRSTBYTE(color);
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
	set_start_menuColor(color);
	return;
}

char getMouseColor(){
	return mouse_color;
}

void setMouseColor(char back, char arrow){
	back = SECONDBYTE(back);
	arrow = FIRSTBYTE(arrow);
	mouse_color = back + arrow;
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
	erase_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
	vidmem[(y*160)+(x*2)] = c;
	draw_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
	return;
}

void printxyc(char c, char color, int x, int y){
	MousePosition mouse_pos = getMousePosition();
	erase_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
	vidmem[(y*160)+(x*2)] = c;
	vidmem[(y*160)+(x*2)+1] = color;
	draw_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
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
	    	erase_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
	    	vidmem[cursor_pos] = c;
	    	cursor_pos++;
	    	vidmem[cursor_pos] = (vidmem[cursor_pos]-vidmem[cursor_pos]%16) + color;
	    	cursor_pos++;
	    	draw_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
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
		erase_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
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
		draw_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
	}
	return;
}

void scrollup(){
	return;
}

void printStartBar(char* startBar){
	MousePosition mouse_pos = getMousePosition();
	int i, j;
	i = j = 0;
	erase_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
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
	draw_mouse(mouse_pos.actual_x, mouse_pos.actual_y);
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

void set_start_menu(){
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
	set_start_menuColor(startbar_color);
	return;
}

void set_start_menuColor(char color){
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

void print_start_menu(){
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

void clear_start_menu(){
	memcpy((char*)VGA_PORT+(160*2), start_menu.savedfirstline, START_MENU_SIZE);
	memcpy((char*)VGA_PORT+(160*3), start_menu.savedsecondline, START_MENU_SIZE);
	memcpy((char*)VGA_PORT+(160*4), start_menu.savedthirdline, START_MENU_SIZE);
	memcpy((char*)VGA_PORT+(160*5), start_menu.savedfourthline, START_MENU_SIZE);
	memcpy((char*)VGA_PORT+(160*6), start_menu.savedfifthline, START_MENU_SIZE);
	return;
}

void switch_VGA_style(int vga_style){
	switch(vga_style){
		case 0:
			set_VGA_style(BACKGROUND_COLOR_LIGHT_GREY, CHAR_COLOR_BLACK, BACKGROUND_COLOR_BLACK, CHAR_COLOR_LIGHT_GREY, BACKGROUND_COLOR_LIGHT_GREY);
			vga_style++;
			break;
		case 1:
			set_VGA_style(BACKGROUND_COLOR_BLUE, CHAR_COLOR_WHITE, BACKGROUND_COLOR_BLACK, CHAR_COLOR_RED, BACKGROUND_COLOR_BLUE);
			vga_style++;
			break;
		case 2:
			set_VGA_style(BACKGROUND_COLOR_RED, CHAR_COLOR_WHITE, BACKGROUND_COLOR_BLACK, CHAR_COLOR_RED, BACKGROUND_COLOR_RED);
			vga_style++;
			break;
		default:
			set_VGA_style(BACKGROUND_COLOR_BLACK, CHAR_COLOR_LIGHT_GREY, BACKGROUND_COLOR_RED, CHAR_COLOR_WHITE, BACKGROUND_COLOR_LIGHT_GREY);
			vga_style = 0;
			break;
	}
	return;
}

void set_VGA_style(char background, char character, char mouseback, char mousechar, char startbar){
	setBackgroundColor(background);
	setAllCharacterColor(character);
	erase_mouse();
	setMouseColor(mouseback, mousechar);
	draw_mouse();
	k_setStartBarColor(startbar);
	return;
}
