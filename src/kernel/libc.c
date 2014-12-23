#include "../../include/kernel/kc.h"
#include "../../include/kernel/defs.h"
#include "../../include/lib/stdio.h"
#include "../../include/drivers/video.h"
#include "../../include/drivers/keyboard.h"

/***************************************************************
* setup_IDT_entry
* Initializes an IDT Descriptor
*
* Arguments: 
*	IDT's element pointer
*	Descriptor's selector
*	Handler pointer	
*	Access level
*	Zero
****************************************************************/

void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access, byte zero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->zero = zero;
}

size_t __write(int fd, char* buffer, size_t count){
	size_t c;
	int i;
	switch(fd) {
		/* 1: Video output */
	    case 1: {
	    	c = 0;
	    	for (i = 0; i < count; i++)
	    	{
	    		print(buffer[i]);
	    		c++;
	    	}
	    }
	    default: {
	    }
	}
	return c;
}

size_t __read(int fd, char* buffer, size_t count){
	size_t c;
	int i;
	switch(fd) {
		/* 1: Keyboard input */
	    case 1: {
	    	c = 0;
	    	for (i = 0; i < count; i++)
	    	{
	    		if(keyboard_buffer_can_read()){
	    		    buffer[i] = get_char_from_keyboard_buffer();
	    		    c++;
	    		}
	    	}
	    }
	    default: {
	    }
	}
	return c;
}

void showLoadingScreen(){
	changeBackgroundColor(CHAR_COLOR_WHITE, BACKGROUND_COLOR_RED);
	printf("\n");
	printf(" $$$$$$$$ $$           $$       $$                    $$$$$$   $$$$$$\n");
	printf(" $$    $$ $$           $$                            $$    $$ $$    $$\n");
	printf(" $$    $$ $$   $$$$$   $$       $$  $$$$$$  $$ $$$   $$    $$ $$\n");
	printf(" $$$$$$$$ $$  $$   $$  $$$$$$$  $$       $$ $$$  $$$ $$    $$  $$$$$$\n");
	printf(" $$    $$ $$  $$$$$$   $$    $$ $$   $$$$$$ $$    $$ $$    $$       $$\n");
	printf(" $$    $$ $$  $$       $$    $$ $$  $$   $$ $$    $$ $$    $$ $$    $$\n");
	printf(" $$    $$  $$  $$$$$$  $$$$$$$  $$   $$$$$$ $$    $$  $$$$$$   $$$$$$\n");
	printf("\n\n\n");
	return;
}