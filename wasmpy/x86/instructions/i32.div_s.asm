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
cmp ecx, 0
je end
mov edx, 0
idiv ecx
push ax
shr eax, 16
push ax
pushw 2
end:
