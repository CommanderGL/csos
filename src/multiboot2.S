#define ASM_FILE 1
#include "multiboot2.h"

#define GRUB_MULTIBOOT_ARCHITECTURE_I386 (0)
#define MULTIBOOT2_HEADER_LEN (multiboot_header_end - multiboot_header)
#define MULTIBOOT2_HEADER_CHECKSUM \
  -(MULTIBOOT2_HEADER_MAGIC \
  + GRUB_MULTIBOOT_ARCHITECTURE_I386 \
  + MULTIBOOT2_HEADER_LEN)

.section .multiboot2
  .align 8

  multiboot_header:
    .long   MULTIBOOT2_HEADER_MAGIC
    .long   GRUB_MULTIBOOT_ARCHITECTURE_I386
    .long   MULTIBOOT2_HEADER_LEN
    .long   MULTIBOOT2_HEADER_CHECKSUM

  tag_entry:
    .align 8
    .short MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
    .short 0
    .long 12
    .long _start
    
  tag_frame_buffer:
    .align 8
    .short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
    .short 0
    .long 20
    .long 1440
    .long 900
    .long 32

  tags_end:
    .align 8
    .short  MULTIBOOT_HEADER_TAG_END
    .short  0
    .long   8
  multiboot_header_end:

  .extern kernel_main

  .globl _start

  _start:
    # temporary stack
    mov     $stack_top, %esp

    # clear eflags
    pushl   $0
    popf

    pushl   %ebx
    /*  Push the magic value. */
    pushl   %eax

    call kernel_main

    # we shouldn't get here...
  .loop:
    hlt
    jmp .loop

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
