/*
* Copyright 2014 Alejandro Bezdjian
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

#include "../include/system.h"

static SMBIOSEntryPoint* SMBIOS;
static SMBIOS_structure_header* BIOS_info;
static char present = 0;

/*
 * Finds the SMBIOS header _SM_
 * It should be somehwere between 0xF0000 and 0xFFFFF
 */
int init_SMBIOS(){
	char* mem = (char*) SMBIOS_START;
	int length, i;
	unsigned char checksum;
	while ((unsigned int) mem < 0x100000){
		if (mem[0] == '_' && mem[1] == 'S' && mem[2] == 'M' && mem[3] == '_'){
			length = mem[5];
			checksum = 0;
			for(i = 0; i < length; i++){
				checksum += mem[i];
			}
			if(checksum == 0){
				SMBIOS = (SMBIOSEntryPoint*) mem;
				present = 1;
				break;
			}
		}
		mem += 16;
	}
	if(present){
		BIOS_info = (SMBIOS_structure_header*) SMBIOS->TableAddress;
	}else{
		// "Unable to find SMBIOS."
		return 1;
	}
	return 0;
}

void smb_BIOSinfo(){
	if(present){
		char* BIOSstring = (char*) ((int)BIOS_info + BIOS_info->Length);
		k_printf("BIOS information:\n");
	    // Prints string 1
		k_printf("\tBrand: ");
		int i = 0;
		while(BIOSstring[i]!='\0'){
			print(BIOSstring[i++]);
		}
		i++;
		k_printf("\n");
	    // Prints string 2
		k_printf("\tVersion: ");
		while(BIOSstring[i]!='\0'){
			print(BIOSstring[i++]);
		}
		i++;
		k_printf("\n");
	    // Prints string 3
		k_printf("\tBuild date: ");
		while(BIOSstring[i]!='\0'){
			print(BIOSstring[i++]);
		}
		k_printf("\n");
	}else{
		k_printError("Unable to find SMBIOS.");
	}
	return;
}