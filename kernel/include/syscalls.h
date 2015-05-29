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

#ifndef _syscalls_
#define _syscalls_

#include "system.h"
#include "../../common.h"

void k_syscall(uint32_t, uint32_t, uint32_t, uint32_t);
uint32_t sys_write(uint32_t, uint32_t, uint32_t);
uint32_t sys_read(uint32_t, uint32_t, uint32_t);
void sys_video(uint32_t, uint32_t, uint32_t);
void sys_sound(uint32_t, uint32_t, uint32_t);
void sys_listeners(uint32_t, uint32_t, uint32_t);
void sys_smbios(uint32_t, uint32_t, uint32_t);
void sys_timer(uint32_t, uint32_t, uint32_t);
void sys_io(uint32_t, uint32_t, uint32_t);

#endif