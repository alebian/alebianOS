#include "../include/shell.h"
#include "../include/lib/stdio.h"
#include "../include/lib/string.h"
#include "../include/kernel/k_libc.h"
#include "../include/drivers/keyboard.h"
#include "../include/drivers/video.h"

void prompt(){
    printf("%s",">$ ");
}

void parsecommand(char * s){
	if(!strcmp(s,"help")){
		help();
	}
	else if(!strcmp(s,"checkbios")){
		checkBIOSinfo();
		printf("\n");
	}
	else if(!strcmp(s, "exit")){
		turnOff();
	}
	else if(!strcmp(s,"clear")){
		k_clearScreen();
	}
	else if(!strcmp(s,"about")){
		about();
	}
	else if(!strcmp(s,"up")){
		scrollup();
	}
	else if(!strcmp(s,"dw")){
		scrolldown();
	}
	else {
		printf("alebian says: COMMAND NOT FOUND\n");
		//genericBeep();
	}
	return;
}

void about(){
	printf("AlebianOS V1.0\n");
	printf("Developed by Alejandro Bezdjian\n");
	return;
}

void help(){
	printf("%s\n", "List of available commands: ");
	printf("%s\n", "about, clear, exit, help, checkbios\n");
	return;
}

void shell(){
	while(1){
	prompt();
	char s[SHELL_BUFFER_SIZE];
	int i=0;
	char c;
		while( ((c=getchar()) != '\n') && i < SHELL_BUFFER_SIZE -1 ){
				if(c!=0){
					if(c == '\b' && i >= 1){
						i--;
						putchar(c);
					} else {
						if(i < SHELL_BUFFER_SIZE - 2){
							s[i] = c;
							i++;
							putchar(c);	
						}
						if(i == SHELL_BUFFER_SIZE - 1){
							//genericBeep();
						}
					}
				}
		}
	putchar(c);
	s[i]='\0';
	parsecommand(s);
	}
}