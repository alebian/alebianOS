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

#ifndef _shell_
#define _shell_

#include "../../common.h"

#define SHELL_BUFFER_SIZE 128
#define SHELL_COMMAND_SIZE 16
#define SHELL_PARAMETER_SIZE 16

typedef struct{
	char buffer[SHELL_BUFFER_SIZE];
	char command[SHELL_COMMAND_SIZE];
	char parameter[SHELL_PARAMETER_SIZE];
	int bpos;
	int max_bpos;
	char lastbuffer[SHELL_BUFFER_SIZE];
	int lastbpos;
	int max_lastbpos;
	int changed;
} shell_buffer;

typedef struct{
	char* user;
	char* computername;	
} session_data;

void shell(char*, char*);
void start_shell_buffer(void);
void restart_shell_buffer(void);
void swap_buffers_tolast(void);
void swap_buffers_tofirst(void);
void swap_buffer(void);
void prompt(void);
void shell_enter(void);
void shell_backspace(void);
void shell_left(void);
void shell_right(void);
void shell_up(void);
void shell_down(void);
void print_changed_buffer(void);
void shell_pagedown(void);
void shell_delete(void);
void shell_insert(void);
void shell_home(void);
void shell_end(void);
void shell_print(unsigned char);
void shell_f1(void);
/* Listeners */
void shell_keyboardListener(void);
/* Functions to parse commands*/
void parsecommand(char*);
void getCommand(char*);
void getParameter(char*, int);
int s_about(int, char*);
int s_help(int, char*);
int s_checkBIOS(int, char*);
int s_exit(int, char*);
int s_k_reboot(int, char*);
int s_clear(int, char*);
int s_timestyle(int, char*);
int s_bkg(int, char*);
int s_logout(int, char*);
int s_test(int, char*);
int s_mouse(int, char*);
int s_cpu(int, char*);

#endif