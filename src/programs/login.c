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

#include "../../include/system.h"

static int logout;
static char username[USERNAME_MAX_SIZE];
static char password[PASSWORD_MAX_SIZE];
static char* pcname = "x86";
static int current;
static int max_current;
static int error;
static int stage;

void login(){
	if(k_enableMouse()){
		k_setLclickListener(&login_lclickListener);	
	}
	login_set_screen();
	login_out();
	error = 1;
	while(error!=0){
		clearScreen();
		login_restart_buffers();
		stage = 1;
		printf("%s", "username: ");
		k_setKeyboardListener(&login_keyboardGetUserListener);
		while(stage == 1){}
		printf("%s", "password: ");
		k_setKeyboardListener(&login_keyboardGetPassListener);
		while(stage == 2){}
		error = login_check();
		if(error==1){
			k_printerror("\nInvalid username..");
		}
		if(error==2){
			k_printerror("\nIncorrect password..");
		}
		if(error!=0){
			beep();
			sleep(10);
		}
	}
	login_in();
	while(login_isLogued()){
		shell(username, pcname);
	}
	return;
}

void login_set_screen(){
	/* Prepare screen to show the login screen */
	clearFullScreen();
	set_vga_size(1, 25);
	setFullBackgroundColor(BACKGROUND_COLOR_BLACK);
	setCharacterColor(CHAR_COLOR_WHITE);
	k_printalert("\nWelcome to ");
	k_printalert(OS_NAME);
	k_printwarning(" v");
	k_printwarning(OS_VERSION);
	printf("\n\n%s\n\n", "Please insert your username and password to login");
	printf("%s\n", "If you don't have a user you can log in as guest:");
	printf("\t%s", "* username: guest, with no password");
	if(k_isMouseEnabled()){
		printf("%s", ", or click here ----> ");
		k_printerror("GUEST\n\n");
		printf("%s", "To turn off your computer click here ----> ");
		k_printerror("TURN OFF\n");	
	}else{
		printf("\n\n\n");
	}
	printf("\n%s", "________________________________________________________________________________");
	set_vga_size(12, 25);
	return;
}

void login_restart_buffers(){
	int i;
	current = 0;
	max_current = 0;
	for(i = 0; i < USERNAME_MAX_SIZE; i++){
		username[i] = 0;
	}
	for(i = 0; i < PASSWORD_MAX_SIZE; i++){
		password[i] = 0;
	}
	return;
}

void login_keyboardGetUserListener(){
	unsigned char c;
	while((c=getchar())!=0){
		if(c == '\b'){
			if(current > 0){
				putchar('\b');
			}
			username[--current]=0;
		}
		else if(c == '\n'){
			if(username[0]!=0){
				putchar('\n');
				stage = 2;
				current = 0;
				max_current = 0;
			}
		}
		else if(c == LEFT){
			if(current > 0){
				move_cursor_back();
				current--;
			}
		}
		else if(c == RIGHT){
			if(current > 0 && current < max_current){
				move_cursor_forward();
				current++;
			}
		}
		else if( isdigit(c) || isalpha(c) ){
			if(current < USERNAME_MAX_SIZE - 2){
				username[current++] = c;
				if(current > max_current){
					max_current++;
				}
				putchar(c);	
			}
			else if(current == USERNAME_MAX_SIZE - 1){
				beep();
			}
		}
	}
	return;
}

void login_keyboardGetPassListener(){
	unsigned char c;
	while((c=getchar())!=0){
		if(c == '\b'){
			if(current > 0){
				putchar('\b');
			}
			password[--current]=0;
		}
		else if(c == '\n'){
			putchar('\n');
			stage = 3;
		}
		else if(c == LEFT){
			if(current > 0){
				move_cursor_back();
				current--;
			}
		}
		else if(c == RIGHT){
			if(current > 0 && current < max_current){
				move_cursor_forward();
				current++;
			}
		}
		else if( isdigit(c) || isalpha(c) || isspecial(c) ){
			if(current < PASSWORD_MAX_SIZE - 2){
				password[current++] = c;
				if(current > max_current){
					max_current++;
				}
				putchar('*');	
			}
			else if(current == PASSWORD_MAX_SIZE - 1){
				beep();
			}
		}
	}
	return;
}

void login_lclickListener(int x, int y){
	if(y==6){
		if(x>=60 && x<=64){
			login_guest();
		}
	}
	else if(y==8){
		if(x>=43 && x<=50){
			k_shutdown();
		}
	}
	return;
}

int login_check(){
	int ans = 1;
	if(!strcmp(username,"guest")){
		ans = 0;
	}
	else if(!strcmp(username,"alebian")){
		if(!strcmp(password,"1234")){
			ans = 0;
		}else{
			ans = 2;
		}
	}
	return ans;
}

void login_guest(){
	strcpy(username, "guest");
	stage = 3;
	return;
}

int login_isLogued(){
	return !logout;
}

void login_in(){
	logout = 0;
	return;
}

void login_out(){
	logout = 1;
	return;
}
