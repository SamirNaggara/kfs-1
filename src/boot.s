; --- Part 1: Multiboot Header Constants ---
; Values required by the Multiboot specification (GRUB)
MAGIC    equ 0x1BADB002        ; Magic number to identify the Multiboot header
FLAGS    equ 0x0               ; Multiboot flags (0 = default/minimal)
CHECKSUM equ -(MAGIC + FLAGS)  ; Checksum to prove header validity

; --- Part 2: Multiboot Header Section ---
; This must be within the first 8KB of the kernel file (forced by linker.ld)
SECTION .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; --- Part 3: Stack Reservation ---
; The C environment requires a stack for local variables and function calls
SECTION .bss
align 16
stack_bottom:
    resb 16384                 ; Reserve 16 KB of uninitialized memory
stack_top:

; --- Part 4: Kernel Entry Point ---
SECTION .text
global start                   ; Make 'start' symbol visible to the linker
extern kmain                   ; kmain is defined in our C code

start:
    ; 1. Initialize Stack Pointer (ESP)
    mov esp, stack_top

    ; 2. Enter C Kernel
    call kmain

    ; 3. Infinite loop (Safeguard if kmain returns)
    cli                        ; Disable interrupts
.hang:
    hlt                        ; Put CPU in low-power sleep
    jmp .hang                  ; Re-enter sleep if awakened

; ------------------------------------------------------------------------------
; outb: Write 8-bit data to a hardware I/O port.
; C Prototype: void outb(unsigned short port, unsigned char data);
; ------------------------------------------------------------------------------
global outb
outb:
    mov al, [esp + 8]          ; Load 2nd argument (data) into AL
    mov dx, [esp + 4]          ; Load 1st argument (port) into DX
    out dx, al                 ; Send data to the port
    ret

; ------------------------------------------------------------------------------
; inb: Read 8-bit data from a hardware I/O port.
; C Prototype: unsigned char inb(unsigned short port);
; ------------------------------------------------------------------------------
global inb
inb:
    mov dx, [esp + 4]          ; Load 1st argument (port) into DX
    xor eax, eax               ; Clear EAX (return value register)
    in al, dx                  ; Read byte from port into AL
    ret                        ; C will read the result from AL/EAX