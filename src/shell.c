#include "../include/system.h"

static char s[SHELL_BUFFER_SIZE];
static char command[SHELL_COMMAND_SIZE];
static char parameter[SHELL_PARAMETER_SIZE];
static int spos;
static char* user = ">$ ";

void shell(){
	k_enableMouse();
	k_shellReady();
	k_setKeyboardListener(&shell_keyboardListener);
	restart_shell_buffer();
	k_updateStartBar();
	prompt();
	while(1){}
	return;
}

void shell_keyboardListener(){
	char c;
	while((c=getchar())!=0){
		if(c == '\b'){
			if(spos > 0){
				putchar(c);
				spos--;
			}
			if(spos==0){
				s[spos]=0;
			}
		}
		else if(c == '\n'){
			if(s[0]!=0){
				shellLineBreak();
				prompt();
				restart_shell_buffer();
			}
		}else{
			if(spos < SHELL_BUFFER_SIZE - 2){
				s[spos++] = c;
				putchar(c);	
			}
			if(spos == SHELL_BUFFER_SIZE - 1){
				beep();
			}
		}
	}
	return;
}

void restart_shell_buffer(){
	int i;
	spos = 0;
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

void shellLineBreak(){
	putchar('\n');
	s[spos]='\0';
	parsecommand(s);
	return;
}

void prompt(){
    printf("%s", user);
    return;
}

/*************************************************************
 *                                                           *
 * Every function needed to parse the command line is below. *
 *                                                           *
 *************************************************************/

void parsecommand(char * s){
	int clength = 0;
	int error = 0;
	int hasParameter = 0;
	getCommand(s);
	clength = strlen(command);
	if(s[clength]!=0 && s[clength+1]=='-'){
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
	else if(!strcmp(command,"time")){
		error = s_time(hasParameter, parameter);
	}
	else if(!strcmp(command,"timestyle")){
		error = s_timestyle(hasParameter, parameter);
	}
	else if(!strcmp(command,"bkg")){
		error = s_bkg(hasParameter, parameter);
	}
	else {
		k_printosmsg("alebian says: COMMAND NOT FOUND\n");
		beep();
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

void getCommand(char * s){
	int i, j;
	for(i=0; s[i]!=0 && s[i]!=' '; i++){
		;
	}
	for(j=0; j<i; j++){
		command[j] = s[j];
	}
	command[j] = 0;
	return;
}

void getParameter(char * s, int clen){
	int i, j, k;
	j = clen + 1;
	for(i=j; s[i]!=0 && s[i]!=' '; i++){
		;
	}
	k = 0;
	for( ; j<i; ){
		parameter[k++] = s[j++];
	}
	parameter[k] = 0;
	return;
}

int s_about(int hp, char * p){
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

int s_help(int hp, char * p){
	if(hp){
		return 1;
	}else{
		printf("%s\n", "Every command in this OS can receive 1 parameter at most.");
		printf("%s\n", "List of available commands: ");
		printf("\t%s\n", "* about: gives information about the system.");
		printf("\t%s\n", "* bios: gives information about the BIOS.");
		printf("\t%s\n", "* bkg: change the system background to the one given by parameter.");
		printf("\t%s\n", "* clear: erases screen content.");
		printf("\t%s\n", "* exit: turns off the computer.");
		printf("\t%s\n", "* help: gives information about the system commands.");
		printf("\t%s\n", "* reboot: sends reboot signal.");
		printf("\t%s\n", "* speed: gives information about the system speed.");
		printf("\t%s\n", "* time: shows the current time.");
		printf("%s\n", "For more information try -help as parameter.");
		return 0;
	}
}

int s_checkBIOS(int hp, char * p){
	if(hp){
		return 4;
	}else{
		k_checkBIOSinfo();
		return 0;
	}
}

int s_exit(int hp, char * p){
	if(hp){
		return 4;
	}else{
		k_turnOff();
		return 0;
	}
}

int s_k_reboot(int hp, char * p){
	if(hp){
		return 4;
	}else{
		k_reboot();
		return 0;
	}
}

int s_clear(int hp, char * p){
	if(hp){
		return 4;
	}else{
		k_clearScreen();
		return 0;
	}
}

int s_speed(int hp, char * p){
	if(hp){
		return 4;
	}else{
		k_checkSystemSpeed();
		return 0;
	}
}

int s_time(int hp, char * p){
	if(hp){
		return 4;
	}else{
		printf("%s\n", k_currentTime());
		return 0;
	}
}

int s_timestyle(int hp, char * p){
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

int s_bkg(int hp, char * p){
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
