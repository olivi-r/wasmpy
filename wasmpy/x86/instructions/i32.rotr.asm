.globl _start
_start:
.intel_syntax noprefix
pop cx
pop cx
shl ecx, 16
pop cx
pop ax
pop ax
shl eax, 16
pop ax
ror eax, cl
push ax
shr eax, 16
push ax
pushw 2
