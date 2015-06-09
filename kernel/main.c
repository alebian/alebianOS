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
	k_install_drivers();
	// Prepare screen for applications
	k_set_desktop();
	while(1){
		login();
		//REMOVE SHELL HEADER AFTER TESTS
		//shell("test", "x86");
	}
	return 0xDEADC0DE;
}
