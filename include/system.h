#ifndef _system_
#define _system_

#include "kernel/defs.h"
#include "kernel/idt.h"
#include "kernel/memory.h"
#include "kernel/kernel.h"
#include "kernel/k_libc.h"
#include "kernel/k_libasm.h"
#include "kernel/interrupts.h"
#include "kernel/buffers.h"
#include "kernel/smbios.h"
#include "drivers/video.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"
#include "drivers/sound.h"
#include "multiboot.h"
#include "shell.h"
#include "lib/ctype.h"
#include "lib/stdarg.h"
#include "lib/stdio.h"
#include "lib/stdlib.h"
#include "lib/string.h"
#include "lib/time.h"
#include "lib/math.h"

#endif