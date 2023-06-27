.globl _start
_start:
.intel_syntax noprefix
pop ax
pop ax
shl eax, 16
pop ax
cmp eax, 0
je zero
pop ax
cmp ax, 2
je nezerox32
pop ax
pop ax
nezerox32:
pop ax
pop ax
jmp end
zero:
pop ax
cmp ax, 2
je zerox32
pop ax
shl eax, 16
pop ax
pop cx
shl ecx, 16
pop cx
pop dx
pop dx
pop dx
pop dx
pop dx
push cx
shr ecx, 16
push cx
push ax
shr eax, 16
push ax
pushw 4
jmp end
zerox32:
pop ax
shl eax, 16
pop ax
pop cx
pop cx
pop cx
push ax
shr eax, 16
push ax
pushw 2
end:
