#include "../../include/system.h"

static int loadingscreenanimation = 1;

void k_showLoadingScreen(){
	/* Prepare screen to show the loading screen */
	clearFullScreen();
	setFullBackgroundColor(BACKGROUND_COLOR_BLACK);
	setCharacterColor(CHAR_COLOR_WHITE);
	k_printalert("\n");
	k_printalert("                                    $$$$$$    $$$$$$ \n");
	k_printalert("                                   $$    $$  $$    $$\n");
	k_printalert("                          $$$$$$   $$    $$  $$      \n");
	k_printalert("                               $$  $$    $$   $$$$$$ \n");
	k_printalert("                           $$$$$$  $$    $$        $$\n");
	k_printalert("                          $$   $$  $$    $$  $$    $$\n");
	k_printalert("                           $$$$$$   $$$$$$    $$$$$$ \n");
	k_printalert("\n                              ");
	k_printalert(OS_NAME);
	k_printalert(" is loading\n");
	set_vga_size(12, 25);
	return;
}

void k_LoadingScreenAnimation(){
	switch(loadingscreenanimation){
		case 1:
			printxyc(' ', CHAR_COLOR_LIGHT_BROWN, 50, 9);
			printxyc(' ', CHAR_COLOR_LIGHT_BROWN, 51, 9);
			printxyc(' ', CHAR_COLOR_LIGHT_BROWN, 52, 9);
			loadingscreenanimation++;
			break;
		case 5:
			printxyc('.', CHAR_COLOR_LIGHT_BROWN, 50, 9);
			loadingscreenanimation++;
			break;
		case 10:
			printxyc('.', CHAR_COLOR_LIGHT_BROWN, 51, 9);
			loadingscreenanimation++;
			break;
		case 15:
			printxyc('.', CHAR_COLOR_LIGHT_BROWN, 52, 9);
			loadingscreenanimation++;
			break;
		case 20:
			loadingscreenanimation = 1;
			break;
		default:
			loadingscreenanimation++;
			break;
	}
	return;
}

void k_rebootanimation(){
	int ticks = 10;
	k_disableShell();
	k_disableMouse();
	_Sti();
	clearFullScreen();
	setFullBackgroundColor(BACKGROUND_COLOR_BLUE);
	setCharacterColor(CHAR_COLOR_WHITE);
	printf("\n\n\n\n\n\n%s", "                                  Rebooting in...");
	set_vga_size(9, 25);
	printf("%s\n", "                                   $$$$$$$$$$");
	printf("%s\n", "                                   $$");
	printf("%s\n", "                                   $$");
	printf("%s\n", "                                   $$$$$$$$$");
	printf("%s\n", "                                           $$");
	printf("%s\n", "                                           $$");
	printf("%s\n", "                                           $$");
	printf("%s", "                                   $$$$$$$$$");
	sleep(ticks);
	clearScreen();
	printf("%s\n", "                                   $$      $$");
	printf("%s\n", "                                   $$      $$");
	printf("%s\n", "                                   $$      $$");
	printf("%s\n", "                                   $$      $$");
	printf("%s\n", "                                   $$$$$$$$$$");
	printf("%s\n", "                                           $$");
	printf("%s\n", "                                           $$");
	printf("%s", "                                           $$");
	sleep(ticks);
	clearScreen();
	printf("%s\n", "                                    $$$$$$$$$");
	printf("%s\n", "                                          $$");
	printf("%s\n", "                                         $$");
	printf("%s\n", "                                      $$$$$$");
	printf("%s\n", "                                           $$");
	printf("%s\n", "                                           $$");
	printf("%s\n", "                                           $$");
	printf("%s", "                                    $$$$$$$$");
	sleep(ticks);
	clearScreen();
	printf("%s\n", "                                   $$$$$$$$$");
	printf("%s\n", "                                           $$");
	printf("%s\n", "                                           $$");
	printf("%s\n", "                                          $$");
	printf("%s\n", "                                        $$");
	printf("%s\n", "                                      $$");
	printf("%s\n", "                                    $$");
	printf("%s", "                                   $$$$$$$$$$");
	sleep(ticks);
	clearScreen();
	printf("%s\n", "                                        $$");
	printf("%s\n", "                                      $$$$");
	printf("%s\n", "                                     $$ $$");
	printf("%s\n", "                                        $$");
	printf("%s\n", "                                        $$");
	printf("%s\n", "                                        $$");
	printf("%s\n", "                                        $$");
	printf("%s", "                                      $$$$$$");
	sleep(ticks);
	clearScreen();
	return;
}

void k_shutdownScreen(){
	k_disableShell();
	k_disableMouse();
	clearFullScreen();
	setFullBackgroundColor(BACKGROUND_COLOR_BLACK);
	set_vga_size(1,25);
	k_printalert("                         Please turn off your computer...");
	sleep(30);
	// 1 row
	printSquare(28, 1, 0x44);
	printSquare(32, 1, 0x44);
	printSquare(36, 1, 0x44);
	printSquare(40, 1, 0x44);
	printSquare(44, 1, 0x44);
	// 2 row
	printSquare(24, 3, 0x44);
	printSquare(28, 3, 0x44);
	printSquare(32, 3, 0x44);
	printSquare(36, 3, 0x44);
	printSquare(40, 3, 0x44);
	printSquare(44, 3, 0x44);
	printSquare(48, 3, 0x44);
	printSquare(52, 3, 0x44);
	printSquare(56, 3, 0x44);
	// 3 row
	printSquare(24, 5, 0x66);
	printSquare(28, 5, 0x66);
	printSquare(32, 5, 0x66);
	printSquare(36, 5, 0xFF);
	printSquare(40, 5, 0xFF);
	printSquare(44, 5, 0x00);
	printSquare(48, 5, 0xFF);
	// 4 row
	printSquare(20, 7, 0x66);
	printSquare(24, 7, 0xFF);
	printSquare(28, 7, 0x66);
	printSquare(32, 7, 0xFF);
	printSquare(36, 7, 0xFF);
	printSquare(40, 7, 0xFF);
	printSquare(44, 7, 0x00);
	printSquare(48, 7, 0xFF);
	printSquare(52, 7, 0xFF);
	printSquare(56, 7, 0xFF);
	// 5 row
	printSquare(20, 9, 0x66);
	printSquare(24, 9, 0xFF);
	printSquare(28, 9, 0x66);
	printSquare(32, 9, 0x66);
	printSquare(36, 9, 0xFF);
	printSquare(40, 9, 0xFF);
	printSquare(44, 9, 0xFF);
	printSquare(48, 9, 0x66);
	printSquare(52, 9, 0xFF);
	printSquare(56, 9, 0xFF);
	printSquare(60, 9, 0xFF);
	// 6 row
	printSquare(20, 11, 0x66);
	printSquare(24, 11, 0x66);
	printSquare(28, 11, 0xFF);
	printSquare(32, 11, 0xFF);
	printSquare(36, 11, 0xFF);
	printSquare(40, 11, 0xFF);
	printSquare(44, 11, 0x66);
	printSquare(48, 11, 0x66);
	printSquare(52, 11, 0x66);
	printSquare(56, 11, 0x66);
	// 7 row
	printSquare(28, 13, 0xFF);
	printSquare(32, 13, 0xFF);
	printSquare(36, 13, 0xFF);
	printSquare(40, 13, 0xFF);
	printSquare(44, 13, 0xFF);
	printSquare(48, 13, 0xFF);
	printSquare(52, 13, 0xFF);
	// 8 row
	printSquare(24, 15, 0x11);
	printSquare(28, 15, 0x11);
	printSquare(32, 15, 0x44);
	printSquare(36, 15, 0x11);
	printSquare(40, 15, 0x11);
	printSquare(44, 15, 0x11);
	// 9 row
	printSquare(20, 17, 0x11);
	printSquare(24, 17, 0x11);
	printSquare(28, 17, 0x11);
	printSquare(32, 17, 0x44);
	printSquare(36, 17, 0x11);
	printSquare(40, 17, 0x11);
	printSquare(44, 17, 0x44);
	printSquare(48, 17, 0x11);
	printSquare(52, 17, 0x11);
	printSquare(56, 17, 0x11);
	// 10 row
	printSquare(16, 19, 0x11);
	printSquare(20, 19, 0x11);
	printSquare(24, 19, 0x11);
	printSquare(28, 19, 0x11);
	printSquare(32, 19, 0x44);
	printSquare(36, 19, 0x11);
	printSquare(40, 19, 0x11);
	printSquare(44, 19, 0x44);
	printSquare(48, 19, 0x11);
	printSquare(52, 19, 0x11);
	printSquare(56, 19, 0x11);
	printSquare(60, 19, 0x11);
	// 11 row
	printSquare(16, 21, 0xFF);
	printSquare(20, 21, 0xFF);
	printSquare(24, 21, 0x11);
	printSquare(28, 21, 0x11);
	printSquare(32, 21, 0x44);
	printSquare(36, 21, 0x44);
	printSquare(40, 21, 0x44);
	printSquare(44, 21, 0x44);
	printSquare(48, 21, 0x11);
	printSquare(52, 21, 0x11);
	printSquare(56, 21, 0xFF);
	printSquare(60, 21, 0xFF);
	// 12 row
	printSquare(16, 23, 0xFF);
	printSquare(20, 23, 0xFF);
	printSquare(24, 23, 0xFF);
	printSquare(28, 23, 0x44);
	printSquare(32, 23, 0xEE);
	printSquare(36, 23, 0x44);
	printSquare(40, 23, 0x44);
	printSquare(44, 23, 0xEE);
	printSquare(48, 23, 0x44);
	printSquare(52, 23, 0xFF);
	printSquare(56, 23, 0xFF);
	printSquare(60, 23, 0xFF);
	return;
}