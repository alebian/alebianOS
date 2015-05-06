#include "../../include/system.h"

static int shell_ready = 0;
static shell_file shellfile;

void k_shutdown(){
	k_shutdownScreen();
	acpiPowerOff();
	_Cli();
	while(1){}
	return;
}

void k_reboot(){
	k_rebootanimation();
	_outb(0x64, 0xFE);
	return;
}

void k_setStartBarColor(char color){
	setStartBarColor(SECONDBYTE(color));
	shell_updateStartBar();
	return;
}

int k_isShellEnabled(){
	return shell_ready;
}

void k_enableShell(){
	shell_ready = 1;
	return;
}

void k_disableShell(){
	shell_ready = 0;
	return;
}

void k_printWithColor(char* s, char color){
	char aux = getCharacterColor();
	setCharacterColor(color);
	printf("%s", s);
	setCharacterColor(aux);
	return;
}

void k_printwarning(char* s){
	k_printWithColor(s, CHAR_COLOR_GREEN);
	return;
}

void k_printerror(char* s){
	k_printWithColor(s, CHAR_COLOR_RED);
	return;
}

void k_printalert(char* s){
	k_printWithColor(s, CHAR_COLOR_LIGHT_BROWN);
	return;
}

void k_printOSmsg(char* s){
	k_printWithColor(s, CHAR_COLOR_BLUE);
	return;
}

void init_shell_file(){
	/******************************************************/
	/* WE SHOULD USE MALLOC BUT IT IS NOT IMPLEMENTED YET */
	shellfile.init = (char*) 0x300000;
	shellfile.limit = 0x100000;
	/******************************************************/
	reset_shell_file();
	return;
}

void reset_shell_file(){
	memset(shellfile.init, 0, shellfile.limit);
	shellfile.writepos = 0;
	shellfile.scroll_downs = 0;
	return;
}

void save_shell_row(char* from){
	memcpy((char*)from, shellfile.init+shellfile.writepos, 160);
	shellfile.writepos+=160;
	shellfile.scroll_downs++;
	return;
}
