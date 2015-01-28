#include "../../include/system.h"

/**********************************************
kmain() 
C code entry point.
*************************************************/

static initializable initializers[] = {
	{"Starting IDT... ", &setup_IDT, 0},
	{"Starting keyboard... ", &start_keyboard_buffer, 0},
	{"Starting mouse... ", &start_mouse, 0},
	{"Loading SMBIOS... ", &startSMBIOS, 0},
	{"Starting paging...", 0, &start_paging},
	{0, 0, 0}
};

int kmain(multiboot* mboot, int multiboot_magic){
	int i;
	/* Prepare screen to show the loading screen */
	k_clearFullScreen();
	k_setFullBackgroundColor(BACKGROUND_COLOR_BLACK);
	k_setCharacterColor(CHAR_COLOR_WHITE);
	k_showLoadingScreen();
	set_vga_size(12, 25);

	if(multiboot_magic!=0x2BADB002){
		k_panic("The OS wasn't loaded by a Multiboot-compliant boot loader and it's impossible to continue.");
		return -1;
	}

	for(i=0; initializers[i].description != 0; i++){
		k_nice_square();
		printf("%s", initializers[i].description);
		if(initializers[i].vinit != 0){
			initializers[i].vinit();
		}
		else if(initializers[i].minit !=0){
			initializers[i].minit(mboot);
		}
		//k_sleep(3);
		k_printwarning("DONE\n");
	}

	// Just to see if everything went well
	//k_sleep(30);

	/* Prepare screen to load shell properly */
	k_clearFullScreen();
	k_setFullBackgroundColor(BACKGROUND_COLOR_BLACK);
	k_setCharacterColor(CHAR_COLOR_LIGHT_GREY);
	set_vga_size(3, 25);

	shell();
	
	return 0;
}

void k_nice_square(){
	printf("%s", "[");
	k_printwarning("+");
	printf("%s", "] ");
	return;
}