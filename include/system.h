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

#include "arch/x86/x86.h"
#include "arch/acpi.h"
#include "arch/smbios.h"
#include "kernel/kernel.h"
#include "kernel/syscalls.h"
#include "kernel/gdt.h"
#include "kernel/idt.h"
#include "kernel/mm/pmm.h"
#include "kernel/mm/paging.h"
#include "kernel/mm/kheap.h"
#include "kernel/main.h"
#include "kernel/interrupts.h"
#include "kernel/panic.h"
#include "kernel/events.h"
#include "kernel/extras.h"
#include "drivers/video.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"
#include "drivers/sound.h"
#include "drivers/timer.h"
#include "lib/ctype.h"
#include "lib/stdarg.h"
#include "lib/stdio.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "lib/math.h"
#include "programs/login.h"
#include "programs/shell.h"
#include "multiboot.h"

#endif