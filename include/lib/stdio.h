/*
* Copyright 2014 Alejandro Bezdjian
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

#ifndef _stdio_
#define _stdio_

#define MAX_BUFFER 50
#define MAX_FILES 10
#define MAX_STRING_LENGTH 80

/* Input */
unsigned char getchar(void);
int scanf(char*, ...);

/* Output */
int putchar(unsigned char);
int printf(char*, ...);
int puts(char*);

#endif
