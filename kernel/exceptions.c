/*
* Copyright 2015 Alejandro Bezdjian
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

void exc_00(){
	panic("Divide error exception.");
	return;
}

void exc_01(){
	panic("Debug exception.");
	return;
}

void exc_02(){
	k_shutdown();
	return;
}

void exc_03(){
	panic("Breakpoint exception.");
	return;
}

void exc_04(){
	panic("Overflow exception.");
	return;
}

void exc_05(){
	panic("Bounds check exception.");
	return;
}

void exc_06(){
	panic("Invalid opcode exception.");
	return;
}

void exc_07(){
	panic("Coprocessor not available exception.");
	return;
}

void exc_08(){
	panic("Double fault exception.");
	return;
}

void exc_09(){
	panic("Coprocessor segment overrun exception.");
	return;
}

void exc_10(){
	panic("Invalid TSS exception.");
	return;
}

void exc_11(){
	panic("Segment not present exception.");
	return;
}

void exc_12(){
	panic("Stack exception.");
	return;
}

void exc_13(){
	panic("General protection exception.");
	return;
}

void exc_14(int errorcode, int cr2){
	k_printError("Page fault exception.\n");
	if(errorcode & 0x01){ // Present bit is set
		if(errorcode & 0x02){ // R/W bit is set
			if(errorcode & 0x04){ // U/S bit is set
				// 1 1 1
				k_printError("User process tried to write a page and caused a protection fault.\n");
			}else{ // U/S bit is not set
				// 0 1 1
				k_printError("Supervisory process tried to write a page and caused a protection fault.\n");
			}
		}else{// R/W bit is not set
			if(errorcode & 0x04){ // U/S bit is set
				// 1 0 1
				k_printError("User process tried to read a page and caused a protection fault.\n");
			}else{ // U/S bit is not set
				// 0 0 1
				k_printError("Supervisory process tried to read a page and caused a protection fault.\n");
			}
		}
	}else{ // Present flag is not set
		if(errorcode & 0x02){ // R/W bit is set
			if(errorcode & 0x04){ // U/S bit is set
				// 1 1 0
				k_printError("User process tried to write to a non-present page entry.\n");
			}else{ // U/S bit is not set
				// 0 1 0
				k_printError("Supervisory process tried to write to a non-present page entry.\n");
			}
		}else{ // R/W bit is not set
			if(errorcode & 0x04){ // U/S bit is set
				// 1 0 0
				k_printError("User process tried to read a non-present page entry.\n");
			}else{ // U/S bit is not set
				// 0 0 0
				k_printError("Supervisory process tried to read a non-present page entry.\n");
			}
		}
	}
	return;
}

void exc_16(){
	panic("Coprocessor error exception.");
	return;
}