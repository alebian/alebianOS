/*
* Copyright 2015 Alejandro Bezdjian
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

#ifndef _ulib_
#define _ulib_

void s_beep(void);
void s_setKBlistener(int);
void s_printError(char*);
void s_printAlert(char*);
void s_printSuccess(char*);
void s_sleep(int);
void s_clearScreen(void);
void s_move_cursor_back(void);
void s_move_cursor_forward(void);
void s_setBackgroundColor(char);
void s_switchTimeStyle(void);
void s_shutdown(void);
void s_reboot(void);
void s_scrolldown(void);
void s_smb_BIOSinfo(void);

#endif