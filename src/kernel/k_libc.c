#include "../../include/kernel/k_libc.h"
#include "../../include/kernel/k_libasm.h"
#include "../../include/kernel/defs.h"
#include "../../include/kernel/buffers.h"
#include "../../include/lib/stdio.h"
#include "../../include/drivers/video.h"
#include "../../include/drivers/keyboard.h"

SMBIOSEntryPoint* SMBIOS;
SMBIOSHeader* SMBIOShead;

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
	    		if(buffer[i]=='\n'){
	    			initializeKeyboardBuffer();
	    		}
	    		c++;
	    	}
	    	break;
	    }
	    default: {
	    	break;
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
	    		buffer[i] = get_char_from_keyboard_buffer();
	    		c++;
	    	}
	    	break;
	    }
	    default: {
	    	break;
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

void checkBIOSinfo(){
	char *mem = (char *) 0xF0000;
	int length, i;
	unsigned char checksum;
	while ((unsigned int) mem < 0x100000) {
		if (mem[0] == '_' && mem[1] == 'S' && mem[2] == 'M' && mem[3] == '_') {
			length = mem[5];
			checksum = 0;
			for(i = 0; i < length; i++) {
				checksum += mem[i];
			}
			if(checksum == 0) {
				SMBIOS = (SMBIOSEntryPoint*) mem;
				break;
			}
		}
		mem += 16;
	}
    /* Once the header of the 0 SMBIOS table entry is found
    * print the BIOS info */
	SMBIOShead = (SMBIOSHeader*) SMBIOS->TableAddress;
	char * BIOSstring = (char*) SMBIOS->TableAddress + SMBIOShead->Length;
	printf("%s\n", "BIOS information:");
    /*Prints string 1 */
	printf("%s", "\tBrand: ");
	i = 0;
	while(BIOSstring[i]!='\0'){
		putchar(BIOSstring[i]);
		i++;
	}
	i++;
	printf("%s\n", " ");
    /*Prints string 2 */
	printf("%s", "\tVersion: ");
	while(BIOSstring[i]!='\0'){
		putchar(BIOSstring[i]);
		i++;
	}
	i++;
	printf("%s\n", " ");
    /*Prints string 3 */
	printf("%s", "\tBuild date: ");
	while(BIOSstring[i]!='\0'){
		putchar(BIOSstring[i]);
		i++;
	}
	return;
}

void turnOff(){
	k_clearScreen();
	printf("\n\n\n\n\n\n\n\n\n%s", "                            Goodbye! See you soon!");
	printf("\n\n\n%s","                         Please turn off your computer");
	_Cli();
		while(1){}
}

void k_clearScreen(){
	clearScreen();
	return;
}

void k_scrollScreen(){
	scrollScreen();
	return;
}

void k_chgBkgColor(char charcolor, char backcolor){
	changeBackgroundColor(charcolor, backcolor);
	return;
}
