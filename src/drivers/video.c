#include "../../include/drivers/video.h"
#include "../../include/kernel/kasm.h"

int cursor_pos = 0;

void clearScreen(){
	char *vidmem = (char*) VGA_PORT;
	unsigned int i=0;
	while(i < (VGA_WIDTH*VGA_HEIGHT*2))
	{
		vidmem[i]=CHAR_BLANK;
		i++;
		vidmem[i]=CHAR_COLOR_LIGHT_GREY;
		i++;
	};
	cursor_pos=0;
	return;
}

void changeBackgroundColor(char charcolor, char backcolor){
	char *vidmem = (char*) VGA_PORT;
	unsigned int i=1;
	char newC = backcolor + charcolor;
	while(i < (VGA_WIDTH*VGA_HEIGHT*2))
	{
		vidmem[i]=newC;
		i+=2;
	};
	return;
}

void scrollScreen(){
	char *vidmem = (char*) VGA_PORT;
	unsigned int i=0;
	while(i < (VGA_WIDTH*(VGA_HEIGHT-1)*2))
	{	
		vidmem[i]= vidmem[i+160];
		i++;
		vidmem[i]=CHAR_COLOR_LIGHT_GREY;
		i++;
	};
	while(i < (VGA_WIDTH*VGA_HEIGHT*2))
	{
		vidmem[i]=CHAR_BLANK;
		i++;
		vidmem[i]=CHAR_COLOR_LIGHT_GREY;
		i++;
	};
	return;
}

/* Prints a character in a specified VGA screen position without changing cursor */
void printxy(char c, int x, int y){
	char *vidmem = (char*) VGA_PORT;
	vidmem[(y*VGA_WIDTH*2)+(x*2)] = c;
	return;
}

void print(char c){
	char *vidmem = (char*) VGA_PORT;
	switch(c) {
	    case '\t': {
	    	cursor_pos+=TAB;
	    }
	    case '\n':{
	    	if(cursor_pos > 78*25*2){
	    	    scrollScreen();
	    	    cursor_pos=160*24;
	    	} else {
	    		cursor_pos += 160 - cursor_pos%160;
	    	}
	    }
	    case '\b': {
	    	if(cursor_pos!=0){            
	    	    cursor_pos -=2;
	    	    vidmem[cursor_pos] = ' ';
	    	}
	    }
	    default: {
	    	vidmem[cursor_pos] = c;
	    	cursor_pos += 2;
	    }
	}
	update_cursor(cursor_pos);
	return;
}

void update_cursor(int position) {
	unsigned short aux = position; 
	if (aux != 0) { 
		aux = aux/2;
		_outb(0x3D4, 0x0F); 
		_outb(0x3D5, (unsigned char)aux); 
		_outb(0x3D4, 0x0E); 
		_outb(0x3D5, (unsigned char )((aux>>8)));
	}
}