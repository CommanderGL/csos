CC := i686-elf-gcc
CXX := i686-elf-g++
LD := i686-elf-ld

CFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -g -pipe

CFILES := $(shell cd src && find -L * -type f -name '*.c')
CXXFILES := $(shell cd src && find -L * -type f -name '*.cpp')
ASFILES := $(shell cd src && find -L * -type f -name '*.S')
NASMFILES := $(shell cd src && find -L * -type f -name '*.asm')
OBJ := $(addprefix build/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o) $(CXXFILES:.cpp=.cpp.o))
HEADER_DEPS := $(addprefix build/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d) $(CXXFILES:.cpp=.cpp.d))

all: bin/csos-grub.bin

bin/csos-grub.bin: linker.ld $(OBJ)
	$(CC) -T linker.ld -o bin/csos-grub.bin -ffreestanding -O2 -nostdlib $(OBJ) -lgcc

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
	./geniso.sh

dev: all geniso
	qemu-system-i386 -cdrom bin/csos-grub.iso

clean:
	rm -rf bin/* build/**/*.o build/*.o
