.globl _start
_start:
.intel_syntax noprefix
pop ax
pop ax
shl eax, 16
pop ax
pop cx
shl ecx, 16
pop cx
popcnt eax, eax
popcnt ecx, ecx
add eax, ecx
push ax
pushw 0
pushw 0
pushw 0
pushw 4
