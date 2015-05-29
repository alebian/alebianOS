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

#include "../include/klib.h"
#include "../include/arch/i386/cpuid.h"
#include "../include/drivers/cpudet.h"

int detect_cpu(){ 
	int vendor = _cpuvendor();
	switch(vendor) {
		case 0x756e6547: /* Intel Magic Code */
		do_intel();
		return 1;
		break;
		case 0x68747541: /* AMD Magic Code */
		do_amd();
		return 1;
		break;
		default:
		k_printf("Unknown x86 CPU Detected\n");
		return -1;
		break;
	}
}

void do_intel(){
	k_printf("Intel\n");
	return;
}

void  do_amd(){
	k_printf("AMD\n");
	return;
}