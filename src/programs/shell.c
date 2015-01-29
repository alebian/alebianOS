#include "../../include/system.h"

static char s[SHELL_BUFFER_SIZE];
static char command[SHELL_COMMAND_SIZE];
static char parameter[SHELL_PARAMETER_SIZE];
static int spos;
static int max_spos;
static char* user;
static char* computername;
static char str_start_bar[VGA_WIDTH+1];
static int start_menu = 0;
static char start_menu_str[] = "Reboot";
static char* shell_file;
/******************************************************/
/* WE SHOULD USE MALLOC BUT IT IS NOT IMPLEMENTED YET */
static address_t shell_file_phys;
/******************************************************/

void shell(char* username, char* pcname){
	shell_set_screen();
	/******************************************************/
	/* WE SHOULD USE MALLOC BUT IT IS NOT IMPLEMENTED YET */
	shell_file_phys = pop_frame();
	shell_file = (char*) shell_file_phys;
	/******************************************************/
	user = username;
	computername = pcname;
	k_shellReady();
	restart_shell_buffer();
	shell_updateStartBar();
	prompt();
	// Set the listeners
	k_setKeyboardListener(&shell_keyboardListener);
	k_setLclickListener(&shell_lclickListener);
	k_setRclickListener(&shell_rclickListener);
	k_setMclickListener(&shell_mclickListener);
	while(k_isOn() && login_isLogued()){}
	/******************************************************/
	/*  WE SHOULD USE FREE BUT IT IS NOT IMPLEMENTED YET  */
	push_frame(shell_file_phys);
	/******************************************************/
	return;
}

void shell_set_screen(){
	/* Prepare screen to show shell properly */
	k_clearFullScreen();
	k_setFullBackgroundColor(BACKGROUND_COLOR_BLACK);
	k_setCharacterColor(CHAR_COLOR_LIGHT_GREY);
	k_set_vga_size(3, 25);
	return;
}

void restart_shell_buffer(){
	int i;
	spos = 0;
	max_spos = 0;
	for(i = 0; i < SHELL_BUFFER_SIZE; i++){
		s[i] = 0;
	}
	for(i = 0; i < SHELL_COMMAND_SIZE; i++){
		command[i] = 0;
	}
	for(i = 0; i < SHELL_PARAMETER_SIZE; i++){
		parameter[i] = 0;
	}
	return;
}

void prompt(){
    printf("%s", user);
    printf("%s", "@");
    printf("%s", computername);
    printf("%s", "$ ");
    return;
}

void shell_updateStartBar(){
	memcpy(&str_start_bar, &START_LOGO, strlen(START_LOGO));
	memcpy(&str_start_bar[67], k_stime(), 13);
	k_printStartBar(str_start_bar);
	return;
}

/*************************************************************
 *                                                           *
 * Every listener needed for shell is below.                 *
 *                                                           *
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
		else if(c == UP){
			shell_up();
		}
		else if(c == DOWN){
			shell_down();
		}
		else if(c == LEFT){
			shell_left();
		}
		else if(c == RIGHT){
			shell_right();
		}
		else if(c == PGUP){
			shell_pageup();
		}
		else if(c == PGDN){
			shell_pagedown();
		}
		else if(c == DEL){
			shell_delete();
		}
		else if(c == INS){
			shell_insert();
		}
		else if(c == HOME){
			shell_home();
		}
		else if(c == END){
			shell_end();
		}
		else if(c == F1){
			shell_f1();
		}
		else if(c == F2){
			shell_f2();
		}
		else if(c == F3){
			shell_f3();
		}
		else if(c == F4){
			shell_f4();
		}
		else if(c == F5){
			shell_f5();
		}
		else if(c == F6){
			shell_f6();
		}
		else if(c == F7){
			shell_f7();
		}
		else if(c == F8){
			shell_f8();
		}
		else if(c == F9){
			shell_f9();
		}
		else if(c == F10){
			shell_f10();
		}
		else if(c == F11){
			shell_f11();
		}
		else if(c == F12){
			shell_f12();
		}else{
			shell_print(c);
		}
	}
	return;
}

void shell_print(unsigned char c){
	if(spos < SHELL_BUFFER_SIZE - 2){
		s[spos++] = c;
		if(spos > max_spos){
			max_spos++;
		}
		putchar(c);	
	}
	else if(spos == SHELL_BUFFER_SIZE - 1){
		k_beep();
	}
	return;
}

void shell_enter(){
	if(s[0]!=0){
		putchar('\n');
		parsecommand(s);
		restart_shell_buffer();
		prompt();
	}
	return;
}

void shell_backspace(){
	if(spos > 0){
		putchar('\b');
	}
	s[--spos]=0;
	return;
}

void shell_left(){
	if(spos > 0){
		k_move_cursor_back();
		spos--;
	}
	return;
}

void shell_right(){
	if(spos > 0 && spos < max_spos){
		k_move_cursor_forward();
		spos++;
	}
	return;
}

void shell_up(){
	/* Future implementation */
	return;
}

void shell_down(){
	/* Future implementation */
	return;
}

void shell_pageup(){
	/* Future implementation */
	return;
}

void shell_pagedown(){
	/* Future implementation */
	k_scrolldown();
	return;
}

void shell_delete(){
	/* Future implementation */
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

void shell_f2(){
	/* Future implementation */
	return;
}

void shell_f3(){
	/* Future implementation */
	return;
}

void shell_f4(){
	/* Future implementation */
	return;
}

void shell_f5(){
	/* Future implementation */
	return;
}

void shell_f6(){
	/* Future implementation */
	return;
}

void shell_f7(){
	/* Future implementation */
	return;
}

void shell_f8(){
	/* Future implementation */
	return;
}

void shell_f9(){
	/* Future implementation */
	return;
}

void shell_f10(){
	/* Future implementation */
	return;
}

void shell_f11(){
	/* Future implementation */
	return;
}

void shell_f12(){
	/* Future implementation */
	return;
}

void shell_lclickListener(int x, int y){
	if(y==0){
		if(x>=0 && x<strlen(START_LOGO)){
			if(!start_menu){
				openStartMenu();
				return;
			}else{
				closeStartMenu();
				return;
			}
		}
		else if( x>=(strlen(START_LOGO)+strlen(ARROW)) && x<(strlen(START_LOGO)+strlen(ARROW)+strlen(start_menu_str)) ){
			if(start_menu){
				k_reboot();
			}
		}
		else if(x>=72){
			int aux = k_getTimeStyle();
			switch(aux){
				case 2:
					aux=0;
					break;
				default:
					aux++;
					break;
			}
			k_setTimeStyle(aux);
			return;
		}else{
			closeStartMenu();
			return;
		}
	}else{
		if(start_menu){
			closeStartMenu();
			return;
		}
	}
	return;
}

void shell_rclickListener(int x, int y){
	shell_lclickListener(x, y);
	return;
}

void shell_mclickListener(int x, int y){
	k_randomVGAstyle();
	return;
}

void openStartMenu(){
	start_menu = 1;
	memcpy(&str_start_bar[strlen(START_LOGO)], &ARROW, strlen(ARROW));
	memcpy(&str_start_bar[strlen(START_LOGO)+strlen(ARROW)], &start_menu_str, strlen(start_menu_str));
	shell_updateStartBar();
	return;
}

void closeStartMenu(){
	start_menu = 0;
	memset(&str_start_bar[strlen(START_LOGO)], ' ', strlen(ARROW)+strlen(start_menu_str));
	shell_updateStartBar();
	return;
}

/*************************************************************
 *                                                           *
 * Every function needed to parse the command line is below. *
 *                                                           *
 *************************************************************/

void parsecommand(char* s){
	int clength = 0;
	int error = 0;
	int hasParameter = 0;
	getCommand(s);
	clength = strlen(command);
	if(s[clength]==' '){
		hasParameter = 1;
		getParameter(s, clength);
	}
	// Execute the requested command 
	if(!strcmp(command,"help")){
		error = s_help(hasParameter, parameter);
	}
	else if(!strcmp(command,"bios")){
		error = s_checkBIOS(hasParameter, parameter);
	}
	else if(!strcmp(command, "exit")){
		error = s_exit(hasParameter, parameter);
	}
	else if(!strcmp(command, "reboot")){
		error = s_k_reboot(hasParameter, parameter); 
	}
	else if(!strcmp(command,"clear")){
		error = s_clear(hasParameter, parameter);
	}
	else if(!strcmp(command,"about")){
		error = s_about(hasParameter, parameter);
	}
	else if(!strcmp(command,"speed")){
		error = s_speed(hasParameter, parameter);
	}
	else if(!strcmp(command,"timestyle")){
		error = s_timestyle(hasParameter, parameter);
	}
	else if(!strcmp(command,"bkg")){
		error = s_bkg(hasParameter, parameter);
	}
	else if(!strcmp(command,"logout")){
		error = s_logout(hasParameter, parameter);
	}
	else {
		k_printosmsg("alebian says: COMMAND NOT FOUND\n");
		k_beep();
	}
	// Print error messages if needed
	switch(error){
		case 1:
			k_printwarning("Too many arguments.\n");
			break;
		case 2:
			k_printwarning("Too few arguments.\n");
			break;
		case 3:
			k_printwarning("Invalid argument.\n");
			break;
		case 4:
			k_printwarning("This command doesn't receive parameters.\n");
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
		command[j] = s[j];
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
		parameter[k++] = s[j++];
	}
	return;
}

int s_about(int hp, char* p){
	if(hp){
		return 1;
	}else{
		k_printalert("\n                                 AlebianOS V");
		k_printalert(OS_VERSION);
		k_printalert("\n                        Developed by Alejandro Bezdjian\n\n");
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
		k_checkBIOSinfo();
		return 0;
	}
}

int s_exit(int hp, char* p){
	if(hp){
		return 4;
	}else{
		k_exitScreen();
		return 0;
	}
}

int s_k_reboot(int hp, char* p){
	if(hp){
		return 4;
	}else{
		k_reboot();
		return 0;
	}
}

int s_clear(int hp, char* p){
	if(hp){
		return 4;
	}else{
		k_clearScreen();
		return 0;
	}
}

int s_speed(int hp, char* p){
	if(hp){
		return 4;
	}else{
		k_checkSystemSpeed();
		return 0;
	}
}

int s_timestyle(int hp, char* p){
	if(hp){
		if(!strcmp(p,"-help")){
			printf("%s\n", "The available styles parameters are:");
			printf("\t%s\n", "* 0: date and time.");
			printf("\t%s\n", "* 1: time only.");
			printf("\t%s\n", "* 2: date only.");
			return 0;
		}
		else if(!strcmp(p,"0")){
			k_setTimeStyle(0);
		}
		else if(!strcmp(p,"1")){
			k_setTimeStyle(1);
		}
		else if(!strcmp(p,"2")){
			k_setTimeStyle(2);
		}else{
			return 3;
		}
		return 0;
	}else{
		return 2;
	}
}

int s_bkg(int hp, char* p){
	if(hp){
		char color = -1;
		if(!strcmp(p,"-help")){
			printf("%s\n", "The available color parameters are: black, blue, green and red.");
			return 0;
		}
		else if(!strcmp(p,"black")){
			color = BACKGROUND_COLOR_BLACK;
		}
		else if(!strcmp(p,"blue")){
			color = BACKGROUND_COLOR_BLUE;
		}
		else if(!strcmp(p,"green")){
			color = BACKGROUND_COLOR_GREEN;
		}
		else if(!strcmp(p,"red")){
			color = BACKGROUND_COLOR_RED;
		}else{
			return 3;
		}
		k_setBackgroundColor(color);
		return 0;
	}else{
		return 2;
	}
}

int s_logout(int hp, char* p){
	if(hp){
		return 4;
	}else{
		k_shellNotReady();
		login_out();
		return 0;
	}
}
