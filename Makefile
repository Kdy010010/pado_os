# ---- toolchain ----
CC=i686-elf-gcc
AS=nasm
LD=i686-elf-ld
OBJCOPY=i686-elf-objcopy
CFLAGS=-m32 -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall
LDFLAGS=-T linker.ld -m elf_i386

BOOT_OBJS = boot.o gdt.o idt.o
KERN_OBJS = kernel.o pstring.o pit.o task.o romfs.o elf.o ipc.o
OBJS = $(BOOT_OBJS) $(KERN_OBJS)

%.o: %.asm
	$(AS) -f elf32 $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# userland ELFs
INIT_ELFS = init/hello.elf init/hello2.elf
init/%.elf: init/%.c
	$(CC) $(CFLAGS) -Wl,-Ttext=0x0 -o $@ $<

romfs.bin: $(INIT_ELFS) tools/mkromfs.py
	python tools/mkromfs.py $@ $(INIT_ELFS)

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary $< $@

pado.iso: kernel.bin romfs.bin
	@mkdir -p isodir/boot
	cp kernel.bin isodir/boot/pado.bin
	cp romfs.bin  isodir/boot/romfs.bin
	grub-mkrescue -o $@ isodir >/dev/null 2>&1 || true

run: pado.iso
	qemu-system-i386 -cdrom pado.iso

clean:
	rm -rf *.o *.bin kernel.elf isodir pado.iso romfs.bin init/*.elf
