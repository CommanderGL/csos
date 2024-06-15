#!/bin/env zsh
mkdir -p isodir/boot/grub
cp bin/csos-grub.bin isodir/boot/
cp grub.cfg isodir/boot/grub/
grub-mkrescue -o bin/csos-grub.iso isodir
rm -rf isodir
