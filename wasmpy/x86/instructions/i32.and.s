.globl _start
_start:
.intel_syntax noprefix
pop ax
pop ax
shl eax, 16
pop ax
pop cx
pop cx
shl ecx, 16
pop cx
and eax, ecx
push ax
shr eax, 16
push ax
pushw 2
