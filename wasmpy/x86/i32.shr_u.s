.globl _start
_start:
.intel_syntax noprefix
mov cx, 32
pop dx
pop dx
pop ax
idiv cx
pop ax
pop ax
shl eax, 16
pop ax
mov cl, dl
shr eax, cl
push ax
shr eax, 16
push ax
pushw 2
