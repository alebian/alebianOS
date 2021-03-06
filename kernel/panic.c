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

void panic(char* message){
	clearFullScreen();
	setFullBackgroundColor(BACKGROUND_COLOR_BLUE);
	set_vga_size(1, 25);
	setCharacterColor(CHAR_COLOR_WHITE);
	k_printf("\n");
	k_printf(message);
	k_printf("\n");
	k_printf("Halting.");
	k_printf("\n");
	beep();
	for (;;){
		turn_off_leds();
		sleep(100);
		turn_on_leds();
		sleep(100);
	}
	return;
}