#include "../../include/system.h"

void k_panic(char* message){
	k_disableShell();
	k_disableMouse();
	clearFullScreen();
	setFullBackgroundColor(BACKGROUND_COLOR_BLUE);
	set_vga_size(1, 25);
	setCharacterColor(CHAR_COLOR_WHITE);
	printf("\n%s\n", message);
	printf("\n%s\n", "Halting");
	beep();
	for (;;){
		turn_off_leds();
		sleep(10);
		turn_on_leds();
		sleep(10);
	}
	return;
}