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

#include "include/shell.h"
#include "include/login.h"
#include "include/lib/stdio.h"
#include "include/lib/ulib.h"
#include "include/lib/string.h"
#include "../common.h"

static shell_buffer shellbuff;
static session_data sdata;

void shell(char* username, char* pcname){
	s_clearScreen();
	sdata.user = username;
	sdata.computername = pcname;
	start_shell_buffer();
	prompt();
	// Set the listeners
	s_setKBlistener((int)&shell_keyboardListener);
	while(login_isLogued()){}
	return;
}

void start_shell_buffer(){
	int i;
	shellbuff.bpos = 0;
	shellbuff.lastbpos = 0;
	shellbuff.max_bpos = 0;
	shellbuff.max_lastbpos = 0;
	shellbuff.changed = 0;
	for(i = 0; i < SHELL_BUFFER_SIZE; i++){
		shellbuff.buffer[i] = 0;
		shellbuff.lastbuffer[i] = 0;
	}
	for(i = 0; i < SHELL_COMMAND_SIZE; i++){
		shellbuff.command[i] = 0;
	}
	for(i = 0; i < SHELL_PARAMETER_SIZE; i++){
		shellbuff.parameter[i] = 0;
	}
	return;
}

void restart_shell_buffer(){
	int i;
	shellbuff.lastbpos = shellbuff.bpos;
	shellbuff.max_lastbpos = shellbuff.max_bpos;
	shellbuff.bpos = 0;
	shellbuff.max_bpos = 0;
	shellbuff.changed = 0;
	for(i = 0; i < SHELL_BUFFER_SIZE; i++){
		shellbuff.lastbuffer[i] = shellbuff.buffer[i];
		shellbuff.buffer[i] = 0;
	}
	for(i = 0; i < SHELL_COMMAND_SIZE; i++){
		shellbuff.command[i] = 0;
	}
	for(i = 0; i < SHELL_PARAMETER_SIZE; i++){
		shellbuff.parameter[i] = 0;
	}
	return;
}

void swap_buffers_tolast(){
	if(shellbuff.changed == 0){
		swap_buffer();
		shellbuff.changed = 1;
	}
	return;
}

void swap_buffers_tofirst(){
	if(shellbuff.changed == 1){
		swap_buffer();
		shellbuff.changed = 0;
	}
	return;
}

void swap_buffer(){
	static char auxb[SHELL_BUFFER_SIZE];
	int aux;
	aux = shellbuff.bpos;
	shellbuff.bpos = shellbuff.lastbpos;
	shellbuff.lastbpos = aux;
	aux = shellbuff.max_bpos;
	shellbuff.max_bpos = shellbuff.max_lastbpos;
	shellbuff.max_lastbpos = aux;
	for(aux = 0 ; aux < SHELL_BUFFER_SIZE ; aux++){
		auxb[aux] = shellbuff.buffer[aux];
		shellbuff.buffer[aux] = shellbuff.lastbuffer[aux];
		shellbuff.lastbuffer[aux] = auxb[aux];
	}
	return;
}

void prompt(){
    printf("%s", sdata.user);
    printf("%s", "@");
    printf("%s", sdata.computername);
    printf("%s", "$ ");
    return;
}

/*************************************************************
 * Every listener needed for shell is below.                 *
 *************************************************************/

void shell_keyboardListener(){
	unsigned char c;
	while((c=getchar())!=0){
		if(c == '\b'){
			shell_backspace();
		}
		else if(c == '\n'){
			shell_enter();
		}
		else if(c == S_UP){
			shell_up();
		}
		else if(c == S_DOWN){
			shell_down();
		}
		else if(c == S_LEFT){
			shell_left();
		}
		else if(c == S_RIGHT){
			shell_right();
		}
		else if(c == S_PGDN){
			shell_pagedown();
		}
		else if(c == S_DEL){
			shell_delete();
		}
		else if(c == S_INS){
			shell_insert();
		}
		else if(c == S_HOME){
			shell_home();
		}
		else if(c == S_END){
			shell_end();
		}
		else if(c == S_F1){
			shell_f1();
		}else{
			shell_print(c);
		}
	}
	return;
}

void shell_print(unsigned char c){
	if(shellbuff.bpos < SHELL_BUFFER_SIZE - 2){
		shellbuff.buffer[shellbuff.bpos++] = c;
		if(shellbuff.bpos > shellbuff.max_bpos){
			shellbuff.max_bpos++;
		}
		putchar(c);	
	}
	else if(shellbuff.bpos == SHELL_BUFFER_SIZE - 1){
		s_beep();
	}
	return;
}

void shell_enter(){
	if(shellbuff.buffer[0]!=0){
		putchar('\n');
		parsecommand(shellbuff.buffer);
		restart_shell_buffer();
		prompt();
	}
	return;
}

void shell_backspace(){
	if(shellbuff.bpos > 0){
		putchar('\b');
	}
	shellbuff.buffer[--shellbuff.bpos] = 0;
	return;
}

void shell_left(){
	if(shellbuff.bpos > 0){
		s_move_cursor_back();
		shellbuff.bpos--;
	}
	return;
}

void shell_right(){
	if(shellbuff.bpos > 0 && shellbuff.bpos < shellbuff.max_bpos){
		s_move_cursor_forward();
		shellbuff.bpos++;
	}
	return;
}

void shell_up(){
	if(shellbuff.changed == 0){
		swap_buffers_tolast();
		print_changed_buffer();
	}
	return;
}

void shell_down(){
	if(shellbuff.changed == 1){
		swap_buffers_tofirst();
		print_changed_buffer();
	}
	return;
}

void print_changed_buffer(){
	int i;
	for(i = 0 ; i < shellbuff.lastbpos ; i++){
		if(shellbuff.lastbpos > 0){
			putchar('\b');
		}
	}
	for(i = 0 ; i < shellbuff.bpos ; i++){
		putchar(shellbuff.buffer[i]);
	}
	return;
}

void shell_pagedown(){
	s_scrolldown();
	return;
}

void shell_delete(){
	if(shellbuff.bpos > 0){
		s_move_cursor_forward();
		shell_backspace();
	}
	return;
}

void shell_insert(){
	/* Future implementation */
	return;
}

void shell_home(){
	/* Future implementation */
	return;
}

void shell_end(){
	/* Future implementation */
	return;
}

void shell_f1(){
	putchar('\n');
	restart_shell_buffer();
	s_help(0, 0);
	prompt();
	return;
}

/*************************************************************
 * Every function needed to parse the command line is below. *
 *************************************************************/

void parsecommand(char* s){
	int clength = 0;
	int error = 0;
	int hasParameter = 0;
	getCommand(s);
	clength = strlen(shellbuff.command);
	if(shellbuff.buffer[clength]==' '){
		hasParameter = 1;
		getParameter(shellbuff.buffer, clength);
	}
	// Execute the requested command 
	if(!strcmp(shellbuff.command,"test")){
		error = s_test(hasParameter, shellbuff.parameter);
	}
	if(!strcmp(shellbuff.command,"help")){
		error = s_help(hasParameter, shellbuff.parameter);
	}
	else if(!strcmp(shellbuff.command,"bios")){
		error = s_checkBIOS(hasParameter, shellbuff.parameter);
	}
	else if(!strcmp(shellbuff.command, "exit")){
		error = s_exit(hasParameter, shellbuff.parameter);
	}
	else if(!strcmp(shellbuff.command, "reboot")){
		error = s_k_reboot(hasParameter, shellbuff.parameter); 
	}
	else if(!strcmp(shellbuff.command,"clear")){
		error = s_clear(hasParameter, shellbuff.parameter);
	}
	else if(!strcmp(shellbuff.command,"about")){
		error = s_about(hasParameter, shellbuff.parameter);
	}
	else if(!strcmp(shellbuff.command,"speed")){
		error = s_speed(hasParameter, shellbuff.parameter);
	}
	else if(!strcmp(shellbuff.command,"timestyle")){
		error = s_timestyle(hasParameter, shellbuff.parameter);
	}
	else if(!strcmp(shellbuff.command,"bkg")){
		error = s_bkg(hasParameter, shellbuff.parameter);
	}
	else if(!strcmp(shellbuff.command,"logout")){
		error = s_logout(hasParameter, shellbuff.parameter);
	}
	else {
		s_printError("COMMAND NOT FOUND\n");
		s_beep();
	}
	// Print error messages if needed
	switch(error){
		case 1:
			s_printAlert("Too many arguments.\n");
			break;
		case 2:
			s_printAlert("Too few arguments.\n");
			break;
		case 3:
			s_printAlert("Invalid argument.\n");
			break;
		case 4:
			s_printAlert("This command doesn't receive parameters.\n");
			break;
		default:
			break;
	}
	return;
}

void getCommand(char* s){
	int i, j;
	for(i=0; s[i]!=0 && s[i]!=' ' && i<SHELL_COMMAND_SIZE; i++){
		;
	}
	for(j=0; j<i; j++){
		shellbuff.command[j] = s[j];
	}
	return;
}

void getParameter(char* s, int clen){
	int i, j, k;
	j = clen + 1;
	for(i=j; s[i]!=0 && s[i]!=' ' && k<SHELL_PARAMETER_SIZE; i++){
		k++;
	}
	k = 0;
	while(j<i){
		shellbuff.parameter[k++] = s[j++];
	}
	return;
}

int s_about(int hp, char* p){
	if(hp){
		return 1;
	}else{
		s_printSuccess("\n                                 ");
		s_printSuccess(OS_NAME);
		s_printSuccess(" v");
		s_printSuccess(OS_VERSION);
		s_printSuccess("\n                        Developed by Alejandro Bezdjian\n\n");
		return 0;
	}
	return 1;
}

int s_help(int hp, char* p){
	if(hp){
		return 1;
	}else{
		printf("%s\n", "List of available commands: ");
		printf("\t%s\n", "* about: gives information about the system.");
		printf("\t%s\n", "* bios: gives information about the BIOS.");
		printf("\t%s\n", "* bkg: change the system background to the one given by parameter.");
		printf("\t%s\n", "* clear: erases screen content.");
		printf("\t%s\n", "* exit: turns off the computer.");
		printf("\t%s\n", "* help: gives information about the system commands.");
		printf("\t%s\n", "* logout: closes user session.");
		printf("\t%s\n", "* reboot: sends reboot signal.");
		printf("\t%s\n", "* speed: gives information about the system speed.");
		printf("%s\n", "Some commands can receive parameters.");
		printf("%s\n", "For more information try -help as parameter.");
		return 0;
	}
}

int s_checkBIOS(int hp, char* p){
	if(hp){
		return 4;
	}else{
		s_smb_BIOSinfo();
		return 0;
	}
}

int s_exit(int hp, char* p){
	if(hp){
		return 4;
	}else{
		s_shutdown();
		return 0;
	}
}

int s_k_reboot(int hp, char* p){
	if(hp){
		return 4;
	}else{
		s_reboot();
		return 0;
	}
}

int s_clear(int hp, char* p){
	if(hp){
		return 4;
	}else{
		s_clearScreen();
		return 0;
	}
}

int s_speed(int hp, char* p){
	if(hp){
		return 4;
	}else{
		// Not implemented yet
		return 0;
	}
}

int s_timestyle(int hp, char* p){
	if(hp){
		return 4;
	}else{
		s_switchTimeStyle();
		return 0;
	}
}

int s_bkg(int hp, char* p){
	if(hp){
		char color = -1;
		if(!strcmp(p,"-help")){
			printf("%s\n", "The available color parameters are: grey, blue, green and red.");
			return 0;
		}
		else if(!strcmp(p,"grey")){
			color = S_BACKGROUND_COLOR_LIGHT_GREY;
		}
		else if(!strcmp(p,"blue")){
			color = S_BACKGROUND_COLOR_BLUE;
		}
		else if(!strcmp(p,"green")){
			color = S_BACKGROUND_COLOR_GREEN;
		}
		else if(!strcmp(p,"red")){
			color = S_BACKGROUND_COLOR_RED;
		}else{
			return 3;
		}
		s_setBackgroundColor(color);
		return 0;
	}else{
		return 2;
	}
}

int s_logout(int hp, char* p){
	if(hp){
		return 4;
	}else{
		login_out();
		return 0;
	}
}

int s_test(int hp, char* p){
	if(hp){
		return 1;
	}else{
		/* Put something to test here */
		return 0;
	}
}