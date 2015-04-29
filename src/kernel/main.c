#include "../../include/system.h"

/**********************************************
 * kmain() 
 * C code entry point.
 **********************************************/

static int shutdown = 0;
static int loadingscreen = 1;

void kmain(multiboot_info* mboot, int multiboot_magic){
	k_set_loading_screen();
	if(multiboot_magic!=0x2BADB002){
		k_panic("The OS wasn't loaded by a Multiboot-compliant bootloader and it's impossible to continue.");
		return;
	}

	setup_GDT();
	k_loading_log("GDT set up completed");

	setup_IDT();
	k_loading_log("IDT set up completed");

	k_SetUpDrivers();
	k_loading_log("Drivers set up completed");

	startSMBIOS();
	k_loading_log("SMBIOS loaded");

	start_paging(mboot);
	k_loading_log("Paging set up completed");

	k_sleep(30); // Just to see if everything went well
	loadingscreen = 0;
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

int k_isLoading(){
	return loadingscreen!=0;
}

void k_LoadingScreenEffect(){
	/*switch(loadingscreen){
		case 1:
			printxyc(' ', CHAR_COLOR_LIGHT_BROWN, 7, 9);
			printxyc(' ', CHAR_COLOR_LIGHT_BROWN, 8, 9);
			printxyc(' ', CHAR_COLOR_LIGHT_BROWN, 9, 9);
			loadingscreen++;
			break;
		case 5:
			printxyc('.', CHAR_COLOR_LIGHT_BROWN, 7, 9);
			loadingscreen++;
			break;
		case 10:
			printxyc('.', CHAR_COLOR_LIGHT_BROWN, 8, 9);
			loadingscreen++;
			break;
		case 15:
			printxyc('.', CHAR_COLOR_LIGHT_BROWN, 9, 9);
			loadingscreen++;
			break;
		case 20:
			loadingscreen = 1;
			break;
		default:
			loadingscreen++;
			break;
	}*/
	switch(loadingscreen){
		case 1:
			printxyc(' ', CHAR_COLOR_LIGHT_BROWN, 50, 9);
			printxyc(' ', CHAR_COLOR_LIGHT_BROWN, 51, 9);
			printxyc(' ', CHAR_COLOR_LIGHT_BROWN, 52, 9);
			loadingscreen++;
			break;
		case 5:
			printxyc('.', CHAR_COLOR_LIGHT_BROWN, 50, 9);
			loadingscreen++;
			break;
		case 10:
			printxyc('.', CHAR_COLOR_LIGHT_BROWN, 51, 9);
			loadingscreen++;
			break;
		case 15:
			printxyc('.', CHAR_COLOR_LIGHT_BROWN, 52, 9);
			loadingscreen++;
			break;
		case 20:
			loadingscreen = 1;
			break;
		default:
			loadingscreen++;
			break;
	}
	return;
}

void k_loading_log(char* message){
	printf("%s", "[");
	k_printwarning("+");
	printf("%s", "] ");
	printf("%s\n", message);
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