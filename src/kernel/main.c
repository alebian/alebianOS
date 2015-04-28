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

	setup_IDT();
	k_loading_log("IDT set up completed.");
	
	k_SetUpDrivers();
	k_loading_log("Drivers set up completed.");

	startSMBIOS();
	k_loading_log("SMBIOS loaded.");

	start_paging(mboot);
	k_loading_log("Paging set up completed.");

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

void k_loading_log(char* message){
	printf("%s", "[");
	k_printwarning("+");
	printf("%s", "] ");
	printf("%s\n", message);
	k_sleep(3);
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
