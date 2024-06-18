[extern int_00]
[global int00]
int00:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_00    ; Divide by Zero #DE
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_01]
[global int01]
int01:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_01    ; Debug #DB
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_02]
[global int02]
int02:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_02    ; NMI interrupt #--
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_03]
[global int03]
int03:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_03    ; Breakpoint #BP
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_04]
[global int04]
int04:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_04    ; Overflow #OF
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_05]
[global int05]
int05:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_05    ; Bound Range Exception #BR
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_06]
[global int06]
int06:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_06    ; Invalid Opcode #UD
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_07]
[global int07]
int07:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_07    ; Device Not Available #NM
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_08]
[global int08]
int08:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_08    ; Double Fault #DF
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret


[extern int_09]
[global int09]
int09:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_09    ; Coprocessor Segment Overrun
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_10]
[global int10]
int10:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_10    ; Invalid Task State Segment (TSS) #TS
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_11]
[global int11]
int11:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_11    ;  Segment Not Present #NP
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_12]
[global int12]
int12:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_12    ; Stack Segment Fault #SS
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_13]
[global int13]
int13:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_13    ; General Protection Fault (GPF) #GP
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_14]
[global int14]
int14:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_14    ; Page Fault #PF
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

;; Exception 15 is an Intel Reserved Interrupt

[extern int_16]
[global int16]
int16:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_16    ; x87 Floating-Point Error #MF
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_17]
[global int17]
int17:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_17    ; Alignment Check #AC
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_18]
[global int18]
int18:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_18    ; Machine Check #MC
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern int_19]
[global int19]
int19:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call int_19    ; SIMD Floating-Point Exception #XF
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern keyboard_handler_c]
[global keyboard_handler]
keyboard_handler:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call keyboard_handler_c
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern timer_handler_c]
[global timer_handler]
timer_handler:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call timer_handler_c
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

[extern mouse_handler_c]
[global mouse_handler]
mouse_handler:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call mouse_handler_c
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret
