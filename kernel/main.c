/*
* Copyright 2014, 2015 Alejandro Bezdjian
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "include/system.h"
#include "../userland/include/login.h"
#include "../userland/include/shell.h"

/**********************************************
 * kmain() 
 * C code entry point.
 **********************************************/

int kmain(multiboot_info_t* mboot, int multiboot_magic){
	if(multiboot_magic!=0x2BADB002){
		panic("The OS wasn't loaded by a Multiboot-compliant bootloader and it's impossible to continue.");
		return 0xDEADC0DE;
	}
	k_showLoadingScreen();
	
	k_loading_log(init_GDT(), "GDT initialized.", "Unable to initialize GDT.");
	k_loading_log(init_IDT(), "IDT initialized.", "Unable to initialize IDT.");
	k_loading_log(set_frecuency(20), "PIC initialized.", "Unable to initialize PIC.");
	k_loading_log(init_pmm(mboot), "Memory manager initialized.", "Unable to initialize memory manager.");
	//k_loading_log(init_paging(mboot), "Paging initialized.", "Unable to initialize paging.");
	k_loading_log(init_keyboard(), "Keyboard driver initialized.", "Unable to initialize keyboard driver.");
	k_loading_log(init_mouse(), "Mouse driver initialized.", "Unable to initialize mouse driver.");
	k_loading_log(init_ACPI(), "ACPI loaded.", "Unable to load ACPI.");
	k_loading_log(init_SMBIOS(), "SMBIOS loaded.", "Unable to load SMBIOS.");

	sleep(30); // Just to see if everything went well

	// Prepare screen for applications
	k_set_desktop();
	while(1){
		login();
		//REMOVE SHELL HEADER AFTER TESTS
		//shell("test", "x86");
	}
	return 0xDEADC0DE;
}

void k_loading_log(int ret, char* completed, char* error){
	if(ret != -1){
		k_printf("[");
		k_printSuccess("OK");
		k_printf("] ");
		k_printf(completed);
		k_printf("\n");
	}else{
		k_printf("[");
		k_printError("X");
		k_printf("] ");
		k_printf(error);
		k_printf("\n");
	}
	return;
}