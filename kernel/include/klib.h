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

#ifndef _klib_
#define _klib_

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define FIRST_BYTE(x) x%16
#define SECOND_BYTE(x) x-(FIRST_BYTE(x))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#pragma pack (1)    /* Allign structures to 1 uint8_t */

void k_printf(char* s);
void k_printWithColor(char* s, char color);
void k_printWarning(char* s);
void k_printError(char* s);
void k_printSuccess(char* s);
void k_disable(void);
void k_shutdown(void);
void k_reboot(void);
void* k_memcpy(void*, const void*, int);
void* k_memset(void*, int, int);
int k_memcmp(const void*, const void*, int);
int k_strlen(char*);

#endif
