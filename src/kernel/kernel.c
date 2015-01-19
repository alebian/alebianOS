#include "../../include/system.h"

/**********************************************
kmain() 
C code entry point.
*************************************************/

int kmain(multiboot * mboot, unsigned int initial_stack){
	/* Prepare screen to show the loading screen */
	k_clearFullScreen();
	k_setFullBackgroundColor(BACKGROUND_COLOR_BLACK);
	k_setCharacterColor(CHAR_COLOR_WHITE);
	k_showLoadingScreen();
	set_vga_size(12, 25);

	//k_set_frecuency(10000);

	printf("%s", "Starting IDT... ");
	setup_IDT();
	k_sleep(3);
	k_printwarning("DONE\n\n");

	printf("%s", "Starting keyboard... ");
	start_keyboard_buffer();
	k_sleep(3);
	k_printwarning("DONE\n\n");

	printf("%s", "Starting mouse... ");
	start_mouse();
	k_sleep(3);
	k_printwarning("DONE\n\n");

	printf("%s", "Starting memory manager and paging... ");
	//start_memory_manager(mboot);
	k_sleep(3);
	k_printwarning("DONE\n\n");

	printf("%s", "Loading SMBIOS... ");
	startSMBIOS();
	k_sleep(3);
	k_printwarning("DONE\n\n");

	// Just to see if everything went well
	//k_panic("TEST");
	k_sleep(3);

	/* Prepare screen to load shell properly */
	k_clearFullScreen();
	k_setFullBackgroundColor(BACKGROUND_COLOR_BLACK);
	k_setCharacterColor(CHAR_COLOR_LIGHT_GREY);
	set_vga_size(3, 25);

	shell();
	
	return 0;
}
