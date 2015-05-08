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

#ifndef _kernel_
#define _kernel_

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define FIRSTBYTE(x) x%16
#define SECONDBYTE(x) x-(FIRSTBYTE(x))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define OS_VERSION "0.1"
#define OS_NAME "alebianOS"

#define TRUE    1
#define FALSE   0
#define EOF     -1
#define NULL    0

typedef unsigned char  byte;
typedef short int 	   word;
typedef int 		   dword;
typedef long 		   qword;
typedef char 		   s8int;
typedef unsigned char  u8int;
typedef short		   s16int;
typedef unsigned short u16int;
typedef int 		   s32int;
typedef unsigned int   u32int;
typedef long 		   s64int;
typedef unsigned long  u64int;

typedef struct{
	char* init;
	int limit;
	int writepos;
	int scroll_downs;
} shell_file;

#pragma pack (1)    /* Allign structures to 1 byte */

void k_shutdown(void);
void k_reboot(void);
void k_setStartBarColor(char);
int k_isShellEnabled(void);
void k_enableShell(void);
void k_disableShell(void);
void k_printWithColor(char*, char);
void k_printwarning(char*);
void k_printerror(char*);
void k_printalert(char*);
void k_printOSmsg(char*);
void k_init_shell_file(void);
void init_shell_file(void);
void reset_shell_file(void);
void save_shell_row(char*);

#endif
