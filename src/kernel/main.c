#include "../../include/system.h"

/**********************************************
 * kmain() 
 * C code entry point.
 **********************************************/

static int loadingscreen = 1;

void kmain(multiboot_info_t* mboot, int multiboot_magic){
	k_showLoadingScreen();
	if(multiboot_magic!=0x2BADB002){
		k_panic("The OS wasn't loaded by a Multiboot-compliant bootloader and it's impossible to continue.");
		return;
	}

	loading_log(init_GDT(), "GDT initialized.", "Unable to initialize GDT.");
	loading_log(init_IDT(), "IDT initialized.", "Unable to initialize IDT.");
	loading_log(set_frecuency(20), "PIC initialized.", "Unable to initialize PIC.");
	loading_log(init_paging(mboot), "Paging initialized.", "Unable to initialize paging.");
	loading_log(init_keyboard(), "Keyboard driver initialized.", "Unable to initialize keyboard driver.");
	loading_log(init_mouse(), "Mouse driver initialized.", "Unable to initialize mouse driver.");
	loading_log(init_ACPI(), "ACPI loaded.", "Unable to load ACPI.");
	loading_log(init_SMBIOS(), "SMBIOS loaded.", "Unable to load SMBIOS.");

	sleep(30); // Just to see if everything went well
	loadingscreen = 0;
	while(1){
		login();
	}
	return;
}

int k_isLoading(){
	return loadingscreen!=0;
}

void loading_log(int ret, char* completed, char* error){
	if(ret != -1){
		printf("%s", "[");
		k_printwarning("OK");
		printf("%s", "] ");
		printf("%s\n", completed);
	}else{
		printf("%s", "[");
		k_printerror("X");
		printf("%s", "] ");
		printf("%s\n", error);
	}
	return;
}