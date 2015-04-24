#include "../../include/system.h"

/**********************************************
 * kmain() 
 * C code entry point.
 **********************************************/

static int shutdown = 0;

void kmain(multiboot* mboot, int multiboot_magic){
	k_set_loading_screen();

	if(multiboot_magic!=0x2BADB002){
		k_panic("The OS wasn't loaded by a Multiboot-compliant boot loader and it's impossible to continue.");
		return;
	}

	k_nice_square();
	printf("%s", "Setting up IDT... ");
	setup_IDT();
	k_sleep(3);
	k_printwarning("DONE\n");
	
	k_nice_square();
	printf("%s", "Setting up drivers... ");
	k_SetUpDrivers();
	k_sleep(3);
	k_printwarning("DONE\n");

	k_nice_square();
	printf("%s", "Loading SMBIOS... ");
	startSMBIOS();
	k_sleep(3);
	k_printwarning("DONE\n");

	k_nice_square();
	printf("%s", "Setting up paging...");
	start_paging(mboot);
	k_sleep(3);
	k_printwarning("DONE\n");

	k_sleep(30); // Just to see if everything went well

	while(k_isOn()){
		login();
	}
	
	return;
}

void k_set_loading_screen(){
	/* Prepare screen to show the loading screen */
	k_clearFullScreen();
	k_setFullBackgroundColor(BACKGROUND_COLOR_BLACK);
	k_setCharacterColor(CHAR_COLOR_WHITE);
	k_showLoadingScreen();
	k_set_vga_size(12, 25);
	return;
}

void k_nice_square(){
	printf("%s", "[");
	k_printwarning("+");
	printf("%s", "] ");
	return;
}

void k_SetUpDrivers(){
	start_keyboard_buffer();
	start_mouse();
	return;
}

void k_turnOn(){
	shutdown = 0;
}

void k_turnOff(){
	shutdown = 1;
}

int k_isOn(){
	return !shutdown;
}
