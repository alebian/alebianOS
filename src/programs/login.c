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
	login_set_screen();
	k_setLclickListener(&login_lclickListener);
	k_setRclickListener(&login_rclickListener);
	k_setMclickListener(&login_mclickListener);
	login_out();
	error = 1;
	while(error!=0){
		k_clearScreen();
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
			k_beep();
			k_sleep(10);
		}
	}
	login_in();
	while(k_isOn() && login_isLogued()){
		shell(username, pcname);
	}
	return;
}

void login_set_screen(){
	/* Prepare screen to show the login screen */
	k_clearFullScreen();
	k_set_vga_size(1, 25);
	k_setFullBackgroundColor(BACKGROUND_COLOR_BLACK);
	k_setCharacterColor(CHAR_COLOR_WHITE);
	k_printalert("\nWelcome to alebianOS ");
	k_printwarning("v");
	k_printwarning(OS_VERSION);
	printf("\n\n%s\n\n", "Please insert your username and password to login");
	printf("%s\n", "If you don't have a user you can log in as guest:");
	printf("\t%s", "* username: guest, with no password");
	if(k_isMouseEnabled()){
		printf("%s", ", or click here ----> ");
		k_printerror("GUEST\n\n");
		printf("%s", "To turn off your computer click here ----> ");
		k_printerror("TURN OFF\n");	
	}
	printf("\n%s", "________________________________________________________________________________");
	k_set_vga_size(12, 25);
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
				k_move_cursor_back();
				current--;
			}
		}
		else if(c == RIGHT){
			if(current > 0 && current < max_current){
				k_move_cursor_forward();
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
				k_beep();
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
				k_move_cursor_back();
				current--;
			}
		}
		else if(c == RIGHT){
			if(current > 0 && current < max_current){
				k_move_cursor_forward();
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
				k_beep();
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
			k_exitScreen();
		}
	}
	return;
}

void login_rclickListener(int x, int y){
	shell_lclickListener(x, y);
	return;
}

void login_mclickListener(int x, int y){
	shell_lclickListener(x, y);
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
