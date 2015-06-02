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

#ifndef _SMBIOS_
#define _SMBIOS_

#define SMBIOS_START 0xF0000

typedef struct {
 	char EntryPointString[4];    		 // This is _SM_
 	unsigned char Checksum;              // This value summed with all the values of the table, should be 0 (overflow)
 	unsigned char Length;                // Length of the Entry Point Table. Since version 2.1 of SMBIOS, this is 0x1F
 	unsigned char MajorVersion;          // Major Version of SMBIOS
 	unsigned char MinorVersion;          // Minor Version of SMBIOS
 	unsigned short MaxStructureSize;     // Maximum size of a SMBIOS Structure (we will se later)
 	unsigned char EntryPointRevision;    // ...
 	char FormattedArea[5];       		 // ...
 	char EntryPointString2[5];   		 // This is _DMI_
 	unsigned char Checksum2;             // Checksum for values from EntryPointString2 to the end of table
 	unsigned short TableLength;          // Length of the Table containing all the structures
 	unsigned int TableAddress;	     	 // Address of the Table
 	unsigned short NumberOfStructures;   // Number of structures in the table
 	unsigned char BCDRevision;           // Unused
} __attribute__ ((packed)) SMBIOSEntryPoint;

typedef struct {
 	unsigned char Type;
 	unsigned char Length;
 	unsigned short Handle;
 } __attribute__ ((packed)) SMBIOS_structure_header;

int init_SMBIOS(void);
void smb_BIOSinfo(void);

#endif