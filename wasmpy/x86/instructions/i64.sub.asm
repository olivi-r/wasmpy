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
pop dx
pop dx
shl edx, 16
pop dx
pop bx
shl ebx, 16
pop bx
sub ecx, ebx
sbb eax, edx
push cx
shr ecx, 16
push cx
push ax
shr eax, 16
push ax
pushw 4
