#include "../../include/system.h"

static shell_buffer shellbuff;
static session_data sdata;
static start_bar sbar;

void shell(char* username, char* pcname){
	shell_set_screen();
	sdata.user = username;
	sdata.computername = pcname;
	sbar.menu_opened = 0;
	k_init_shell_file();
	k_setStartMenu();
	k_enableShell();
	start_shell_buffer();
	shell_updateStartBar();
	prompt();
	// Set the listeners
	k_setKeyboardListener(&shell_keyboardListener);
	if(k_isMouseEnabled()){
		k_setLclickListener(&shell_lclickListener);
		k_setRclickListener(&shell_rclickListener);
		k_setMclickListener(&shell_mclickListener);
	}
	while(login_isLogued()){}
	/******************************************************/
	/*  WE SHOULD USE FREE BUT IT IS NOT IMPLEMENTED YET  */
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

void shell_updateStartBar(){
	memcpy(&sbar.str, &START_LOGO, strlen(START_LOGO));
	memcpy(&sbar.str[67], k_stime(), 13);
	k_printStartBar(sbar.str);
	return;
}

/*************************************************************
 * Every listener needed for shell is below.                 *
 *************************************************************/

void shell_keyboardListener(){
	unsigned char c;
	if(sbar.menu_opened){
		closeStartMenu();
	}
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
	if(shellbuff.bpos < SHELL_BUFFER_SIZE - 2){
		shellbuff.buffer[shellbuff.bpos++] = c;
		if(shellbuff.bpos > shellbuff.max_bpos){
			shellbuff.max_bpos++;
		}
		putchar(c);	
	}
	else if(shellbuff.bpos == SHELL_BUFFER_SIZE - 1){
		k_beep();
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
		k_move_cursor_back();
		shellbuff.bpos--;
	}
	return;
}

void shell_right(){
	if(shellbuff.bpos > 0 && shellbuff.bpos < shellbuff.max_bpos){
		k_move_cursor_forward();
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

void shell_pageup(){
	k_scrollup();
	return;
}

void shell_pagedown(){
	k_scrolldown();
	return;
}

void shell_delete(){
	k_deleteKey();
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
			if(!sbar.menu_opened){
				openStartMenu();
				return;
			}else{
				closeStartMenu();
				return;
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
	}
	else if(y==2){
		if(sbar.menu_opened){
			/* Shutdown */
			if(x>=0 && x<8){
				k_exitScreen();
			}
			closeStartMenu();
		}
		return;
	}
	else if(y==3){
		if(sbar.menu_opened){
			/* Reboot */
			if(x>=0 && x<6){
				k_reboot();
			}
			closeStartMenu();
		}
		return;
	}
	else if(y==4){
		if(sbar.menu_opened){
			/* Logout */
			if(x>=0 && x<8){
				k_disableShell();
				login_out();
			}
			closeStartMenu();
		}
		return;
	}
	else if(y==5){
		if(sbar.menu_opened){
			closeStartMenu();
			/* THEME: BLACK GREY RED BLUE */
			if(x==7){
				k_setVGAstyle(BACKGROUND_COLOR_BLACK, CHAR_COLOR_LIGHT_GREY, BACKGROUND_COLOR_RED, CHAR_COLOR_WHITE, BACKGROUND_COLOR_LIGHT_GREY);
			}
			if(x==8){
				k_setVGAstyle(BACKGROUND_COLOR_LIGHT_GREY, CHAR_COLOR_BLACK, BACKGROUND_COLOR_BLACK, CHAR_COLOR_LIGHT_GREY, BACKGROUND_COLOR_LIGHT_GREY);
			}
			if(x==9){
				k_setVGAstyle(BACKGROUND_COLOR_RED, CHAR_COLOR_WHITE, BACKGROUND_COLOR_BLACK, CHAR_COLOR_RED, BACKGROUND_COLOR_RED);
			}
			if(x==10){
				k_setVGAstyle(BACKGROUND_COLOR_BLUE, CHAR_COLOR_WHITE, BACKGROUND_COLOR_BLACK, CHAR_COLOR_RED, BACKGROUND_COLOR_BLUE);
			}
		}
		return;
	}
	else{
		if(sbar.menu_opened){
			closeStartMenu();
		}
		return;
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
	sbar.menu_opened = 1;
	k_printStartMenu();
	return;
}

void closeStartMenu(){
	sbar.menu_opened = 0;
	k_clearStartMenu();
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
		k_printalert("\n                                 ");
		k_printalert(OS_NAME);
		k_printalert(" v");
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
		k_disableShell();
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