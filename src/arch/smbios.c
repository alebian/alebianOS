#include "../../include/system.h"

static SMBIOSEntryPoint* SMBIOS;
static SMBIOS_structure_header* BIOS_info;
static int present = 0;

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
		printf("%s\n", "BIOS information:");
	    // Prints string 1
		printf("\t%s", "Brand: ");
		int i = 0;
		while(BIOSstring[i]!='\0'){
			putchar(BIOSstring[i++]);
		}
		i++;
		printf("%s\n", " ");
	    // Prints string 2
		printf("\t%s", "Version: ");
		while(BIOSstring[i]!='\0'){
			putchar(BIOSstring[i++]);
		}
		i++;
		printf("%s\n", " ");
	    // Prints string 3
		printf("\t%s", "Build date: ");
		while(BIOSstring[i]!='\0'){
			putchar(BIOSstring[i++]);
		}
		printf("%s\n", " ");
	}else{
		k_printerror("Unable to find SMBIOS.");
	}
	return;
}


void smb_SystemSpeed(){
	/*k_findSMBIOS();
	word * mem = (word*) SMBIOS->TableAddress + SMBIOShead->Length;
	printf("%s\n", "System speed:");
	printf("\t%s", "External clock frequency: ");
	printf("%d", mem[0x12]);
	printf("%s\n", "Mhz");
	printf("\t%s", "Maximum CPU speed: ");
	printf("%d", mem[0x14]);
	printf("%s\n", "Mhz");
	printf("\t%s", "Current CPU speed: ");
	printf("%d", mem[0x16]);
	printf("%s\n", "Mhz");*/
	return;
}