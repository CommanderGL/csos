CC := i686-elf-gcc
CXX := i686-elf-g++
LD := i686-elf-ld

CFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -g -pipe -D__LIMINE__

CFILES := $(shell cd src && find -L * -type f -name '*.c')
CXXFILES := $(shell cd src && find -L * -type f -name '*.cpp')
ASFILES := $(shell cd src && find -L * -type f -name '*.S')
NASMFILES := $(shell cd src && find -L * -type f -name '*.asm')
OBJ := $(addprefix build/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o) $(CXXFILES:.cpp=.cpp.o))
HEADER_DEPS := $(addprefix build/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d) $(CXXFILES:.cpp=.cpp.d))

all: bin/csos-limine.bin

bin/csos-limine.bin: linker.ld $(OBJ)
	$(CC) -T linker.ld -o bin/csos-limine.bin -ffreestanding -O2 -nostdlib -no-pie $(OBJ) -lgcc

-include $(HEADER_DEPS)

build/%.c.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

build/%.cpp.o: src/%.cpp
	$(CXX) -c $< -o $@ $(CFLAGS)

build/%.S.o: src/%.S
	$(CC) -c $< -o $@ $(CFLAGS)

build/%.asm.o: src/%.asm
	nasm -Wall -f elf32 $< -o $@

geniso:
	git clone https://github.com/limine-bootloader/limine.git --branch=v7.x-binary --depth=1
	make -C limine
	mkdir -p isodir/boot/limine
	cp bin/csos-limine.bin isodir/boot/
	cp limine.cfg limine/limine-bios.sys limine/limine-bios-cd.bin \
		limine/limine-uefi-cd.bin isodir/boot/limine/
	mkdir -p isodir/EFI/BOOT
	cp limine/BOOTX64.EFI isodir/EFI/BOOT/
	cp limine/BOOTIA32.EFI isodir/EFI/BOOT/
	cp FATSCII.F16 isodir/
	xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
   	-no-emul-boot -boot-load-size 4 -boot-info-table \
   	--efi-boot boot/limine/limine-uefi-cd.bin \
   	-efi-boot-part --efi-boot-image --protective-msdos-label \
   	isodir -o bin/csos-limine.iso
	./limine/limine bios-install bin/csos-limine.iso
	rm -rf limine
	rm -rf isodir

dev: all geniso
	qemu-system-i386 -cdrom bin/csos-limine.iso -display gtk,show-tabs=on

dev-gdb: all geniso
	qemu-system-i386 -cdrom bin/csos-limine.iso -display gtk,show-tabs=on -s -S &
	gdb vmlinux

clean:
	rm -rf bin/* build/**/*.o build/*.o
