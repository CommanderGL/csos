#define ASM_FILE        1
#include "multiboot.h"

#ifdef HAVE_ASM_USCORE
# define EXT_C(sym) _ ## sym
#else
# define EXT_C(sym) sym
#endif

#define STACK_SIZE 0x4000

#ifdef __ELF__
# define AOUT_KLUDGE 0
#else
# define AOUT_KLUDGE MULTIBOOT_AOUT_KLUDGE
#endif
#define MULTIBOOT_HEADER_FLAGS MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_VIDEO_MODE | AOUT_KLUDGE

.text

.globl start, _start
start:
_start:
  jmp multiboot_entry
  .align 4

.section .multiboot
multiboot_header:
  .long MULTIBOOT_HEADER_MAGIC
  .long MULTIBOOT_HEADER_FLAGS
  .long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
#ifndef __ELF__
  .long multiboot_header
  .long _start
  .long _edata
  .long _end
  .long multiboot_entry
#else
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0       
#endif
  .long 0
  .long 1440
  .long 900
  .long 32

multiboot_entry:
  movl $(stack + STACK_SIZE), %esp

  pushl $0
  popf

  pushl   %ebx
  pushl   %eax
  call    EXT_C(kernel_main)

  pushl   $halt_message
  call    EXT_C(panic)
  
loop: hlt
  jmp     loop

halt_message:
  .asciz  "Halted."
  .comm   stack, STACK_SIZE
