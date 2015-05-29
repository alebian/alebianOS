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

#ifndef _login_
#define _login_

#define USERNAME_MAX_SIZE 32
#define PASSWORD_MAX_SIZE 32

int login(void);
void login_set_screen(void);
void login_restart_buffers(void);
void login_keyboardGetUserListener(void);
void login_keyboardGetPassListener(void);
int login_check(void);
void login_guest(void);
int login_isLogued(void);
void login_in(void);
void login_out(void);

#endif