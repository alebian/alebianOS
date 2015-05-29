SUDO := sudo
ASM := $(SUDO) nasm
ASMFLAGS := -f aout
CC := $(SUDO) gcc
CFLAGS := -Wall -c -fno-builtin
LINK := $(SUDO) ld
LINKFLAGS := -T

BIN := build/bin/

all: prepare assembler csources link update iso

prepare:
	@sudo mkdir -p build/bin
	@sudo mkdir -p build/img
	@sudo mkdir -p build/isodir/boot/grub

assembler:
	@echo "Compiling ASM..."
	@$(ASM) $(ASMFLAGS) src/arch/i386/start.asm -o $(BIN)start.o
	@$(ASM) $(ASMFLAGS) src/arch/i386/i386.asm -o $(BIN)i386.o

csources:
	@echo "Compiling C..."
	@$(CC) $(CFLAGS) src/arch/acpi.c -o $(BIN)acpi.o
	@$(CC) $(CFLAGS) src/arch/smbios.c -o $(BIN)smbios.o
	@$(CC) $(CFLAGS) src/kernel/main.c -o $(BIN)main.o
	@$(CC) $(CFLAGS) src/kernel/interrupts.c -o $(BIN)interrupts.o
	@$(CC) $(CFLAGS) src/kernel/kernel.c -o $(BIN)kernel.o
	@$(CC) $(CFLAGS) src/kernel/syscalls.c -o $(BIN)syscalls.o
	@$(CC) $(CFLAGS) src/kernel/gdt.c -o $(BIN)gdt.o
	@$(CC) $(CFLAGS) src/kernel/idt.c -o $(BIN)idt.o
	@$(CC) $(CFLAGS) src/kernel/mm/pmm.c -o $(BIN)pmm.o
	@$(CC) $(CFLAGS) src/kernel/mm/paging.c -o $(BIN)paging.o
	@$(CC) $(CFLAGS) src/kernel/mm/kheap.c -o $(BIN)kheap.o
	@$(CC) $(CFLAGS) src/kernel/panic.c -o $(BIN)panic.o
	@$(CC) $(CFLAGS) src/kernel/events.c -o $(BIN)events.o
	@$(CC) $(CFLAGS) src/kernel/extras.c -o $(BIN)extras.o
	@$(CC) $(CFLAGS) src/drivers/video.c -o $(BIN)video.o
	@$(CC) $(CFLAGS) src/drivers/keyboard.c -o $(BIN)keyboard.o
	@$(CC) $(CFLAGS) src/drivers/mouse.c -o $(BIN)mouse.o
	@$(CC) $(CFLAGS) src/drivers/sound.c -o $(BIN)sound.o
	@$(CC) $(CFLAGS) src/drivers/timer.c -o $(BIN)timer.o
	@$(CC) $(CFLAGS) src/lib/ctype.c -o $(BIN)ctype.o
	@$(CC) $(CFLAGS) src/lib/stdio.c -o $(BIN)stdio.o
	@$(CC) $(CFLAGS) src/lib/stdlib.c -o $(BIN)stdlib.o
	@$(CC) $(CFLAGS) src/lib/string.c -o $(BIN)string.o
	@$(CC) $(CFLAGS) src/lib/math.c -o $(BIN)math.o
	@$(CC) $(CFLAGS) src/programs/login.c -o $(BIN)login.o
	@$(CC) $(CFLAGS) src/programs/shell.c -o $(BIN)shell.o

link:
	@echo "Linkediting..."
	@$(LINK) $(LINKFLAGS) build/link.ld -o $(BIN)kernel.bin $(BIN)i386.o $(BIN)start.o $(BIN)acpi.o $(BIN)main.o $(BIN)gdt.o $(BIN)idt.o $(BIN)pmm.o $(BIN)paging.o $(BIN)kheap.o $(BIN)smbios.o $(BIN)panic.o $(BIN)events.o $(BIN)extras.o $(BIN)interrupts.o $(BIN)kernel.o $(BIN)syscalls.o $(BIN)video.o $(BIN)keyboard.o $(BIN)mouse.o $(BIN)sound.o $(BIN)timer.o $(BIN)ctype.o $(BIN)stdio.o $(BIN)stdlib.o $(BIN)string.o $(BIN)math.o $(BIN)login.o $(BIN)shell.o

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
