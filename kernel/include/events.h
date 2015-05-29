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

#ifndef _events_
#define _events_

typedef void(*klistener)(void);
typedef void(*clicklistener)(int, int);

void k_KeyboardListener(void);
void k_setKeyboardListener(void(*listener)(void));
void k_mouseRightClick(int, int);
void k_mouseLeftClick(int, int);
void k_mouseMidClick(int, int);
void k_setLclickListener(clicklistener);
void k_setRclickListener(clicklistener);
void k_setMclickListener(clicklistener);
void k_OSkblistener(void);
void k_OSlclcklistener(int, int);
void k_OSrclcklistener(int, int);
void k_OSmclcklistener(int, int);

#endif