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

#ifndef _ints_
#define _ints_

void int_00(void);
void int_01(void);
void int_02(void);
void int_03(void);
void int_04(void);
void int_05(void);
void int_06(void);
void int_07(void);
void int_08(void);
void int_09(void);
void int_10(void);
void int_11(void);
void int_12(void);
void int_13(void);
void int_14(int, int);
void int_16(void);
void int_80(dword, dword, dword, dword);
void irq_00(void);
void irq_01(int);
void irq_12(int);
long getTicks(void);
int k_enableMouse(void);
int k_disableMouse(void);
int k_isMouseEnabled(void);

#endif