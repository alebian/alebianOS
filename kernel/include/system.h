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

#ifndef _system_
#define _system_

#include "main.h"
#include "arch/i386/i386.h"
#include "drivers/video.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"
#include "drivers/sound.h"
#include "drivers/timer.h"
#include "drivers/acpi.h"
#include "drivers/smbios.h"
#include "drivers/cpudet.h"
#include "klib.h"
#include "syscalls.h"
#include "gdt.h"
#include "idt.h"
#include "mm/pmm.h"
#include "mm/paging.h"
#include "mm/kheap.h"
#include "interrupts.h"
#include "panic.h"
#include "events.h"
#include "extras.h"
#include "multiboot.h"

#endif