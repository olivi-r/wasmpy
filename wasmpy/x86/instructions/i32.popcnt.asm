.globl _start
_start:
.intel_syntax noprefix
pop ax
pop ax
shl eax, 16
pop ax
popcnt eax, eax
push ax
pushw 0
pushw 2
