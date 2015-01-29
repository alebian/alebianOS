#include "../../include/system.h"

/**********************************************
kmain() 
C code entry point.
*************************************************/

static int shutdown = 0;
static initializable initializers[] = {
	{"Starting IDT... ", &setup_IDT, 0},
	{"Starting keyboard... ", &start_keyboard_buffer, 0},
	{"Starting mouse... ", &start_mouse, 0},
	{"Loading SMBIOS... ", &startSMBIOS, 0},
	{"Starting paging...", 0, &start_paging},
	{0, 0, 0}
};

void kmain(multiboot* mboot, int multiboot_magic){
	int i;

	k_set_loading_screen();

	if(multiboot_magic!=0x2BADB002){
		k_panic("The OS wasn't loaded by a Multiboot-compliant boot loader and it's impossible to continue.");
		return;
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

	k_enableMouse();

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

void k_turnOn(){
	shutdown = 0;
}

void k_turnOff(){
	shutdown = 1;
}

int k_isOn(){
	return !shutdown;
}
