SUDO := sudo
ASM := $(SUDO) nasm
ASMFLAGS := -f aout
CC := $(SUDO) gcc
CFLAGS := -Wall -c -fno-builtin
LINK := $(SUDO) ld
LINKFLAGS := -T

BIN := build/bin/

all: prepare kassembler kcsources link update iso

prepare:
	@sudo mkdir -p build/bin
	@sudo mkdir -p build/img
	@sudo mkdir -p build/isodir/boot/grub

kassembler:
	@echo "Compiling Kernel ASM..."
	@$(ASM) $(ASMFLAGS) kernel/arch/i386/start.asm -o $(BIN)start.o
	@$(ASM) $(ASMFLAGS) kernel/arch/i386/i386.asm -o $(BIN)i386.o
	@$(ASM) $(ASMFLAGS) kernel/arch/i386/cpuid.asm -o $(BIN)cpuid.o
	@echo "Compiling Userland ASM..."
	@$(ASM) $(ASMFLAGS) userland/syscall.asm -o $(BIN)syscall.o

kcsources:
	@echo "Compiling Kernel C..."
	@$(CC) $(CFLAGS) kernel/drivers/acpi.c -o $(BIN)acpi.o
	@$(CC) $(CFLAGS) kernel/drivers/cpudet.c -o $(BIN)cpudet.o
	@$(CC) $(CFLAGS) kernel/drivers/keyboard.c -o $(BIN)keyboard.o
	@$(CC) $(CFLAGS) kernel/drivers/mouse.c -o $(BIN)mouse.o
	@$(CC) $(CFLAGS) kernel/drivers/smbios.c -o $(BIN)smbios.o
	@$(CC) $(CFLAGS) kernel/drivers/sound.c -o $(BIN)sound.o
	@$(CC) $(CFLAGS) kernel/drivers/timer.c -o $(BIN)timer.o
	@$(CC) $(CFLAGS) kernel/drivers/video.c -o $(BIN)video.o
	@$(CC) $(CFLAGS) kernel/mm/kheap.c -o $(BIN)kheap.o
	@$(CC) $(CFLAGS) kernel/mm/paging.c -o $(BIN)paging.o
	@$(CC) $(CFLAGS) kernel/mm/pmm.c -o $(BIN)pmm.o
	@$(CC) $(CFLAGS) kernel/events.c -o $(BIN)events.o
	@$(CC) $(CFLAGS) kernel/extras.c -o $(BIN)extras.o
	@$(CC) $(CFLAGS) kernel/gdt.c -o $(BIN)gdt.o
	@$(CC) $(CFLAGS) kernel/idt.c -o $(BIN)idt.o
	@$(CC) $(CFLAGS) kernel/interrupts.c -o $(BIN)interrupts.o
	@$(CC) $(CFLAGS) kernel/klib.c -o $(BIN)klib.o
	@$(CC) $(CFLAGS) kernel/main.c -o $(BIN)main.o
	@$(CC) $(CFLAGS) kernel/panic.c -o $(BIN)panic.o
	@$(CC) $(CFLAGS) kernel/syscalls.c -o $(BIN)syscalls.o
	@echo "Compiling Userland C..."
	@$(CC) $(CFLAGS) userland/lib/ctype.c -o $(BIN)ctype.o
	@$(CC) $(CFLAGS) userland/lib/math.c -o $(BIN)math.o
	@$(CC) $(CFLAGS) userland/lib/stdio.c -o $(BIN)stdio.o
	@$(CC) $(CFLAGS) userland/lib/stdlib.c -o $(BIN)stdlib.o
	@$(CC) $(CFLAGS) userland/lib/string.c -o $(BIN)string.o
	@$(CC) $(CFLAGS) userland/lib/ulib.c -o $(BIN)ulib.o
	@$(CC) $(CFLAGS) userland/login.c -o $(BIN)login.o
	@$(CC) $(CFLAGS) userland/shell.c -o $(BIN)shell.o

link:
	@echo "Linkediting..."
	@$(LINK) $(LINKFLAGS) build/link.ld -o $(BIN)kernel.bin $(BIN)start.o $(BIN)i386.o $(BIN)cpuid.o $(BIN)syscall.o $(BIN)acpi.o $(BIN)cpudet.o $(BIN)keyboard.o $(BIN)mouse.o $(BIN)smbios.o $(BIN)sound.o $(BIN)timer.o $(BIN)video.o $(BIN)kheap.o $(BIN)paging.o $(BIN)pmm.o $(BIN)events.o $(BIN)extras.o $(BIN)gdt.o $(BIN)idt.o $(BIN)interrupts.o $(BIN)klib.o $(BIN)main.o $(BIN)panic.o $(BIN)syscalls.o $(BIN)ctype.o $(BIN)math.o $(BIN)stdio.o $(BIN)stdlib.o $(BIN)string.o $(BIN)ulib.o $(BIN)login.o $(BIN)shell.o

update:
	@echo "Updating Disk Image...";
	@sudo mount -o loop build/img/alebianOS.img /mnt
	@sudo cp build/bin/kernel.bin /mnt/boot
	@sudo umount /mnt

iso:
	@echo "Generating ISO...";
	@sudo cp build/bin/kernel.bin build/isodir/boot/kernel.bin
	@sudo cp build/grub.cfg build/isodir/boot/grub/grub.cfg
	@sudo cp /usr/lib/grub/i386-pc/stage2_eltorito  build/isodir/boot/grub/
	@sudo genisoimage -R -input-charset iso8859-1 -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o build/alebianOS.iso build/isodir

clean:
	@$(SUDO) $(RM) $(BIN)*
